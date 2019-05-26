/*
 * $Header: /Projects/lcd/src/lcdmon/c-test.c 3     6/07/99 9:18a Alpern $
 *
 *    File: c-test.c
 *
 *    Copyright (C) 1998,1999 Adam Alpern
 *    Email: alpern@brightware.com
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this library; if not, write to the Free Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include <matrix-orbital/c-interface.h>
#include <stdio.h>

int 
main(int argc, char *argv[])
{
    MtxOrb *lcd = mtxorb_open_default("\\\\.\\COM1");

    mtxorb_init(lcd);
    mtxorb_wipe_display_horiz(lcd, 1, 20, 15); 
     
    mtxorb_close(lcd);
    return 0;
}
