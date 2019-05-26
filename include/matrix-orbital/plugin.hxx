//
// $Header: /Projects/lcd/include/matrix-orbital/plugin.hxx 5     6/09/99 12:59a Alpern $
//
//    File: <matrix-orbital/plugin.hxx>
//
//    Defines interfaces for registering new screens in an LcdMon
//    plugin.
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

#ifndef MATRIX_ORBITAL_PLUGIN_HXX
#define MATRIX_ORBITAL_PLUGIN_HXX 1

#include <matrix-orbital/export.h>
#include <iostream>

class MATRIX_ORBITAL_DLL LcdScreen;
class MATRIX_ORBITAL_DLL MatrixOrbitalController;

typedef LcdScreen* (*LcdFactoryFun)(MatrixOrbitalController &);

class MATRIX_ORBITAL_DLL LcdRegisterScreen
{
  public:
    LcdRegisterScreen(const char *screen_name, LcdFactoryFun factory_fn);
};

MATRIX_ORBITAL_DLL void mtxorb_load_plugins();
MATRIX_ORBITAL_DLL void mtxorb_report_on_plugins(std::ostream &os = std::cout);
MATRIX_ORBITAL_DLL LcdScreen* mtxorb_create_screen(MatrixOrbitalController &lcd, const char *name);

#endif // MATRIX_ORBITAL_PLUGIN_HXX
