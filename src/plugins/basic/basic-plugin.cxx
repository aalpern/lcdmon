//
// $Header: /Projects/lcd/src/plugins/basic/basic-plugin.cxx 2     6/15/99 9:34a Alpern $
//
//    File: basic-plugin.cxx
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

#include <matrix-orbital/screen.hxx>
#include <matrix-orbital/version.h>
#include <matrix-orbital/plugin.hxx>

#include <stdio.h>

// ----------------------------------------------------------------------
// AboutScreen
// ----------------------------------------------------------------------

class AboutScreen : public LcdScreen
{
  public:
    typedef LcdScreen Inherited;

    AboutScreen(lcd_type &lcd);
    virtual ~AboutScreen();

  protected:
    virtual void do_draw();
};

AboutScreen::AboutScreen(MatrixOrbitalController &lcd)
    : Inherited(lcd)
{
}

AboutScreen::~AboutScreen()
{
}

void
AboutScreen::do_draw()
{
    char buf[40];
    sprintf(buf, "LCDMon/NT %d.%d.%d",
	    MATRIX_ORBITAL_MAJOR_VERSION,
	    MATRIX_ORBITAL_MINOR_VERSION,
	    MATRIX_ORBITAL_REVISION);
    _lcd.display_title_banner(buf);
    _lcd.display_string(4, 2, "by Adam Alpern");
}

static LcdScreen* 
make_about_screen(MatrixOrbitalController &lcd)
{
    return new AboutScreen(lcd);
}

static LcdRegisterScreen r_about("About", make_about_screen); 
