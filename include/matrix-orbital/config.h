/*
 * $Header: /Projects/lcd/include/matrix-orbital/config.h 2     6/08/99 9:47a Alpern $
 *
 *    File: <matrix-orbital/config.h>
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

#ifndef MATRIX_ORBITAL_CONFIG_H
#define MATRIX_ORBITAL_CONFIG_H 1

# if defined(__SVR4) && defined(__sun)
#  define MATRIX_SOLARIS_PLATFORM 1
# else
#  define MATRIX_SOLARIS_PLATFORM 0
# endif

# if defined(__hpux)
#  define MATRIX_HPUX_PLATFORM 1
# else
#  define MATRIX_HPUX_PLATFORM 0
# endif

# if defined(_WINDOWS) || defined(_WIN32)
#  define MATRIX_WINDOWS_PLATFORM 1
# else
#  define MATRIX_WINDOWS_PLATFORM 0
# endif

# if defined(__BeOS__)
#  define MATRIX_BEOS_PLATFORM 1
# else
#  define MATRIX_BEOS_PLATFORM 0
# endif

# if MATRIX_HPUX_PLATFORM || MATRIX_SOLARIS_PLATFORM
#  define MATRIX_UNIX_PLATFORM 1
# else
#  define MATRIX_UNIX_PLATFORM 0
# endif

#endif /* MATRIX_ORBITAL_CONFIG_H */
