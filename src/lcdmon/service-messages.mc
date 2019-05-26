;//  
;//  $Header: $
;// 
;//     File: service-messages.mc
;// 
;//     Messages for NT event log support for LCD Monitor.
;// 
;//     Copyright (C) 1998,1999 Adam Alpern
;//     Email: alpern@brightware.com
;// 
;//  This library is free software; you can redistribute it and/or
;//  modify it under the terms of the GNU General Public License as
;//  published by the Free Software Foundation; either version 2 of the
;//  License, or (at your option) any later version.
;// 
;//  This library is distributed in the hope that it will be useful, but
;//  WITHOUT ANY WARRANTY; without even the implied warranty of
;//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
;//  Library General Public License for more details.
;// 
;//  You should have received a copy of the GNU General Public License
;//  along with this library; if not, write to the Free Foundation,
;//  Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
;// 
;
;#ifndef LCDMON_SERVICE_MESSAGES_MC
;#define LCDMON_SERVICE_MESSAGES_MC 1
;

SeverityNames=(
	Note=0x0:APP_SEVERITY_NOTE
	Informational=0x1:APP_SEVERITY_INFORMATIONAL
	Warning=0x2:APP_SEVERITY_WARNING
	Error=0x3:APP_SEVERITY_ERROR
)

MessageId= 
Severity=Note
SymbolicName=LCDMON_NOTE
Language=English
Note: %1
.
MessageId= 
Severity=Informational
SymbolicName=LCDMON_INFO
Language=English
Info: %1
.
MessageId= 
Severity=Warning
SymbolicName=LCDMON_WARN
Language=English
Warning: %1
.
MessageId= 
Severity=Error 
SymbolicName=LCDMON_ERROR
Language=English
Error: %1
.

;#endif // LCDMON_SERVICE_MESSAGES_MC
