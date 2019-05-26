//
// $Header: $
//
//    File: <matrix-orbital/be-screen.hxx>
//
//    Screen classes for BeOS.
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

#ifndef MATRIX_ORBITAL_BE_SCREEN_HXX
#define MATRIX_ORBITAL_BE_SCREEN_HXX 1

#include <matrix-orbital/screen.hxx>

class BeScreen : public LcdScreen
{
  protected:
    typedef LcdScreen Inherited;

    BeScreen(lcd_type &lcd);
    
  public:
    virtual ~BeScreen();

    static const char *to_string(cpu_type cpu);
    static const char *to_string(platform_type platform);
};

class BePlatformScreen : public BeScreen
{
  public:
    typedef BeScreen Inherited;

    BePlatformScreen(lcd_type &lcd);
    virtual ~BePlatformScreen();

  protected:
    virtual void do_draw();
    system_info _initial_info;
    system_info _current_info;
};

#endif // MATRIX_ORBITAL_BE_SCREEN_HXX
