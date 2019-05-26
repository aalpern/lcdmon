//
// $Header: /Projects/lcd/src/mtxorb/c-interface.cxx 5     6/21/99 10:06a Alpern $
//
//    File: c-interface.cxx
//
//    Implementation of the C interface for the
//    MatrixOrbitalController class.
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
#include <matrix-orbital/c-interface.h>
#include <matrix-orbital/controller.hxx>

struct MtxOrb {
    MatrixOrbitalController *lcd;
};

static MtxOrb *allocate_lcd()
{
    MtxOrb *lcd = new MtxOrb;
    lcd->lcd = new MatrixOrbitalController();
    return lcd;
}

static MtxOrb *allocate_lcd(short width, short height)
{
    MtxOrb *lcd = new MtxOrb;
    lcd->lcd = new MatrixOrbitalController(width, height);
    return lcd;
}

static void deallocate_lcd(MtxOrb *&lcd)
{
    delete lcd->lcd;
    delete lcd;
    lcd = NULL;
}

#define NO_ARG_COMMAND_BODY(member_name, lcd) \
  try { \
      lcd->lcd->member_name(); \
  } catch ( ... ) { \
  }

#define NO_ARG_RETURN_COMMAND_BODY(member_name, lcd, error_value) \
  try { \
      return lcd->lcd->member_name(); \
  } catch ( ... ) { \
       return error_value; \
  }

#define ONE_ARG_COMMAND_BODY(member_name, lcd, arg) \
  try { \
      lcd->lcd->member_name(arg); \
  } catch ( ... ) { \
  }

#define TWO_ARG_COMMAND_BODY(member_name, lcd, arg1, arg2) \
  try { \
      lcd->lcd->member_name(arg1, arg2); \
  } catch ( ... ) { \
  }

#define THREE_ARG_COMMAND_BODY(member_name, lcd, arg1, arg2, arg3) \
  try { \
      lcd->lcd->member_name(arg1, arg2, arg3); \
  } catch ( ... ) { \
  }

#define FOUR_ARG_COMMAND_BODY(member_name, lcd, arg1, arg2, arg3, arg4) \
  try { \
      lcd->lcd->member_name(arg1, arg2, arg3, arg4); \
  } catch ( ... ) { \
  }

// ----------------------------------------------------------------------
// Open and shut
// ----------------------------------------------------------------------

MtxOrb*
mtxorb_open(char *device, short width, short height)
{
    try {
	MtxOrb *lcd = allocate_lcd(width, height);
	lcd->lcd->open(device);
	return lcd;
    } catch ( ... ) {
	return NULL;
    }
}

MtxOrb*
mtxorb_open_default(char *device)
{
    try {
	MtxOrb *lcd = allocate_lcd();
	lcd->lcd->open(device);
	return lcd;
    } catch ( ... ) {
	return NULL;
    }
}

void
mtxorb_close(MtxOrb *lcd)
{
    try {
	lcd->lcd->close();
	deallocate_lcd(lcd);
    } catch ( ... ) {
	return;
    }
}

void 
mtxorb_sleep(unsigned long millisec)
{
    MatrixOrbitalController::do_sleep(millisec);
}

// ----------------------------------------------------------------------
// Init
// ----------------------------------------------------------------------

void
mtxorb_init(MtxOrb *lcd)
{
    NO_ARG_COMMAND_BODY(init, lcd);
}

void
mtxorb_init_hbar(MtxOrb *lcd)
{
    NO_ARG_COMMAND_BODY(init_hbar, lcd);
}

void
mtxorb_init_vbar_thin(MtxOrb *lcd)
{
    NO_ARG_COMMAND_BODY(init_vbar_thin, lcd);
}

void
mtxorb_init_vbar_thick(MtxOrb *lcd)
{
    NO_ARG_COMMAND_BODY(init_vbar_thick, lcd);
}

void
mtxorb_init_large_digits(MtxOrb *lcd)
{
    NO_ARG_COMMAND_BODY(init_large_digits, lcd);
}

// ----------------------------------------------------------------------
// Settings
// ----------------------------------------------------------------------

void
mtxorb_contrast(MtxOrb *lcd, short value)
{
    ONE_ARG_COMMAND_BODY(contrast, lcd, value);
}

void
mtxorb_brightness(MtxOrb *lcd, short value)
{
    ONE_ARG_COMMAND_BODY(brightness, lcd, value);
}

void
mtxorb_backlight(MtxOrb *lcd, int on_or_off, short on_minutes)
{
    TWO_ARG_COMMAND_BODY(backlight, lcd, on_or_off, on_minutes);
}

void
mtxorb_autoscroll(MtxOrb *lcd, int on_or_off)
{
    ONE_ARG_COMMAND_BODY(autoscroll, lcd, on_or_off);
}

void
mtxorb_linewrap(MtxOrb *lcd, int on_or_off)
{
    ONE_ARG_COMMAND_BODY(linewrap, lcd, on_or_off);
}

void
mtxorb_blink(MtxOrb *lcd, int on_or_off)
{
    ONE_ARG_COMMAND_BODY(blink, lcd, on_or_off);
}

void
mtxorb_cursor(MtxOrb *lcd, int on_or_off)
{
    ONE_ARG_COMMAND_BODY(cursor, lcd, on_or_off);
}

void
mtxorb_display(MtxOrb *lcd, int on_or_off, short on_minutes)
{
    TWO_ARG_COMMAND_BODY(display, lcd, on_or_off, on_minutes);
}

// ----------------------------------------------------------------------
// Drawing
// ----------------------------------------------------------------------

void
mtxorb_top_left(MtxOrb *lcd)
{
    NO_ARG_COMMAND_BODY(top_left, lcd);
}

void
mtxorb_move_to(MtxOrb *lcd, short x, short y)
{
    TWO_ARG_COMMAND_BODY(move_to, lcd, x, y);
}

void
mtxorb_left(MtxOrb *lcd)
{
    NO_ARG_COMMAND_BODY(left, lcd);
}

void
mtxorb_right(MtxOrb *lcd)
{
    NO_ARG_COMMAND_BODY(right, lcd);
}

void
mtxorb_display_hbar(MtxOrb *lcd, short x, short y, short len, int reverse)
{
    FOUR_ARG_COMMAND_BODY(display_hbar, lcd, x, y, len, reverse);
}

void
mtxorb_display_vbar(MtxOrb *lcd, short x, short height)
{
    TWO_ARG_COMMAND_BODY(display_vbar, lcd, x, height);
}

void
mtxorb_display_large_digit(MtxOrb *lcd, short column, short n)
{
    TWO_ARG_COMMAND_BODY(display_large_digit, lcd, column, n);
}

void
mtxorb_create_custom_character(MtxOrb *lcd, short n, char *data)
{
    TWO_ARG_COMMAND_BODY(create_custom_character, lcd, n, data);
}

// ----------------------------------------------------------------------
// Drawing extensions
// ----------------------------------------------------------------------

void
mtxorb_display_string(MtxOrb *lcd, short x, short y, char *str)
{
    THREE_ARG_COMMAND_BODY(display_string, lcd, x, y, str);
}

void
mtxorb_display_title_banner(MtxOrb *lcd, char *str, short row)
{
    TWO_ARG_COMMAND_BODY(display_title_banner, lcd, str, row);
}

void
mtxorb_wipe_display_horiz(MtxOrb *lcd, short start_column, short end_column, 
		       unsigned long millisec_sleep)
{
    THREE_ARG_COMMAND_BODY(wipe_display_horiz, lcd, start_column, end_column, 
			   millisec_sleep);
}

void
mtxorb_wipe_display_vert(MtxOrb *lcd, short start_row, short end_row, 
		      unsigned long millisec_sleep)
{
    THREE_ARG_COMMAND_BODY(wipe_display_vert, lcd, start_row, end_row, 
			   millisec_sleep);
}

void
mtxorb_fill_column(MtxOrb *lcd, short column, char fill)
{
    TWO_ARG_COMMAND_BODY(fill_column, lcd, column, fill);
}

void
mtxorb_fill_row(MtxOrb *lcd, short row, char fill)
{
    TWO_ARG_COMMAND_BODY(fill_row, lcd, row, fill);
}

void 
mtxorb_fill_screen(MtxOrb *lcd, char fill)
{
    ONE_ARG_COMMAND_BODY(fill_screen, lcd, fill);
}

void 
mtxorb_frame_display(MtxOrb *lcd, char fill)
{
    ONE_ARG_COMMAND_BODY(frame_display, lcd, fill);
}

void 
mtxorb_sliding_curtains_open(MtxOrb *lcd, char fill, unsigned long millisec_sleep)
{
    TWO_ARG_COMMAND_BODY(sliding_curtains_open, lcd, fill, millisec_sleep);
}

void 
mtxorb_sliding_curtains_close(MtxOrb *lcd, char fill, int clear, unsigned long millisec_sleep)
{
    THREE_ARG_COMMAND_BODY(sliding_curtains_close, lcd, fill, clear, millisec_sleep);
}

// ----------------------------------------------------------------------
// Diagnostics
// ----------------------------------------------------------------------

mtxorb_status_t  
mtxorb_last_error(MtxOrb *lcd)
{
    NO_ARG_RETURN_COMMAND_BODY(last_error, lcd, 0);
}

char *
mtxorb_last_error_string(MtxOrb *lcd)
{
    try { 
	return const_cast<char *>(lcd->lcd->last_error_string());
    } catch ( ... ) { 
	return NULL; 
    }    
}

unsigned long 
mtxorb_bytes_written(MtxOrb *lcd)
{
    NO_ARG_RETURN_COMMAND_BODY(bytes_written, lcd, 0);
}
