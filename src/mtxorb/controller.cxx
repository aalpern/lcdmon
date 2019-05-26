//
// $Header: /Projects/lcd/src/mtxorb/controller.cxx 7     7/01/99 12:21p Alpern $
//
//    File: controller.cxx
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
#include <matrix-orbital/controller.hxx>
#include <stdio.h>
#include <string.h>
#include <iostream>

#if MATRIX_WINDOWS_PLATFORM && _MSC_VER >= 110
using namespace std;
#endif

MatrixOrbitalController::MatrixOrbitalController(short width, short height, type_t t, bool has_keypad)
    : _type(t), _width(width), _height(height), _has_keypad(has_keypad), _error(false), _status(0)
{
}

MatrixOrbitalController::~MatrixOrbitalController()
{
    close();
    free_error_buffer();
}

void
MatrixOrbitalController::free_error_buffer()
{
    if (_error_string_buffer) {
#if _WINDOWS
	::LocalFree(_error_string_buffer);
#else
	delete [] _error_string_buffer;
#endif
	_error_string_buffer = NULL;
    }
}

void 
MatrixOrbitalController::do_sleep(unsigned long millisec)
{
#if _WINDOWS
    ::Sleep(millisec);
#elif __BEOS__
    // BeOS measures sleep time in microseconds
    snooze(millisec * 1000);
#else
    sleep(millisec);
#endif
}

// ----------------------------------------------------------------------
// Low-level interface
// ----------------------------------------------------------------------

bool
MatrixOrbitalController::open(const char *device, unsigned long baud)
{
#if _WINDOWS
    _dev = ::CreateFile(device,        // lpFileName
			GENERIC_WRITE, // dwDesiredAccess
			0,             // dwSharedMode
			NULL,          // lpSecurityAttributes
			OPEN_EXISTING, // dwCreationDistribution
			0,             // not overlapped I/O
			NULL);         // hTemplate - NULL for comm devices
    if ( (_dev == INVALID_HANDLE_VALUE) || !_dev) {
	_error = true;
	_status = ::GetLastError();
	return false; 
    }
#if 1
    DCB dcb;
    dcb.DCBlength = sizeof(DCB);
    
    if (!GetCommState(_dev, &dcb)) {
	_status = GetLastError();
	_error = true;
	cerr << "Error in GetCommState(): " << _status << ", " << last_error_string() << endl;
	return false;
    }
    
    dcb.DCBlength = sizeof(DCB);
    dcb.BaudRate  = CBR_19200;
    dcb.fBinary   = 1;
    dcb.fParity   = 1;
    dcb.Parity    = 0;
    dcb.StopBits  = ONESTOPBIT; // value of ONESTOPBIT is 0
    dcb.ByteSize  = 8;
    
    if (!SetCommState(_dev, &dcb)) {
	_status = GetLastError();
	_error = true;
	cerr << "Error in SetCommState(): " << _status << ", " << last_error_string() << endl;
	return false;
    }

#endif
#elif __BEOS__
    _status = _port.Open(device);
    if ( status < 0 ) {
	_error = true;
	return false;
    }
#endif
    return true;
}

bool
MatrixOrbitalController::error() const
{
    return _error;
}

unsigned long
MatrixOrbitalController::bytes_written() const
{
    return _bytes_written;
}

MatrixOrbitalController::status_type
MatrixOrbitalController::last_error() const
{
    return _status;
}

char* 
MatrixOrbitalController::copy_string(const char *str)
{
    size_t len = strlen(str);
    char *cpy = new char[len+1];
    strcpy(cpy, str);
    return cpy;
}

const char* 
MatrixOrbitalController::last_error_string()
{
    free_error_buffer();
#if _WINDOWS
    ::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER 
		    | FORMAT_MESSAGE_FROM_SYSTEM,	
		    NULL,
		    _status,
		    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		    (LPTSTR) &_error_string_buffer,
		    0,
		    NULL);
#else
    _error_string_buffer = copy_string(strerror(_status));
#endif
    return static_cast<const char *>(_error_string_buffer);
}

void 
MatrixOrbitalController::close()
{
#if _WINDOWS
    ::CloseHandle(_dev);
#elif __BEOS__
    _port.Close();
#endif
}

void 
MatrixOrbitalController::init()
{
    clear();
    top_left();
    contrast(180);
    linewrap(true);
    blink(false);
    autoscroll(true);
}

bool 
MatrixOrbitalController::write(const char *buffer)
{
    size_t len = strlen(buffer);
    return write(buffer, len);
}

bool 
MatrixOrbitalController::write(const char *buffer, size_t n)
{
#if _WINDOWS
    BOOL result = ::WriteFile(_dev,
			      reinterpret_cast<LPCVOID>(buffer),
			      n,
			      &_bytes_written,
			      NULL);
    _error = result;
    _status = ::GetLastError();
    return result;
#elif __BEOS__
    ssize_t result = _port.Write(reinterpret_cast<const void *>(buffer), n);
    _error = ( result < 0 );
    if ( _error ) 
	_status = result;
    return _error;
#endif
}

void 
MatrixOrbitalController::send_char(char c)
{
    char buf[2];
    buf[0] = c;
    buf[1] = '\0';
    write(buf, 1);
}

void 
MatrixOrbitalController::send_command(char c)
{
    char buf[3];
    buf[0] = 0xFE;
    buf[1] = c;
    buf[2] = '\0';
    write(buf, 2);
}

void 
MatrixOrbitalController::send_command(char c, short param)
{
    char buf[4];
    buf[0] = 0xFE;
    buf[1] = c;
    buf[2] = param;
    buf[3] = '\0';
    write(buf, 3);
}

void 
MatrixOrbitalController::send_command(char c, short param1, short param2)
{
    char buf[5];
    buf[0] = 0xFE;
    buf[1] = c;
    buf[2] = param1;
    buf[3] = param2;
    buf[4] = '\0';
    write(buf, 4);
}

void 
MatrixOrbitalController::send_command(char c, short param1, short param2, short param3)
{
    char buf[6];
    buf[0] = 0xFE;
    buf[1] = c;
    buf[2] = param1;
    buf[3] = param2;
    buf[4] = param3;
    buf[5] = '\0';
    write(buf, 5);
}

void 
MatrixOrbitalController::send_command(char c, short param1, short param2, short param3, short param4)
{
    char buf[7];
    buf[0] = 0xFE;
    buf[1] = c;
    buf[2] = param1;
    buf[3] = param2;
    buf[4] = param3;
    buf[5] = param4;
    buf[6] = '\0';
    write(buf, 6);
}

// ----------------------------------------------------------------------
// Commands
// ----------------------------------------------------------------------

void 
MatrixOrbitalController::init_hbar()
{
    send_command('h');
}

void 
MatrixOrbitalController::init_vbar_thin()
{
    send_command('s');
}

void 
MatrixOrbitalController::init_vbar_thick()
{
    send_command('v');
}

void 
MatrixOrbitalController::init_large_digits()
{
    // if (_height < 4)
    // return;
    send_command('n');
}

// ----------------------------------------
// Settings
// ----------------------------------------

void 
MatrixOrbitalController::contrast(short value)
{
    if (_type != LCD) 
	return;
    send_command('P', value);
}

void 
MatrixOrbitalController::brightness(short value)
{
    if (_type != VFD) 
	return;
    send_command('Y', value);
}

void 
MatrixOrbitalController::display(bool value, short time)
{
    if (_type != VFD) 
	return;
    if (!value)
	send_command('F');
    else
	send_command('B', time);
}

void 
MatrixOrbitalController::backlight(bool value, short time)
{
    if (!value)
	send_command('F');
    else
	send_command('B', time);
}

void 
MatrixOrbitalController::autoscroll(bool value)
{
    send_command((value ? 'Q' : 'R'));
}

void 
MatrixOrbitalController::linewrap(bool value)
{
    send_command((value ? 'C' : 'D'));
} 

void 
MatrixOrbitalController::blink(bool value)
{
    send_command((value ? 'S' : 'T'));
} 

void 
MatrixOrbitalController::cursor(bool value)
{
    send_command((value ? 'J' : 'K'));
}

// ----------------------------------------
// Drawing
// ----------------------------------------

void 
MatrixOrbitalController::clear()
{
    send_command('X');
}

void 
MatrixOrbitalController::top_left()
{
    send_command('H');
}

void 
MatrixOrbitalController::left(size_t n)
{
    for (size_t i = 0; i < n; i++)
	send_command('L');
}

void 
MatrixOrbitalController::right(size_t n)
{
    for (size_t i = 0; i < n; i++)
	send_command('M');
}

void 
MatrixOrbitalController::move_to(short x, short y)
{
    send_command('G', x, y);
}

void 
MatrixOrbitalController::display_hbar(short x, short y, short len, short direction)
{
    send_command('|', x, y, direction, len);
}

void 
MatrixOrbitalController::display_vbar(short x, short height)
{
    send_command('=', x, height);
}

void 
MatrixOrbitalController::display_large_digit(short column, short n)
{
    if (_height < 4) 
	return;
    send_command('#', column, n);
}

void 
MatrixOrbitalController::create_custom_character(short n, const char *data)
{
    send_command('N', n);
    write(data);
}

// ----------------------------------------------------------------------
// Drawing Extensions
// ----------------------------------------------------------------------

void 
MatrixOrbitalController::display_string(short x, short y, const char *str)
{
    move_to(x, y);
    write(str);
}

void 
MatrixOrbitalController::display_title_banner(const char *str, short row)
{    
    char buf[40];
    size_t len = strlen(str);
    if (len > 17)
	len = 17;
    sprintf(buf, "\xFF\xFF %.*s \xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", (int)len, str);
    buf[20] = '\0';
    display_string(1, row, buf);
}

void 
MatrixOrbitalController::fill_column(short column, char fill)
{
    for (short i = 1; i <= _height; i++) {
	move_to(column, i);
	send_char(fill);
    }
}

void 
MatrixOrbitalController::fill_row(short row, char fill)
{
    for (short i = 1; i <= _width; i++) {
	move_to(i, row);
	send_char(fill);
    }
}

void
MatrixOrbitalController::fill_screen(char fill)
{
    for (short i = 1; i <= _height; i++)
	fill_row(i, fill);
}

void 
MatrixOrbitalController::frame_display(char fill)
{
    fill_column(1, fill);
    fill_column(_width, fill);
    fill_row(1, fill);
    fill_row(_height, fill);
}

void
MatrixOrbitalController::wipe_display_horiz(short start_column, short end_column, unsigned long millisec_sleep)
{    
    bool backwards = (end_column - start_column) < 0;
    short i;

    if (backwards) {
	for (i = start_column; i >= end_column; i--) {
	    if (i != start_column)
		fill_column(i + 1, ' ');
	    fill_column(i, 0xFF);
	    do_sleep(millisec_sleep);
	    if (i == end_column)
		fill_column(i, ' ');
	}
    } else {
	for (i = start_column; i <= end_column; i++) {
	    if (i != start_column) 
		fill_column(i - 1, ' ');
	    fill_column(i, 0xFF);
	    do_sleep(millisec_sleep);
	    if (i == end_column) 
		fill_column(i, ' ');	    
	}
    }
}

void 
MatrixOrbitalController::wipe_display_vert(short start_row, short end_row, unsigned long millisec_sleep)
{
    bool upwards = (end_row - start_row) < 0;
    short i;
    
    if (upwards) {
	for (i = start_row; i >= end_row; i--) {
	    if (i != start_row)
		fill_row(i + 1, ' ');
	    fill_row(i, 0xFF);
	    do_sleep(millisec_sleep);
	    if (i == end_row)
		fill_row(i, ' ');
	}
    } else {
	for (i = start_row; i <= end_row; i++) {
	    if (i != start_row)
		fill_row(i - 1, ' ');
	    fill_row(i, 0xFF);
	    do_sleep(millisec_sleep);
	    if (i == end_row)
		fill_row(i, ' ');
	}
    }
}

void 
MatrixOrbitalController::sliding_curtains_open(char fill, unsigned long millisec_sleep)
{
    fill_screen(fill);
    for (int i = (_width / 2); i > 0; i--) {
	fill_column(i, ' ');
	fill_column((_width + 1) - i, ' ');
	do_sleep(millisec_sleep);
    }
}

void 
MatrixOrbitalController::sliding_curtains_close(char fill, bool xclear, unsigned long millisec_sleep)
{
    if ( xclear )
	clear();
    for (int i = 1; i <= (_width / 2); i++) {
	fill_column(i, fill);
	fill_column((_width + 1) - i, fill);
	do_sleep(millisec_sleep);
    }
}
