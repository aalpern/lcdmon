//
// $Header: /Projects/lcd/include/matrix-orbital/controller.hxx 3     6/21/99 10:00a Alpern $
//
//    File: <matrix-orbital/controller.hxx>
//
//    A controller class for the Matrix Orbital series of LCD/VFD
//    displays. See http://www.matrix-orbital.com/. They're cool!
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

#ifndef MATRIX_ORBITAL_HXX
#define MATRIX_ORBITAL_HXX 1

#include <matrix-orbital/export.h>
#include <matrix-orbital/bool.h>

#if MATRIX_WINDOWS_PLATFORM
#include <windows.h>
#elif MATRIX_BEOS_PLATFORM
#include <be/device/SerialPort.h>
#endif

#if MATRIX_BEOS_PLATFORM
#define MATRIX_DEFAULT_DEVICE "serial1"
#elif MATRIX_WINDOWS_PLATFORM
#define MATRIX_DEFAULT_DEVICE "\\\\.\\COM1"
#elif MATRIX_LINUX_PLATFORM
#define MATRIX_DEFAULT_DEVICE "/dev/lcd"
#endif

// [class] MatrixOrbitalController
//
// The basic controller for a serial-port LCD display. Knows how to
// open the port, send data, and wraps the entire command set in handy
// functions (so you don't need to remember that "\xFEX" means "clear
// the screen" - just call clear()).
//
// Supports the LCD and VFD screens in any size.

class MATRIX_ORBITAL_DLL MatrixOrbitalController
{
  public:
    enum type_t { LCD, VFD };

#if __BEOS__
    typedef status_t status_type;
#else
    typedef int status_type;
#endif

    MatrixOrbitalController(short width = 20, 
			    short height = 4,
			    type_t = LCD,
			    bool has_keypad = false);
    virtual ~MatrixOrbitalController();    

    // open()
    //   Open the device for the display. On a Windows system, this
    //   would be "\\\\.\\COM1", "\\\\.\\COM2", etc... On Linux it
    //   will be something like "/dev/lcd" or "/dev/cua0", etc...
    bool open(const char *device = MATRIX_DEFAULT_DEVICE, unsigned long baud = 19200);
    
    // close()
    //   Close the connection to the serial device.
    void close();

    // init()
    //   Initialize the LCD screen to a reasonable default
    //   state. Clears the screen and moves the cursor to the top
    //   left.
    void init();

    // -------------------------------------------------------------
    // Accessors
    // -------------------------------------------------------------

    type_t type() const   { return _type; }
    short  height() const { return _height; }
    short  width() const  { return _width; }

    // -------------------------------------------------------------
    // Command Set
    //
    //   The next set of functions directly wraps the Matrix Orbital
    //   BIOS command set.
    // -------------------------------------------------------------

    // init_ routines()
    //   Each of these sends one of the initialization codes to the
    //   module for the particular drawing features. Each need be
    //   called only once.
        
    void init_hbar();
    void init_vbar_thin();
    void init_vbar_thick();
    void init_large_digits();

    // clear()
    //   Clear the display. Wipe it clean.
    void clear();

    // contrast()    
    //   LCD only. Set the contrast of the display panel. 
    void contrast(short value);

    // brightness()    
    //   VFD only. Set the brightness of the display panel. Value can
    //   one of 0x00 (100%), 0x01 (75%), 0x02 (50%), or 0x03 (25%).	
    void brightness(short value);
    
    // backlight()    
    //   Turn the display backlight on or off. time is the number of
    //   minutes for the light to stay on. The maximum value is
    //   180. The default value of 0 means stay on forever (or until
    //   explicitly shut off).
    void backlight(bool value, short time = 0);
    
    // autoscroll()  
    //   Toggle the autoscrolling feature for text on or off.
    void autoscroll(bool value);

    // linewrap()
    //   Toggle the automatic line-wrapping feature for text on or
    //   off.
    void linewrap(bool value);

    // blink()
    //   Toggle cursor-blinking on or off.
    void blink(bool value);

    // cursor()
    //   Toggle display of the cursor on or off.
    void cursor(bool value);

    // display()
    //   VFD only. Turn the display on or off. VFD displays can get
    //   burn-in if left with the same image too long. If turning the
    //   display on, time is the number of minutes to leave it on
    //   for. The maximum value is 180, and the default value of 0
    //   leaves it on forever.
    void display(bool value, short time = 0); // VFD only

    // top_left()
    //   Reposition the cursor to the top left corner of the display.
    void top_left();

    // move_to()
    //   Move the cursor to a particular position (row and column) on
    //   the display.
    void move_to(short x, short y);

    // left()
    //   Move the current cursor position one space to the left.
    void left(size_t n = 1);

    // right() 
    //   Move the current cursor position one space to the right.
    void right(size_t n = 1);   

    // display_hbar()
    //   Display a horizontal bar at x,y on the screen. Length is in
    //   pixels - each character is 5 pixels wide, giving a 20-column
    //   display a maximum hbar size of 100. Direction is 0 for
    //   left-to-right (the default) or 1 for right-to-left,
    //
    //  Preconditions: you must call init_hbar() before calling this
    //   method.
    void display_hbar(short x, short y, short len, short direction = 0);
    
    // display_vbar()    
    //   Display a vertical bar at the specified column. Height is in
    //   pixels. Each character is 8 pixels high, giving a 4-line
    //   display a maximum vbar height of 32. Vertical bars always
    //   start from the bottom of the display.
    //
    //  Preconditions: you must call init_vbar_thick() or
    //   init_vbar_thin() before calling this method.
    void display_vbar(short x, short height);

    // display_large_digit()
    //
    //  Preconditions: You must call init_large_digits() before
    //   calling this method.
    void display_large_digit(short column, short n);
    
    // create_custom_character()
    //  Create a custom-character that will be stored in the
    //  Matrix-Orbital LCD unit's memory. data is a matrix of
    //  characters. Refer to the Matrix Orbital manual for the format
    //  of the data.
    void create_custom_character(short n, const char *data);

    // -------------------------------------------------------------
    // Drawing extensions
    //
    //   These functions all provide some higher-level drawing
    //   capability built on the command set.
    // -------------------------------------------------------------
    
    // display_string()
    //   Write a string at a location on the screen. Equivalent to
    //   calling: move_to(x, y); write(str);
    void display_string(short x, short y, const char *str);

    // display_title_banner()
    //   Display a "banner" at the specified row. The banner is
    //   preceded by 2 black squares and a space, leaving up to 17
    //   characters available for the title. Any trailing space will
    //   be filled in to complete the banner effect.
    //
    //   Example:
    //     lcd.display_title_banner("My Screen", 1);
    //   Output:
    //     "%% My Screen %%%%%%%"
    void display_title_banner(const char *str, short row = 1);

    // fill_column()
    // fill_row()
    // void fill_screen(()
    //   Fill an entire column, row, or the entire screen with a
    //   single character.
    void fill_column(short column, char fill = 0xFF);
    void fill_row(short row, char fill = 0xFF);
    void fill_screen(char fill = 0xFF);
    
    // frame_display()
    //   Draw a 1-character frame around the entire display.
    void frame_display(char fill = 0xFF);

    // wipe_display_horiz()
    //   Perform a horizontal "wipe" of the screen, leaving the wiped
    //   area clear. If start_column is less than end_column, the wipe
    //   will be from left to right. If it is greater it will be from
    //   right to left. The speed of the wipe can be controlled by the
    //   millisec_sleep parameter, which is the delay time between
    //   drawing each increment.
    void wipe_display_horiz(short start_column = 1, short end_column = 20, unsigned long millisec_sleep = 20);

    // wipe_display_vert()
    //   Perform a vertical "wipe" of the screen, leaving the wiped
    //   area clear. If start_row is less than end_row, the wipe
    //   will be from top to bottom. If it is greater it will be from
    //   bottom to top. The speed of the wipe can be controlled by the
    //   millisec_sleep parameter, which is the delay time between
    //   drawing each increment.
    void wipe_display_vert(short start_row = 1, short end_row = 4, unsigned long millisec_sleep = 20);

    // sliding_curtains_open()
    // sliding_curtains_close()
    //   Performs a theatre curtains-style transition effect, using
    //   the specified fill character (default to 0xFF, the solid
    //   character).
    void sliding_curtains_open(char fill = 0xFF, unsigned long millisec_sleep = 20);
    void sliding_curtains_close(char fill = 0xFF, bool clear = false, unsigned long millisec_sleep = 20);
    
    // ------------------------------    
    // Low-level interface 
    // ------------------------------

    bool write(const char *buffer, size_t n);
    bool write(const char *buffer);
    void send_char(char c);
    void send_command(char c);
    void send_command(char c, short param);
    void send_command(char c, short param1, short param2);
    void send_command(char c, short param1, short param2, 
		      short param3);
    void send_command(char c, short param1, short param2, 
		      short param3, short param4);

    // ------------------------------
    // Diagnostics
    // ------------------------------
    
    // error()
    //   Returns true if the last write operation failed, false
    //   otherwise;
    bool error() const;

    // last_error()
    //   Return the most recent error code returned by an operation.
    status_type last_error() const;
    
    // last_error_string()
    //   Return a descriptive message describing the last error that
    //   occured during an operation. This pointer is valid until the
    //   next call to last_error_string().
    const char *last_error_string();

    // bytes_written()
    //   Return the number of bytes written by the last write
    //   operation.
    unsigned long bytes_written() const;    

    static void do_sleep(unsigned long millisec);
    
  protected:
    void free_error_buffer();
    static char* copy_string(const char *str);

#if _WINDOWS
    HANDLE _dev;
    LPVOID _error_string_buffer;
#elif __BEOS__
    BSerialPort _port;
    char *_error_string_buffer;
#endif

    short  _height;
    short  _width;
    type_t   _type;
    bool   _has_keypad;
    status_type _status;

    unsigned long _bytes_written;
    bool   _error;
};

#endif // MATRIX_ORBITAL_HXX


