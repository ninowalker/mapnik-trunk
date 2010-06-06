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

//$Id$

#ifndef MARKERS_SYMBOLIZER_HPP
#define MARKERS_SYMBOLIZER_HPP

//mapnik
#include <mapnik/symbolizer.hpp>
#include <mapnik/path_expression_grammar.hpp>
#include <mapnik/color.hpp>

namespace mapnik {
   
struct MAPNIK_DECL markers_symbolizer : 
        public symbolizer_with_image
{
public:
    markers_symbolizer()
        : symbolizer_with_image(path_expression_ptr(new path_expression)),
        allow_overlap_(false),
        fill_(color(0,0,255)), 
        spacing_(100.0), 
        max_error_(0.2) {}
        
    markers_symbolizer(path_expression_ptr filename, bool allow_overlap=false) 
        : symbolizer_with_image(filename), 
        allow_overlap_(allow_overlap),
        fill_(color(0,0,255)), 
        spacing_(100.0), 
        max_error_(0.2) {}
    
    markers_symbolizer(markers_symbolizer const& rhs) 
        : symbolizer_with_image(rhs), 
        allow_overlap_(rhs.allow_overlap_),
        fill_(rhs.fill_), 
        spacing_(rhs.spacing_), 
        max_error_(rhs.max_error_) {}
    
    void set_allow_overlap(bool overlap)
    {
        allow_overlap_ = overlap;
    }
    
    bool get_allow_overlap() const
    {
        return allow_overlap_;
    }

    void set_spacing(double spacing)
    {
        spacing_ = spacing;
    }
    
    float get_spacing() const
    {
        return spacing_;
    }

    void set_max_error(double max_error)
    {
        max_error_ = max_error;
    }
    
    float get_max_error() const
    {
        return max_error_;
    }
    
    void set_fill(color fill)
    {
        fill_ = fill;
    }
    
    color const& get_fill() const
    {
        return fill_;
    }
    
private:
    bool allow_overlap_;
    color fill_;
    double spacing_;
    double max_error_;
};
}

#endif // MARKERS_SYMBOLIZER_HPP
