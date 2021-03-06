/*****************************************************************************
 * 
 * This file is part of Mapnik (c++ mapping toolkit)
 *
 * Copyright (C) 2006,2009 Artem Pavlenko
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

#include <cstdlib>
#include <mapnik/unicode.hpp>

#include <string>

#ifdef MAPNIK_DEBUG
#include <iostream>
#endif 

namespace mapnik {

transcoder::transcoder (std::string const& encoding)
    : ok_(false),
      conv_(0)
{
    UErrorCode err = U_ZERO_ERROR;
    conv_ = ucnv_open(encoding.c_str(),&err);
    if (U_SUCCESS(err)) ok_ = true;
    // TODO ??
}

UnicodeString transcoder::transcode(const char* data) const
{
    
    UErrorCode err = U_ZERO_ERROR;
    
    UnicodeString ustr(data,-1,conv_,err); 
    if (ustr.isBogus())
    {
        ustr.remove();
    }
    return ustr;
}

transcoder::~transcoder()
{
    if (conv_) ucnv_close(conv_);
}   
}
