//
// $Header: /Projects/lcd/src/lcdmon/demo.cxx 1     6/21/99 10:00a Alpern $
//
//    File: demo.cxx
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

// About
// CPU/Memory
// CPU/Memory/Time
// MultiProcessor
// Processor
// TCP/IP

int 
main(int argc, char *argv[])
{
    int banner_delay = 1250;
#if MATRIX_WINDOWS_PLATFORM
    const char *com = "\\\\.\\COM1";
#elif MATRIX_BEOS_PLATFORM
    const char *com = "serial1";
#endif
    MatrixOrbitalController lcd(20, 4);

    mtxorb_load_plugins();    
    mtxorb_report_on_plugins();

    if (argc >= 2)
	com = argv[1];
    lcd.open(com);
    
    if ( lcd.error() ) {
	fprintf(stderr, "LCD error: %s\n", lcd.last_error_string());
	return -1;
    }    

    lcd.init();
    lcd.clear();
    lcd.top_left();

    
    // ---------------------------------------
    lcd.display_title_banner("DEMO");
    lcd.display_string(1, 3, "Horizontal Wipe");
    MatrixOrbitalController::do_sleep(banner_delay);

    lcd.wipe_display_horiz(1, 20);
    lcd.wipe_display_horiz(20, 1);
    lcd.wipe_display_horiz(4, 14, 50);
    lcd.wipe_display_horiz(14, 4, 50);

    // ---------------------------------------
    lcd.display_title_banner("DEMO");
    lcd.display_string(2, 3, "Vertical Wipe");    
    MatrixOrbitalController::do_sleep(banner_delay);

    lcd.wipe_display_vert(1, 4);
    lcd.wipe_display_vert(4, 1);

    // ---------------------------------------
    int delay = 250;
    lcd.display_title_banner("DEMO");
    lcd.display_string(2, 3, "Fill Column/Row");
    MatrixOrbitalController::do_sleep(banner_delay);

    lcd.clear();
    lcd.fill_column(1, 'x');
    MatrixOrbitalController::do_sleep(delay);
    lcd.fill_column(3, 'y');
    MatrixOrbitalController::do_sleep(delay);
    lcd.fill_column(5, 'z');
    MatrixOrbitalController::do_sleep(delay);
    lcd.fill_column(7, '!');
    MatrixOrbitalController::do_sleep(delay);
    lcd.fill_column(2, 'a');
    MatrixOrbitalController::do_sleep(delay);
    lcd.fill_column(4, 'b');
    MatrixOrbitalController::do_sleep(delay);
    lcd.fill_column(6, 'c');
    MatrixOrbitalController::do_sleep(delay);
    lcd.fill_column(20, '%');
    MatrixOrbitalController::do_sleep(delay);
    lcd.fill_row(4, '-');
    MatrixOrbitalController::do_sleep(delay);
    lcd.fill_row(2, '*');
    MatrixOrbitalController::do_sleep(delay);

    MatrixOrbitalController::do_sleep(1000);

    // ---------------------------------------
    lcd.clear();
    lcd.display_title_banner("DEMO");
    lcd.display_string(2, 3, "Fill Screen");    
    MatrixOrbitalController::do_sleep(banner_delay);

    lcd.fill_screen();
    MatrixOrbitalController::do_sleep(500);
    lcd.fill_screen('a');
    MatrixOrbitalController::do_sleep(500);
    lcd.fill_screen('!');
    MatrixOrbitalController::do_sleep(500);

    // ---------------------------------------
    lcd.clear();
    lcd.display_title_banner("DEMO");
    lcd.display_string(1, 3, "Sliding Curtains");
    MatrixOrbitalController::do_sleep(banner_delay);

    lcd.sliding_curtains_close();
    MatrixOrbitalController::do_sleep(500);
    lcd.sliding_curtains_open();
    MatrixOrbitalController::do_sleep(500);
    lcd.sliding_curtains_close('|', false, 50);
    MatrixOrbitalController::do_sleep(500);
    lcd.sliding_curtains_open('|', 50);
    
    MatrixOrbitalController::do_sleep(banner_delay);

    // ---------------------------------------
    lcd.clear();
    lcd.display_title_banner("DEMO");
    lcd.display_string(1, 3, "Frame Display");
    MatrixOrbitalController::do_sleep(banner_delay);

    lcd.clear();
    lcd.frame_display();
    MatrixOrbitalController::do_sleep(500);
    lcd.frame_display('x');
    MatrixOrbitalController::do_sleep(500);
    lcd.frame_display('*');
    MatrixOrbitalController::do_sleep(500);
    lcd.frame_display('@');
    MatrixOrbitalController::do_sleep(500);
    lcd.frame_display('+');

    // ---------------------------------------
    MatrixOrbitalController::do_sleep(banner_delay);
    lcd.clear();
    lcd.top_left();
    lcd.write("\nThank you for trying LCD/Mon!\n");

    return 0;
}
