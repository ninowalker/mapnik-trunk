/* This file is part of Mapnik (c++ mapping toolkit)
 * Copyright (C) 2005 Artem Pavlenko
 *
 * Mapnik is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

//$Id$

#include "filter_parser.hh"

using std::string;

namespace mapnik
{
    template<typename FeatureT>
    class filter_factory
    {
    public:
	filter_factory() {}
	filter_ptr compile(string const& str) const
	{
	    stack<ref_ptr<filter<FeatureT> > > filters;
	    stack<ref_ptr<expression<FeatureT> > > exps;
	    filter_grammar<FeatureT>  grammar(filters,exps);
	    char const *text = str.c_str();
	    parse_info<> info = parse(text,text+strlen(text),grammar,space_p);
	    if (info.full && !filters.empty())
	    {
		cout<<"success parsing filter expression\n";
		return filters.top();	
	    }
	    else 
	    {
		cout << "failed at: \": " << info.stop << "\n";
		return filter_ptr(new null_filter<FeatureT>());
	    }  
	}

    };
}
