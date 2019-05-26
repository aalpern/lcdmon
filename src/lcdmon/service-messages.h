//  
//  $Header: $
// 
//     File: service-messages.mc
// 
//     Messages for NT event log support for LCD Monitor.
// 
//     Copyright (C) 1998,1999 Adam Alpern
//     Email: alpern@brightware.com
// 
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License as
//  published by the Free Software Foundation; either version 2 of the
//  License, or (at your option) any later version.
// 
//  This library is distributed in the hope that it will be useful, but
//  WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Library General Public License for more details.
// 
//  You should have received a copy of the GNU General Public License
//  along with this library; if not, write to the Free Foundation,
//  Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
// 

#ifndef LCDMON_SERVICE_MESSAGES_MC
#define LCDMON_SERVICE_MESSAGES_MC 1

//
//  Values are 32 bit values layed out as follows:
//
//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +---+-+-+-----------------------+-------------------------------+
//  |Sev|C|R|     Facility          |               Code            |
//  +---+-+-+-----------------------+-------------------------------+
//
//  where
//
//      Sev - is the severity code
//
//          00 - Success
//          01 - Informational
//          10 - Warning
//          11 - Error
//
//      C - is the Customer code flag
//
//      R - is a reserved bit
//
//      Facility - is the facility code
//
//      Code - is the facility's status code
//
//
// Define the facility codes
//


//
// Define the severity codes
//
#define APP_SEVERITY_WARNING             0x2
#define APP_SEVERITY_NOTE                0x0
#define APP_SEVERITY_INFORMATIONAL       0x1
#define APP_SEVERITY_ERROR               0x3


//
// MessageId: LCDMON_NOTE
//
// MessageText:
//
//  Note: %1
//
#define LCDMON_NOTE                      0x00000001L

//
// MessageId: LCDMON_INFO
//
// MessageText:
//
//  Info: %1
//
#define LCDMON_INFO                      0x40000002L

//
// MessageId: LCDMON_WARN
//
// MessageText:
//
//  Warning: %1
//
#define LCDMON_WARN                      0x80000003L

//
// MessageId: LCDMON_ERROR
//
// MessageText:
//
//  Error: %1
//
#define LCDMON_ERROR                     0xC0000004L

#endif // LCDMON_SERVICE_MESSAGES_MC
