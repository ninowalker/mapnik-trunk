/*****************************************************************************
 * 
 * This file is part of Mapnik (c++ mapping toolkit)
 *
 * Copyright (C) 2006 Artem Pavlenko, Jean-Francois Doyon
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

#include <boost/python.hpp>
#include <mapnik/image_util.hpp>
#include <mapnik/image_view.hpp>

using mapnik::ImageData32;
using mapnik::image_view;
using mapnik::save_to_file;

void export_image_view()
{
    using namespace boost::python;
    class_<image_view<ImageData32> >("ImageView","A view into an image.",no_init)
       .def("width",&image_view<ImageData32>::width)
       .def("height",&image_view<ImageData32>::height)
       ;
    
    //def("save_to_file",save_to_file<image_view<ImageData32> >);
}
