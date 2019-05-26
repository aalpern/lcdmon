/*
 * $Header: /Projects/lcd/include/matrix-orbital/export.h 3     6/08/99 12:02a Alpern $
 *
 *    File: <matrix-orbital/export.hxx>
 *
 *    Copyright (C) 1998,1999 Adam Alpern
 *    Email: alpern@brightware.com
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public License
 * as published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA
 */

#include <matrix-orbital/config.h>

#ifndef MATRIX_ORBITAL_EXPORT_HXX
#define MATRIX_ORBITAL_EXPORT_HXX 1

#ifndef MATRIX_ORBITAL_EXPORT
#define MATRIX_ORBITAL_EXPORT     dllimport
#endif

#ifndef MATRIX_ORBITAL_PDH_EXPORT
#define MATRIX_ORBITAL_PDH_EXPORT dllimport
#endif

#ifndef MATRIX_ORBITAL_BE_EXPORT
#define MATRIX_ORBITAL_BE_EXPORT  dllimport
#endif

/*
 * MtxOrb.dll
 *  - All platforms
 */
#ifndef MATRIX_ORBITAL_DLL
#if _MSC_VER || __BEOS__
#define MATRIX_ORBITAL_DLL __declspec ( MATRIX_ORBITAL_EXPORT )
#else
#define MATRIX_ORBITAL_DLL
#endif
#endif /* MATRIX_ORBITAL_DLL */

/*
 * MtxOrbPdh.dll
 *  - Windows NT only
 */
#ifndef MATRIX_ORBITAL_PDH_DLL
#if _MSC_VER || __BEOS__
#define MATRIX_ORBITAL_PDH_DLL __declspec ( MATRIX_ORBITAL_PDH_EXPORT )
#else
#define MATRIX_ORBITAL_PDH_DLL
#endif
#endif /* MATRIX_ORBITAL_PDH_DLL */

/*
 * MtxOrbBe.so
 *  - BeOS only
 */
#ifndef MATRIX_ORBITAL_BE_DLL
#if _MSC_VER || __BEOS__
#define MATRIX_ORBITAL_BE_DLL __declspec ( MATRIX_ORBITAL_BE_EXPORT )
#else
#define MATRIX_ORBITAL_BE_DLL
#endif
#endif /* MATRIX_ORBITAL_BE_DLL */

#if _MSC_VER
#pragma warning ( disable : 4786 )
#endif

#endif /* MATRIX_ORBITAL_EXPORT_HXX */
