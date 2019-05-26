//
// $Header: /Projects/lcd/src/lcdmon/service.cxx 3     6/15/99 9:38a Alpern $
//
//    File: service.cxx
//
//    Implementation of an NT service version of LCDMon. A little
//    short on error handling at the moment, until it's working.
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
#include <windows.h>
#include <process.h>
#include <errno.h>
#include <stdio.h>
#include <stdarg.h>

#include <iostream>
#include <sstream>

#if _MSC_VER >= 1100
using namespace std;
#endif

// + Local Prototypes +
void WINAPI service_main(DWORD, LPSTR *);
void WINAPI service_control_handler(DWORD);
void usage(int argc, char *argv[]);
int  service_install();
int  service_remove();
int  service_start(int argc, char *argv[]);
int  service_stop();
int  service_run();
bool service_report_status(DWORD dwCurrentState, 
			   DWORD dwWin32ExitCode = NO_ERROR, 
			   DWORD dwWaitHint = 0);
void do_service_run(DWORD argc, LPSTR *argv);
void do_service_stop();
void setup_screens();
void event_log(const char *msg, WORD wType = EVENTLOG_INFORMATION_TYPE, DWORD event_id = 0);
void event_log(ostringstream &os, WORD wType = EVENTLOG_INFORMATION_TYPE, DWORD event_id = 0);
// - Local Prototypes -

static const int kNoError               = 0;
static const int kServiceError          = -100;
static const int kServiceManagerError   = -101;
static const int kServiceInstallError   = -102;
static const int kServiceCouldNotStop   = -103;
static const int kServiceCouldNotStart  = -103;
static const int kServiceCouldNotRemove = -103;

static SERVICE_STATUS g_status;
static SERVICE_STATUS_HANDLE g_status_handle;

const char * const g_event_registry_path = "SYSTEM\\CurrentControlSet\\Services\\EventLog\\Application\\LCD Monitor";
const char * const g_event_source_name = "LCD Monitor";
HANDLE g_event_log_handle = NULL;

// ----------------------------------------------------------------------
// Screens
// ----------------------------------------------------------------------

#include <list>

#if MATRIX_WINDOWS_PLATFORM && _MSC_VER >= 110
using namespace std;
#endif

typedef list< LcdScreen* > screen_list;
static screen_list screens;

// ----------------------------------------------------------------------
// NT Event Log
// ----------------------------------------------------------------------

ostream&
format_result(ostream &os, SCODE code)
{
    void *buf;
    FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER
		   | FORMAT_MESSAGE_FROM_SYSTEM,
		   NULL, code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		   (LPTSTR) &buf, 0, NULL );
    try { os << (LPTSTR)buf; }
    catch ( ... ) {}
    LocalFree(buf);
    return os;
}

bool
event_log_is_installed()
{
    HKEY key = NULL;
    if ( RegOpenKey(HKEY_LOCAL_MACHINE, g_event_registry_path, &key) != ERROR_SUCCESS )
	return false;
    RegCloseKey(key);
    return true;
}

bool
event_log_install()
{
    if ( event_log_is_installed() )
	return true;
    
    HKEY key = NULL;
    char filename[1024];
    DWORD dwMsgTypes = 
	EVENTLOG_ERROR_TYPE | EVENTLOG_WARNING_TYPE | EVENTLOG_INFORMATION_TYPE;
    if ( RegCreateKey(HKEY_LOCAL_MACHINE, g_event_registry_path, &key) != ERROR_SUCCESS )
	return false;

    GetModuleFileName(NULL, filename, sizeof(filename));

    RegSetValueEx(key, "EventMessageFile", 0, REG_SZ,
		  reinterpret_cast<CONST BYTE *>(filename),
		  sizeof(filename));
    RegSetValueEx(key, "CategoryMessageFile", 0, REG_SZ,
		  reinterpret_cast<CONST BYTE *>(filename),
		  sizeof(filename));
    RegSetValueEx(key, "TypesSupported", 0, REG_DWORD, 
		  reinterpret_cast<CONST BYTE *>(&dwMsgTypes),
		  sizeof(DWORD));

    RegCloseKey(key);
    return true;
}

bool 
event_log(ostringstream &os, WORD wType, DWORD event_id)
{
    return event_log(os.str().c_str(), wType, event_id);
}

bool 
event_log(const char *msg, WORD wType, DWORD event_id)
{
    const char *buf_ptr[] = { msg };
    
    event_log_install();

    if ( !g_event_log_handle )
	if ((g_event_log_handle = RegisterEventSource(NULL, g_event_source_name)) == NULL)
	    return false;
   
    return ReportEvent(event_source, wType, 0, event_id, NULL, 1, 0, buf_ptr, NULL);
}

// ----------------------------------------------------------------------
// Main
// ----------------------------------------------------------------------

int 
main(int argc, char *argv[])
{
    SERVICE_TABLE_ENTRY dispatchTable [] = {
	{ "LCDMON", service_main },
	{ NULL, NULL }
    };

    if (argc >= 2) {
	const char *cmd = argv[1];
	if (!_stricmp(argv[1], "-install"))
	    return service_install();
	else if (!_stricmp(argv[1], "-remove"))
	    return service_remove();
    }
    
    if (!StartServiceCtrlDispatcher(dispatchTable)) {
	int err = GetLastError();
	ostringstream str;
	str << "Error starting the service control dispatcher: ";
	format_result(str, err) << endl;
	event_log(str, EVENTLOG_ERROR_TYPE);
	return err;
    }

    return 0;
}

void 
usage(int argc, char *argv[])
{
    fprintf(stderr, "usage: %s <option>\n", argv[0]);
    fprintf(stderr, "  <option> may be one of\n");
    fprintf(stderr, "     -install  - Install the NT service\n");
    fprintf(stderr, "     -remove   - Remove the NT service\n");
}

// ----------------------------------------------------------------------
// Service manipulation
// ----------------------------------------------------------------------

static int 
service_open(SC_HANDLE &service_control_handle, SC_HANDLE &service_handle)
{
    service_control_handle = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if ( service_control_handle == 0 ) {
	cerr << "ERROR - Failed to open Service Control Manager: ";
	format_result(cerr, GetLastError()) << endl;
	return kServiceManagerError;
    }

    service_handle = OpenService(service_control_handle, "LCDMON", SERVICE_ALL_ACCESS);
    if ( service_handle == 0 ) {
	cerr << "ERROR - Failed to open service: ";
	format_result(cerr, GetLastError()) << endl;
	return kServiceError;
    }

    return kNoError;
}

int
service_install()
{
    char filename[FILENAME_MAX];
    SC_HANDLE service_control_handle;
    SC_HANDLE service_handle;
    int status = -1;

    GetModuleFileName(NULL, filename, FILENAME_MAX);
    service_control_handle = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if ( service_control_handle == 0 ) {
	cerr << "ERROR - Failed to open Service Control Manager: ";
	format_result(cerr, GetLastError()) << endl;
	return kServiceManagerError;
    }

    service_handle = CreateService(service_control_handle,
				   "LCDMON",
				   "LCD Monitor",
				   SERVICE_ALL_ACCESS,
				   SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS,
				   SERVICE_DEMAND_START,
				   SERVICE_ERROR_NORMAL,
				   filename,
				   NULL, NULL, "", NULL, NULL);
    if ( service_handle != 0 ) {
	fprintf(stdout, "Service installed");
	status = 0;
    } else {
	cerr << "ERROR - Failed to installed service: ";
	format_result(cerr, GetLastError()) << endl;
	status = kServiceInstallError;
    }
    CloseServiceHandle(service_control_handle);
    return status;
}

int 
service_remove()
{
    char filename[FILENAME_MAX];
    SC_HANDLE service_control_handle;
    SC_HANDLE service_handle;
    int status = -1;
    
    status = service_open(service_control_handle, service_handle);
    if ( !status ) {
	status = service_stop();
	if ( status == kServiceCouldNotStop ) {
	    fprintf(stderr, "Could not remove the service because it could not be stopped\n");
	    return kServiceCouldNotStop;
	}
    }

    if ( DeleteService(service_handle) == 0 ) {
	cerr << "ERROR - Could not remove the service: ";
	format_result(cerr, GetLastError()) << endl;
	return kServiceCouldNotRemove;
    }
    fprintf(stdout, "Service removed\n");

    return status;
}

bool 
service_report_status(DWORD dwCurrentState, 
		      DWORD dwWin32ExitCode, 
		      DWORD dwWaitHint)
{
    static DWORD s_check_point = 1;
    bool result = true;

    if ( dwCurrentState == SERVICE_START_PENDING )
	g_status.dwControlsAccepted = 0;
    else
	g_status.dwControlsAccepted = SERVICE_ACCEPT_STOP;

    g_status.dwCurrentState  = dwCurrentState;
    g_status.dwWin32ExitCode = dwWin32ExitCode;
    g_status.dwWaitHint      = dwWaitHint;

    switch ( dwCurrentState ) {
      case SERVICE_RUNNING:
      case SERVICE_STOPPED:
	  g_status.dwCheckPoint = 0;
	  break;
      default:
	  g_status.dwCheckPoint = s_check_point++;
    }

    result = SetServiceStatus(g_status_handle, &g_status);
    return result;
}

// ----------------------------------------------------------------------
// Service Main
// ----------------------------------------------------------------------

bool g_stop = false;

void 
do_service_stop()
{
    g_stop = true;
}

void 
do_service_run(DWORD argc, LPSTR *argv)
{
#if MATRIX_WINDOWS_PLATFORM
    const char *com = "\\\\.\\COM1";
#elif MATRIX_BEOS_PLATFORM
    const char *com = "serial1";
#endif
    MatrixOrbitalController lcd(20, 4);
    service_report_status(SERVICE_RUNNING);    

    mtxorb_load_plugins();

    LcdScreen *screen_about = mtxorb_create_screen(lcd, "About");
    if ( !screen_about ) {
	event_log("Failed to create LCD screen\n", EVENTLOG_ERROR_TYPE);
	return;
    }
    screen_about->display_time(2000);

#if _WINDOWS
    LcdScreen *screen_cpus = mtxorb_create_screen(lcd, "MultiProcessor");
    LcdScreen *screen_cmt = mtxorb_create_screen(lcd, "CPU/Memory/Time");

    screens.push_back(screen_cmt);
    screens.push_back(screen_cpus);
#endif

#if __BEOS__
#endif

    if (argc >= 2)
	com = argv[1];
    lcd.open(com);
    
    if ( lcd.error() ) {
	fprintf(stderr, "LCD error: %s\n", lcd.last_error_string());
	return;
    }    
    
    service_report_status(SERVICE_RUNNING);

    lcd.init();
    lcd.blink(false);
    lcd.init_hbar();
    lcd.init_large_digits();
    screen_about->cycle();
    lcd.wipe_display_horiz(1, 20, 15);
    
    if ( screens.size() <= 0 )
	return;    
    
    screen_list::iterator i = screens.begin();    
    lcd.clear();
    lcd.top_left();

    while (!g_stop) 
    {
	(*i)->cycle();
	i++;
	if (i == screens.end())
	    i = screens.begin();
	lcd.clear();
    }
    
    lcd.close();
}


void WINAPI
service_main(DWORD argc, LPSTR *argv)
{
    g_status_handle = RegisterServiceCtrlHandler("LCDMON", service_control_handler);
    
    if ( !g_status_handle ) {
	int err = GetLastError();
	ostringstream str;
	str << "Failed to register service control handler: ";
	format_result(str, err) << endl;
	event_log(str, EVENTLOG_ERROR_TYPE);
	return;
    }

    g_status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    g_status.dwServiceSpecificExitCode = 0;

    service_report_status(SERVICE_START_PENDING, NO_ERROR, 3000);

    do_service_run(argc, argv);
    
    if ( g_status_handle ) 
	service_report_status(SERVICE_STOPPED);

    return;
}

// ----------------------------------------------------------------------
// Service Control Handler
// ----------------------------------------------------------------------

void WINAPI 
service_control_handler(DWORD ctrl_code)
{
    switch ( ctrl_code ) {
      case SERVICE_CONTROL_STOP:
	  service_report_status(SERVICE_STOP_PENDING);
	  do_service_stop();
	  break;
    }
}
