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
#ifndef UNICODE_HPP
#define UNICODE_HPP

#include <string>

namespace mapnik {
    
    inline std::wstring to_unicode(std::string const& text)
    {
        std::wstring out;
        unsigned long code = 0;
        int expect = 0;
        std::string::const_iterator itr=text.begin();
        
        while ( itr != text.end())
        {
            unsigned p = (*itr++) & 0xff;
            if ( p >= 0xc0)
            {
                if ( p < 0xe0)      // U+0080 - U+07ff
                {
                    expect = 1;
                    code = p & 0x1f;
                }
                else if ( p < 0xf0)  // U+0800 - U+ffff
                {
                    expect = 2;
                    code = p & 0x0f;
                }
                else if ( p  < 0xf8) // U+1000 - U+10ffff
                {
                    expect = 3;
                    code = p & 0x07;
                }
                continue;
            }
            else if (p >= 0x80)
            {
                --expect;
                if (expect >= 0)
                {
                    code <<= 6;
                    code += p & 0x3f;
                }
                if (expect > 0)
                    continue;
                expect = 0;
            }
            else 
            {
                code = p;            // U+0000 - U+007f (ascii)
            }
            out.push_back(wchar_t(code));
        }
        return out;
    }
}

#endif // UNICODE_HPP