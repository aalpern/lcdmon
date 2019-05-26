/*
 * $Header: /Projects/lcd/include/matrix-orbital/c-interface.h 7     6/21/99 10:06a Alpern $
 *
 *    File: <matrix-orbital/c-interface.h>
 *
 *    C interface for a controller class for the Matrix Orbital series
 *    of LCD/VFD displays. See http: *www.matrix-orbital.com/. They're
 *    cool!
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

#ifndef MATRIX_ORBITAL_C_INTERFACE_H
#define MATRIX_ORBITAL_C_INTERFACE_H 1

#include <matrix-orbital/export.h>

#ifdef __cplusplus
extern "C" {
#endif   
    
/* 
 * Like FILE in the C stdio package, MtxOrb is an opaque type - you
 * cannot look inside it, only pass it to the API functions.
 */
typedef struct MtxOrb MtxOrb;

#if __BEOS__
typedef status_t mtxorb_status_t;
#else
typedef int mtxorb_status_t;
#endif

/* Open and shut */
MATRIX_ORBITAL_DLL MtxOrb* mtxorb_open(char *device, short width, short height);
MATRIX_ORBITAL_DLL MtxOrb* mtxorb_open_default(char *device);
MATRIX_ORBITAL_DLL void mtxorb_close(MtxOrb *lcd);
MATRIX_ORBITAL_DLL void mtxorb_sleep(unsigned long millisec);

/* Init */
MATRIX_ORBITAL_DLL void mtxorb_init(MtxOrb *lcd);
MATRIX_ORBITAL_DLL void mtxorb_init_hbar(MtxOrb *lcd);
MATRIX_ORBITAL_DLL void mtxorb_init_vbar_thin(MtxOrb *lcd);
MATRIX_ORBITAL_DLL void mtxorb_init_vbar_thick(MtxOrb *lcd);
MATRIX_ORBITAL_DLL void mtxorb_init_large_digits(MtxOrb *lcd);

/* Settings */
MATRIX_ORBITAL_DLL void mtxorb_contrast(MtxOrb *lcd, short value);
MATRIX_ORBITAL_DLL void mtxorb_brightness(MtxOrb *lcd, short value);
MATRIX_ORBITAL_DLL void mtxorb_backlight(MtxOrb *lcd, int on_or_off, short on_minutes);
MATRIX_ORBITAL_DLL void mtxorb_autoscroll(MtxOrb *lcd, int on_or_off);
MATRIX_ORBITAL_DLL void mtxorb_linewrap(MtxOrb *lcd, int on_or_off);
MATRIX_ORBITAL_DLL void mtxorb_blink(MtxOrb *lcd, int on_or_off);
MATRIX_ORBITAL_DLL void mtxorb_cursor(MtxOrb *lcd, int on_or_off);
MATRIX_ORBITAL_DLL void mtxorb_display(MtxOrb *lcd, int on_or_off, short on_minutes); /* VFD only */

/* Drawing */
MATRIX_ORBITAL_DLL void mtxorb_top_left(MtxOrb *lcd);
MATRIX_ORBITAL_DLL void mtxorb_move_to(MtxOrb *lcd, short x, short y);
MATRIX_ORBITAL_DLL void mtxorb_left(MtxOrb *lcd);
MATRIX_ORBITAL_DLL void mtxorb_right(MtxOrb *lcd);
MATRIX_ORBITAL_DLL void mtxorb_display_hbar(MtxOrb *lcd, short x, short y, short len, int reverse);
MATRIX_ORBITAL_DLL void mtxorb_display_vbar(MtxOrb *lcd, short x, short height);
MATRIX_ORBITAL_DLL void mtxorb_display_large_digit(MtxOrb *lcd, short column, short n);
MATRIX_ORBITAL_DLL void mtxorb_create_custom_character(MtxOrb *lcd, short n, char *data);

/* Drawing extensions */
MATRIX_ORBITAL_DLL void mtxorb_display_string(MtxOrb *lcd, short x, short y, char *str);
MATRIX_ORBITAL_DLL void mtxorb_display_title_banner(MtxOrb *lcd, char *str, short row);
MATRIX_ORBITAL_DLL void mtxorb_wipe_display_horiz(MtxOrb *lcd, short start_column, short end_column, 
			    unsigned long millisec_sleep);
MATRIX_ORBITAL_DLL void mtxorb_wipe_display_vert(MtxOrb *lcd, short start_row, short end_row, 
			   unsigned long millisec_sleep);
MATRIX_ORBITAL_DLL void mtxorb_fill_column(MtxOrb *lcd, short column, char fill);
MATRIX_ORBITAL_DLL void mtxorb_fill_row(MtxOrb *lcd, short row, char fill);
MATRIX_ORBITAL_DLL void mtxorb_fill_screen(MtxOrb *lcd, char fill);
MATRIX_ORBITAL_DLL void mtxorb_frame_display(MtxOrb *lcd, char fill);
MATRIX_ORBITAL_DLL void mtxorb_sliding_curtains_open(MtxOrb *lcd, char fill, unsigned long millisec_sleep);
MATRIX_ORBITAL_DLL void mtxorb_sliding_curtains_close(MtxOrb *lcd, char fill, int clear, unsigned long millisec_sleep);

/* Diagnostics */
MATRIX_ORBITAL_DLL mtxorb_status_t mtxorb_last_error(MtxOrb *lcd);
MATRIX_ORBITAL_DLL char *        mtxorb_last_error_string(MtxOrb *lcd);
MATRIX_ORBITAL_DLL unsigned long mtxorb_bytes_written(MtxOrb *lcd);

#ifdef __cplusplus
}
#endif   

#endif /* MATRIX_ORBITAL_C_INTERFACE_H */
