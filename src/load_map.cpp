/*****************************************************************************
 * 
 * This file is part of Mapnik (c++ mapping toolkit)
 *
 * Copyright (C) 2006 Artem Pavlenko
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 *****************************************************************************/


#include <iostream>

#include <boost/foreach.hpp>
#include <boost/optional.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "color.hpp"
#include "color_factory.hpp"
#include "filter_factory.hpp"
#include "layer.hpp"
#include "datasource_cache.hpp"
#include "load_map.hpp"

namespace mapnik 
{
    void load_map(Map & map, std::string const& filename)
    {
        using boost::property_tree::ptree;
        ptree pt;
        
        read_xml(filename,pt);
        
        boost::optional<std::string> bgcolor = 
            pt.get_optional<std::string>("Map.<xmlattr>.bgcolor");
        if ( bgcolor)
        {
            Color bg = color_factory::from_string(bgcolor->c_str());
            map.setBackground(bg);
        }
        
        BOOST_FOREACH (ptree::value_type & v, pt.get_child("Map"))
        {
            if (v.first == "Style")
            {
                std::string name = v.second.get<std::string>("<xmlattr>.name");
                feature_type_style style;
                
                BOOST_FOREACH(ptree::value_type & rule_tag, v.second)
                {
                    if (rule_tag.first == "Rule")
                    {
                        std::string name = 
                            rule_tag.second.get<std::string>("<xmlattr>.name","");
                        std::string title = 
                            rule_tag.second.get<std::string>("<xmlattr>.title","");
                        rule_type rule(name,title);

                        boost::optional<std::string> filter_expr = 
                            rule_tag.second.get_optional<std::string>("Filter");
                        
                        if (filter_expr)
                        {
                            rule.set_filter(create_filter(*filter_expr));
                        }
                        
                        boost::optional<double> min_scale = 
                            rule_tag.second.get_optional<double>("MinScaleDenominator");
                        if (min_scale)
                        {
                            rule.set_min_scale(*min_scale);
                        }
                        
                        boost::optional<double> max_scale = 
                            rule_tag.second.get_optional<double>("MaxScaleDenominator");
                        if (max_scale) 
                        {
                            rule.set_max_scale(*max_scale);   
                        }    
                        
                        BOOST_FOREACH(ptree::value_type & sym, rule_tag.second)
                        {
                            if ( sym.first == "PointSymbolizer")
                            {
                                std::cout << sym.first << "\n";
                            } 
                            else  if ( sym.first == "LineSymbolizer")
                            {
                                stroke strk;
                                BOOST_FOREACH (ptree::value_type & css, sym.second)
                                {
                                    std::string css_name  = 
                                        css.second.get<std::string>("<xmlattr>.name");
                                    std::string data = css.second.data();
                                    if (css_name == "stroke")
                                    {
                                        Color c = color_factory::from_string(css.second.data().c_str());
                                        strk.set_color(c);
                                    }
                                    else if (css_name == "stroke-width")
                                    {
                                        try 
                                        {
                                            float width = lexical_cast<float>(data);
                                            strk.set_width(width);
                                        }
                                        catch (bad_lexical_cast & ex)
                                        {
                                            std::clog << ex.what() << "\n";
                                        }
                                    }
                                    else if (css_name == "stroke-opacity")
                                    {
                                        try 
                                        {
                                            float opacity = lexical_cast<float>(data);
                                            strk.set_opacity(opacity);
                                        }
                                        catch (bad_lexical_cast & ex)
                                        {
                                            std::clog << ex.what() << "\n";
                                        }
                                    }
                                    else if (css_name == "stroke-linejoin")
                                    {
                                         if ("miter" == data)
                                         {
                                             strk.set_line_join(mapnik::MITER_JOIN);
                                         }
                                         else if ("round" == data)
                                         {
                                             strk.set_line_join(mapnik::ROUND_JOIN);
                                         }
                                         else if ("bevel" == data)
                                         {
                                             strk.set_line_join(mapnik::BEVEL_JOIN);
                                         }
                                    }
                                    else if (css_name == "stroke-linecap")
                                    {
                                        if ("round" == data)
                                        {
                                            strk.set_line_cap(mapnik::ROUND_CAP);
                                        }
                                        else if ("butt" == data)
                                        {
                                            strk.set_line_cap(mapnik::BUTT_CAP);
                                        }
                                        else if ("square" == data)
                                        {
                                            strk.set_line_cap(mapnik::SQUARE_CAP);
                                        }
                                    }
                                    else if (css_name == "stroke-dasharray")
                                    {
                                        tokenizer<> tok (data);
                                        std::vector<float> dash_array;
                                        for (tokenizer<>::iterator itr = tok.begin(); itr != tok.end(); ++itr)
                                        {
                                            try 
                                            {
                                                float f = boost::lexical_cast<float>(*itr);
                                                dash_array.push_back(f);
                                            }
                                            catch ( boost::bad_lexical_cast & ex)
                                            {
                                                std::clog << ex.what() << "\n";
                                            }
                                        }
                                        if (dash_array.size())
                                        {
                                            size_t size = dash_array.size();
                                            if ( size % 2) 
                                            { 
                                                for (size_t i=0; i < size ;++i)
                                                {
                                                    dash_array.push_back(dash_array[i]);
                                                }
                                            }
                                            std::vector<float>::const_iterator pos = dash_array.begin();
                                            while (pos != dash_array.end())
                                            {
                                                strk.add_dash(*pos,*(pos + 1));
                                                pos +=2;
                                            }
                                        }
                                    }
                                }
                                rule.append(line_symbolizer(strk));
                            } 
                            else if ( sym.first == "PolygonSymbolizer")
                            {
                                polygon_symbolizer poly_sym;
                                BOOST_FOREACH (ptree::value_type & css, sym.second)
                                {
                                    std::string css_name  = 
                                        css.second.get<std::string>("<xmlattr>.name");
                                    std::string data = css.second.data();
                                    if (css_name == "fill")
                                    {
                                        Color c = color_factory::from_string(css.second.data().c_str());
                                        poly_sym.set_fill(c);
                                    }
                                    else if (css_name == "fill-opacity")
                                    {
                                        try 
                                        {
                                            float opacity = lexical_cast<float>(data);
                                            poly_sym.set_opacity(opacity);
                                        }
                                        catch (bad_lexical_cast & ex)
                                        {
                                            std::clog << ex.what() << "\n";
                                        }
                                    }
                                }
                                rule.append(poly_sym);
                            }
                            else if ( sym.first == "TextSymbolizer")
                            {
                                std::cout << sym.first << "\n";
                            } 
                            else if ( sym.first == "RasterSymbolizer")
                            {
                                rule.append(raster_symbolizer());
                            } 
                        } 
                        
                        style.add_rule(rule);
                    }
                }
                
                map.insert_style(name, style);
                
            }
            else if (v.first == "Layer")
            {
                
                std::string name = v.second.get<std::string>("<xmlattr>.name","");
                Layer lyr(name);
                
                boost::optional<std::string> status = 
                    v.second.get<std::string>("<xmlattr>.status");
                
                if (status && *status == "off")
                {
                    lyr.setActive(false);
                }
                
                
                BOOST_FOREACH (ptree::value_type & child, v.second)
                {
                    if (child.first == "StyleName")
                    {
                        lyr.add_style(child.second.data());
                    }
                    else if (child.first == "Datasource")
                    {
                        parameters params;
                        BOOST_FOREACH (ptree::value_type & param_tag, child.second)
                        {

                            if (param_tag.first == "Parameter")
                            {
                                std::string name = param_tag.second.get<std::string>("<xmlattr>.name");
                                std::string value = param_tag.second.data();
                                std::clog << "name = " << name << " value = " << value << "\n";
                                params[name] = value; 
                            }
                        }
                        //now we're ready to create datasource 
                        boost::shared_ptr<datasource> ds = datasource_cache::instance()->create(params);
                        lyr.set_datasource(ds);
                    }
                }
                map.addLayer(lyr);
            }
        }
    }   
}
