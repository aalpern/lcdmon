//
// $Header: /Projects/lcd/src/mtxorb/screen.cxx 3     6/08/99 9:44a Alpern $
//
//    File: screen.cxx
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

#define MATRIX_ORBITAL_EXPORT dllexport
#include <matrix-orbital/screen.hxx>
#include <matrix-orbital/version.h>
#include <matrix-orbital/plugin.hxx>

#include <stdio.h>

// ----------------------------------------------------------------------
// LcdScreen
// ----------------------------------------------------------------------

LcdScreen::LcdScreen(MatrixOrbitalController &lcd,
		     unsigned long display_time,
		     unsigned long display_interval)
    : _lcd(lcd), 
      _display_time(display_time),
      _display_interval(display_interval)
{
}

void 
LcdScreen::draw()
{
    do_draw();
}

void 
LcdScreen::cycle()
{
    unsigned long t = 0;
    while (t < _display_time) 
    {
	draw();
	MatrixOrbitalController::do_sleep(_display_interval);
	t += _display_interval;
    }
}

void 
LcdScreen::display_time(unsigned long value)
{
    _display_time = value;
}

unsigned long
LcdScreen::display_time() const
{
    return _display_time;
}

void 
LcdScreen::display_interval(unsigned long value)
{
    _display_interval = value;
}

unsigned long 
LcdScreen::display_interval() const
{
    return _display_interval;
}

