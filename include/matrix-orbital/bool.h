/* 
 * $Header: $
 *
 *    File: <matrix-orbital/bool.h>
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

#ifndef MATRIX_ORBITAL_BOOL_H
#define MATRIX_ORBITAL_BOOL_H 1

#ifndef __BEOS__

#if _MSC_VER
#pragma warning(disable:4237)
#endif

#if _MSC_VER == 1020
#define MATRIX_ORBITAL_HAS_BOOL 0
#define MATRIX_ORBITAL_RENAME_BOOL 1
#else _MSC_VER == 1100
#define MATRIX_ORBITAL_HAS_BOOL 1
#define MATRIX_ORBITAL_RENAME_BOOL 0
#endif

#if !MATRIX_ORBITAL_HAS_BOOL
/* Some compilers (VC++ 4.2) don't implement builtin bool, but don't
 * let you use the symbol either! Grr. If your compiler has bool, just
 * define MATRIX_ORBITAL_HAS_BOOL=1
 */
#  if MATRIX_ORBITAL_RENAME_BOOL
#    define bool MATRIX_ORBITAL_bool
#  endif
#  if !defined(true)
typedef int bool;
#    define true 1
#    define false 0
#  endif
#endif

#endif /* __BEOS__ */

#endif /* MATRIX_ORBITAL_BOOL_H */
