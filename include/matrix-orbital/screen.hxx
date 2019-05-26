//
// $Header: /Projects/lcd/include/matrix-orbital/screen.hxx 2     6/08/99 9:46a Alpern $
//
//    File: <matrix-orbital/screen.hxx>
//
//    Defines a class which can be subclassed to define new display
//    modes for the LCD panel.
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

#ifndef MATRIX_ORBITAL_SCREEN_HXX
#define MATRIX_ORBITAL_SCREEN_HXX 1

#include <matrix-orbital/export.h>
#include <matrix-orbital/controller.hxx>

// [abstract class] LcdScreen
//
//   The abstract base class for all LCD mode-screens. Subclasses
//   implement the do_draw() method to perform the drawing on the LCD
//   screen.

class MATRIX_ORBITAL_DLL LcdScreen 
{
  public:
    typedef MatrixOrbitalController lcd_type;

    LcdScreen(lcd_type &lcd,
	      unsigned long display_time = 5000,
	      unsigned long diplay_interval = 500);
    virtual ~LcdScreen() {}

    // draw()
    //   Draw the screen once. This calls the pure virtual do_draw()
    //   which performs the actual drawing and must be implemented by
    //   each subclass.
    virtual void draw();

    // cycle()
    //   Perform one "cycle" of drawing - looping and drawing every
    //   _display_interval milliseconds until _display_time is up.
    virtual void cycle();

    lcd_type& lcd() { return _lcd; }    
    void display_time(unsigned long time);
    unsigned long display_time() const;
    void display_interval(unsigned long interval);
    unsigned long display_interval() const;

  protected:
    lcd_type &_lcd;

    // do_draw()
    //   Subclasses must implement this method to do all the drawing
    //   using the _lcd member.    
    virtual void do_draw() = 0;

  private:
    unsigned long _display_time;
    unsigned long _display_interval;
};

#endif // MATRIX_ORBITAL_SCREEN_HXX
