//
// $Header: /Projects/lcd/src/lcdmon/main.cxx 7     6/15/99 9:38a Alpern $
//
//    File: main.cxx
//
//    Copyright (C) 1998,1999 Adam Alpern
//    Email: alpern@brightware.com
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 2 of the
// License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; if not, write to the Free Foundation,
// Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
//

#include <matrix-orbital/lcd.hxx>
#include <matrix-orbital/plugin.hxx>

#include <stdio.h>
#include <string.h>
#include <time.h>

#include <list>

#if MATRIX_WINDOWS_PLATFORM && _MSC_VER >= 110
using namespace std;
#endif

typedef list< LcdScreen* > screen_list;
static screen_list screens;

// About
// CPU/Memory
// CPU/Memory/Time
// MultiProcessor
// Processor
// TCP/IP

int 
main(int argc, char *argv[])
{
#if MATRIX_WINDOWS_PLATFORM
    const char *com = "\\\\.\\COM1";
#elif MATRIX_BEOS_PLATFORM
    const char *com = "serial1";
#endif
    MatrixOrbitalController lcd(20, 4);

    mtxorb_load_plugins();    
    mtxorb_report_on_plugins();

    LcdScreen *screen_about = mtxorb_create_screen(lcd, "About");
    screen_about->display_time(2000);

#if MATRIX_WINDOWS_PLATFORM
    LcdScreen *screen_cpus = mtxorb_create_screen(lcd, "MultiProcessor");
    LcdScreen *screen_cmt = mtxorb_create_screen(lcd, "CPU/Memory/Time");

    screens.push_back(screen_cmt);
    screens.push_back(screen_cpus);
#endif

    if (argc >= 2)
	com = argv[1];
    lcd.open(com);
    
    if ( lcd.error() ) {
	fprintf(stderr, "LCD error: %s\n", lcd.last_error_string());
	return -1;
    }    
    
    lcd.init();
    lcd.blink(false);
    lcd.init_hbar();
    lcd.init_vbar_thick();
    lcd.init_large_digits();

    screen_about->cycle();
    lcd.wipe_display_horiz(1, 20, 15);
    
    if ( screens.size() <= 0 )
	return -2;    
    
    screen_list::iterator i = screens.begin();    
    lcd.clear();
    lcd.top_left();

    while (1) 
    {
	(*i)->cycle();
	i++;
	if (i == screens.end())
	    i = screens.begin();
	lcd.clear();
    }
    
    lcd.close();
    return 0;
}
