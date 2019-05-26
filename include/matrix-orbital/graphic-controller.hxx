//
// $Header: /Projects/lcd/include/matrix-orbital/controller.hxx 3     6/21/99 10:00a Alpern $
//
//    File: <matrix-orbital/graphic-controller.hxx>
//
//    A controller class for the Matrix Orbital series of LCD/VFD
//    displays. See http://www.matrix-orbital.com/. They're cool!
//
//    Copyright (C) 1998,1999 Adam Alpern
//    Email: alpern@brightware.com
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public License
// as published by the Free Software Foundation; either version 2 of
// the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA
//

#ifndef MATRIX_ORBITAL_GRAPHIC_CONTROLLER_HXX
#define MATRIX_ORBITAL_GRAPHIC_CONTROLLER_HXX 1

#include <matrix-orbital/controller.hxx>

class MATRIX_ORBITAL_DLL MatrixOrbitalGraphicController
    : public MatrixOrbitalController
{
  public:
    virtual ~MatrixOrbitalGraphicController();

    // -------------------------------------------------------------
    // Command Set
    // -------------------------------------------------------------

    void set_current_font(short font_id);
    void set_font_metrics(short left_margin, short top_margin, short x_space, short y_space, short scroll_row);

    void set_drawing_color(short value);
    void draw_line(short x1, short y1, short x2, short y2);
    void continue_line(short x1, short y1);
    void put_pixel(short x, short y);
    void draw_rectangle(short color, short x1, short y1, short x2, short y2);
    void initialize_bar_graph(short reference_number, short type, short x1, short y1, short x2, short y2);
    void write_to_bar_graph(short reference_number, short value);
    void set_graphic_position(short x, short y);
    void display_bitmap(short reference_number, short x, short y);

  protected:
};

#endif // MATRIX_ORBITAL_GRAPHIC_CONTROLLER_HXX
