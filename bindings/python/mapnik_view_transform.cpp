/*****************************************************************************
 * 
 * This file is part of Mapnik (c++ mapping toolkit)
 *
 * Copyright (C) 2009 Artem Pavlenko, Jean-Francois Doyon
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

// boost
#include <boost/python.hpp>

// mapnik
#include <mapnik/ctrans.hpp>

namespace {

   mapnik::coord2d forward_point(mapnik::CoordTransform const& t, mapnik::coord2d const& in)
   {
      mapnik::coord2d out(in);
      t.forward(out);
      return out;
   }
   
   mapnik::coord2d backward_point(mapnik::CoordTransform const& t, mapnik::coord2d const& in)
   {
      mapnik::coord2d out(in);
      t.backward(out);
      return out;
   }
   
   mapnik::Envelope<double> forward_envelope(mapnik::CoordTransform const& t, mapnik::Envelope<double> const& in)
   {
      return t.forward(in);
   }
   
   mapnik::Envelope<double> backward_envelope(mapnik::CoordTransform const& t, mapnik::Envelope<double> const& in)
   {
      return t.backward(in);
   }
}

void export_view_transform() 
{
   using namespace boost::python;
   using mapnik::Envelope;
   using mapnik::CoordTransform;
   using mapnik::coord2d;
   
   class_<CoordTransform>("ViewTransform",init<int,int,Envelope<double> const& > (
      "Create a ViewTransform with a width and height as integers and extent"))
      .def("forward", forward_point)
      .def("backward",backward_point)
      .def("forward", forward_envelope)
      .def("backward",backward_envelope)
      .def("scale_x",&CoordTransform::scale_x)
      .def("scale_y",&CoordTransform::scale_y)
      ;
}
