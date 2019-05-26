//
// $Header: $
//
//    File: <matrix-orbital/corba/corba.hxx>
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

#ifndef MATRIX_ORBITAL_CORBA_CORBA_HXX
#define MATRIX_ORBITAL_CORBA_CORBA_HXX 1

#if MATRIX_ORB_VISIBROKER
#include <corba.h>
#elif MATRIX_ORB_TAO
#error "TAO is not yet supported"
#elif MATRIX_ORB_OMNIBROKER
#error "OmniBroker is not yet supported"
#elif MATRIX_ORB_ORBACUS
#error "Orbacus is not yet supported"
#endif

#if MATRIX_ORB_VISIBROKER && !MATRIX_ORB_HAS_POA
#define CORBA_IMPLBASE(name) _sk_##name
#endif

#endif // MATRIX_ORBITAL_CORBA_CORBA_HXX
