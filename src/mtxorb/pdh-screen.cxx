//
// $Header: /Projects/lcd/src/mtxorb/pdh-screen.cxx 4     6/09/99 12:49a Alpern $
//
//    File: pdh-screen.cxx
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

#define MATRIX_ORBITAL_PDH_EXPORT dllexport
#include <matrix-orbital/pdh-screen.hxx>
#include <time.h>
#include <string.h>
#include <stdio.h>

// ----------------------------------------------------------------------
// PdhScreen
// ----------------------------------------------------------------------

unsigned long PdhScreen::s_pdh_query_count = 0;
unsigned long PdhScreen::s_pdh_counter_count = 0;

PdhScreen::PdhScreen(MatrixOrbitalController &lcd,
		     unsigned long display_time,
		     unsigned long display_interval)
    : Inherited(lcd, display_time, display_interval)
{
    pdh_open_query();
}

PdhScreen::~PdhScreen()
{
    pdh_close_query();
}

void
PdhScreen::cycle()
{
    _lcd.init_hbar();
    Inherited::cycle();
}

PDH_STATUS
PdhScreen::pdh_open_query()
{
    _pdh_status = ::PdhOpenQuery(NULL, 
				 (DWORD)s_pdh_query_count++, 
				 &_pdh_query);
    return _pdh_status;
}

PDH_STATUS
PdhScreen::pdh_close_query()
{
    _pdh_status = ::PdhCloseQuery(_pdh_query);
    return _pdh_status;
}

PDH_STATUS
PdhScreen::pdh_add_counter(const char *path, HCOUNTER *counter)
{
    _pdh_status = ::PdhAddCounter(_pdh_query, path,
				  (DWORD)s_pdh_counter_count++,
				  counter);
    return _pdh_status;
}

PDH_STATUS
PdhScreen::pdh_collect_data()
{
    _pdh_status = ::PdhCollectQueryData(_pdh_query);
    return _pdh_status;
}

PDH_STATUS
PdhScreen::status() const
{
    return _pdh_status;
}

long
PdhScreen::pdh_long_value(HCOUNTER counter)
{
    PDH_FMT_COUNTERVALUE value;
    PDH_STATUS status = ::PdhGetFormattedCounterValue(counter,
						PDH_FMT_LONG,
						NULL,
						&value);
    return value.longValue;
}

double
PdhScreen::pdh_double_value(HCOUNTER counter)
{   
    PDH_FMT_COUNTERVALUE value;
    PDH_STATUS status = ::PdhGetFormattedCounterValue(counter,
					    PDH_FMT_DOUBLE,
					    NULL,
					    &value);
    return value.doubleValue;
}

LONGLONG 
PdhScreen::pdh_large_value(HCOUNTER counter)
{
    PDH_FMT_COUNTERVALUE value;
    PDH_STATUS status = ::PdhGetFormattedCounterValue(counter,
						      PDH_FMT_LARGE,
						      NULL,
						      &value);
    return value.largeValue;
}

// ----------------------------------------------------------------------
// CpuMemoryScreen
// ----------------------------------------------------------------------

CpuMemoryScreen::CpuMemoryScreen(MatrixOrbitalController &lcd)
    : Inherited(lcd)
{
    pdh_add_counter("\\Processor(0)\\% Processor Time", &_processor_counter);
    pdh_add_counter("\\Memory\\% Committed Bytes In Use", &_memory_counter);
    _lcd.init_hbar();
}

CpuMemoryScreen::~CpuMemoryScreen()
{
}

void 
CpuMemoryScreen::do_draw()
{    
    pdh_collect_data();
    _lcd.top_left();

    _lcd.display_string(1, 1, "0%   CPU Usage  100%");
    _lcd.display_string(1, 4, "0% Memory Usage 100%");
    _lcd.display_hbar(1, 2, processor_percentage());
    _lcd.display_hbar(1, 3, memory_percentage());
}

long 
CpuMemoryScreen::memory_percentage()
{
    return pdh_long_value(_memory_counter);
}

long 
CpuMemoryScreen::processor_percentage()
{
    return pdh_long_value(_processor_counter);
}

// ----------------------------------------------------------------------
// CpuMemoryTimeScreen
// ----------------------------------------------------------------------

CpuMemoryTimeScreen::CpuMemoryTimeScreen(MatrixOrbitalController &lcd)
    : CpuMemoryScreen(lcd)
{
}

CpuMemoryTimeScreen::~CpuMemoryTimeScreen()
{
}

void 
CpuMemoryTimeScreen::do_draw()
{
    pdh_collect_data();
    
    _lcd.display_string(1, 3, "CPU ");
    _lcd.display_string(1, 4, "RAM ");    
    _lcd.display_hbar(5, 3, processor_percentage() * 0.75);
    _lcd.display_hbar(5, 4, memory_percentage() * 0.75);

    char time_buf[20];
    char date_buf[20];
    size_t n;
    struct tm *t;
    time_t the_time;
    
    time(&the_time);
    t = localtime(&the_time);
    
    n = strftime(time_buf, 20, "    %I:%M:%S %p", t);
    n = strftime(date_buf, 20, "  %a, %b %d %Y", t);
    
    _lcd.display_string(1, 1, date_buf);
    _lcd.display_string(1, 2, time_buf);
}

// ----------------------------------------------------------------------
// ClockScreen
// ----------------------------------------------------------------------

ClockScreen::ClockScreen(MatrixOrbitalController &lcd)
    : Inherited(lcd)
{
}

ClockScreen::~ClockScreen()
{
}

void
ClockScreen::do_draw()
{
    struct tm *t;
    time_t the_time;
    
    time(&the_time);
    t = localtime(&the_time);
    
    _lcd.top_left();
    if (t->tm_hour < 10) {
	_lcd.display_large_digit(1, 0);
	_lcd.display_large_digit(5, t->tm_hour);
    } else {
	_lcd.display_large_digit(1, 1);
	_lcd.display_large_digit(5, t->tm_hour - 10);
    }

    if (t->tm_min < 10) {
	_lcd.display_large_digit(9, 0);
	_lcd.display_large_digit(13, t->tm_min);
    } else {
	_lcd.display_large_digit(9, t->tm_min / 10);
	_lcd.display_large_digit(13, t->tm_min % 10);
    }
}

// ----------------------------------------------------------------------
// ProcessorScreen
// ----------------------------------------------------------------------

ProcessorScreen::ProcessorScreen(MatrixOrbitalController &lcd)
    : Inherited(lcd)
{
    pdh_add_counter("\\Processor(0)\\% Processor Time", &_total_counter);
    pdh_add_counter("\\Processor(0)\\% User Time", &_user_counter);
    pdh_add_counter("\\Processor(0)\\% Privileged Time", &_priv_counter);
    pdh_add_counter("\\Processor(0)\\% Interrupt Time", &_int_counter);
}

ProcessorScreen::~ProcessorScreen()
{
}

void 
ProcessorScreen::do_draw()
{
    pdh_collect_data();

    _lcd.display_string(1, 1, "0%     Total    100%");
    _lcd.display_hbar(1, 2, pdh_long_value(_total_counter));
    _lcd.display_string(1, 3, "User   Priv.   Int. ");
    _lcd.display_hbar(1, 4, pdh_long_value(_user_counter) / 3);
    _lcd.display_hbar(7, 4, pdh_long_value(_priv_counter) / 3);
    _lcd.display_hbar(14, 4, pdh_long_value(_int_counter) / 3);    
}

// ----------------------------------------------------------------------
// MultiProcessorScreen
// ----------------------------------------------------------------------

MultiProcessorScreen::MultiProcessorScreen(MatrixOrbitalController &lcd)
    : Inherited(lcd), _n_cpus(0), _total_counter1(NULL),
      _total_counter2(NULL), _total_counter3(NULL), _total_counter4(NULL)
{
    if ( PdhValidatePath("\\Processor(0)\\% Processor Time") == ERROR_SUCCESS ) {
	pdh_add_counter("\\Processor(0)\\% Processor Time", &_total_counter1);
	++_n_cpus;
    }
    if ( PdhValidatePath("\\Processor(1)\\% Processor Time") == ERROR_SUCCESS ) {
	pdh_add_counter("\\Processor(1)\\% Processor Time", &_total_counter2);
	++_n_cpus;
    }
    if ( PdhValidatePath("\\Processor(2)\\% Processor Time") == ERROR_SUCCESS ) {
	pdh_add_counter("\\Processor(2)\\% Processor Time", &_total_counter3);
	++_n_cpus;
    }
    if ( PdhValidatePath("\\Processor(3)\\% Processor Time") == ERROR_SUCCESS ) {
	pdh_add_counter("\\Processor(3)\\% Processor Time", &_total_counter4);
	++_n_cpus;
    }
}

MultiProcessorScreen::MultiProcessorScreen(MatrixOrbitalController &lcd, unsigned short n_cpus)
    : Inherited(lcd), _n_cpus(n_cpus), _total_counter1(NULL),
      _total_counter2(NULL), _total_counter3(NULL), _total_counter4(NULL)
{
    switch ( n_cpus ) {
      case 4:
	  pdh_add_counter("\\Processor(3)\\% Processor Time", &_total_counter4);
      case 3:
	  pdh_add_counter("\\Processor(0)\\% Processor Time", &_total_counter1);
      case 2:
	  pdh_add_counter("\\Processor(1)\\% Processor Time", &_total_counter2);
      case 1:
	  pdh_add_counter("\\Processor(2)\\% Processor Time", &_total_counter3);
	  break;
    }
}

MultiProcessorScreen::~MultiProcessorScreen()
{
}

void
MultiProcessorScreen::do_draw()
{
    pdh_collect_data();

    switch (_n_cpus) {
      case 1:
	  _lcd.display_string(1, 1, "0%     CPU 1    100%");
	  _lcd.display_hbar(1, 2, pdh_long_value(_total_counter1));
	  break;
      case 2:
	  _lcd.display_string(1, 1, "0%     CPU 1    100%");
	  _lcd.display_hbar(1, 2, pdh_long_value(_total_counter1));
	  _lcd.display_string(1, 3, "0%     CPU 2    100%");
	  _lcd.display_hbar(1, 4, pdh_long_value(_total_counter2));
	  break;
      case 3:
	  _lcd.display_string(1, 1, "0%     CPU 1    100%");
	  _lcd.display_hbar(1, 2, pdh_long_value(_total_counter1));
	  _lcd.display_string(1, 3, "  CPU 2      CPU 3  ");
	  _lcd.display_hbar(1, 4, pdh_long_value(_total_counter2) / 2);
	  _lcd.display_hbar(10, 4, pdh_long_value(_total_counter3) / 2);
      case 4:
	  _lcd.display_string(1, 1, "  CPU 1      CPU 2  ");
	  _lcd.display_hbar(1, 2, pdh_long_value(_total_counter1) / 2);
	  _lcd.display_hbar(10, 2, pdh_long_value(_total_counter2) / 2);
	  _lcd.display_string(1, 3, "  CPU 3      CPU 4  ");
	  _lcd.display_hbar(1, 4, pdh_long_value(_total_counter3) / 2);
	  _lcd.display_hbar(10, 4, pdh_long_value(_total_counter4) / 2);
	  break;
    }
}

// ----------------------------------------------------------------------
// TcpScreen
// ----------------------------------------------------------------------

TcpConnectionScreen::TcpConnectionScreen(MatrixOrbitalController &lcd)
    : Inherited(lcd)
{
    pdh_add_counter("\\TCP\\Segments Received/sec", &_segments_received);
    pdh_add_counter("\\TCP\\Segments Sent/sec",   &_segments_sent);
    pdh_add_counter("\\TCP\\Connections Active",  &_active_connections);
    pdh_add_counter("\\TCP\\Connections Passive", &_passive_connections);
    pdh_add_counter("\\TCP\\Connection Failures", &_connection_failures);
    pdh_add_counter("\\TCP\\Connections Reset",   &_connection_resets);
}

TcpConnectionScreen::~TcpConnectionScreen()
{
}

void
TcpConnectionScreen::do_draw()
{
    pdh_collect_data();
    
    _lcd.display_title_banner("TCP STATS");
    char buf[24];
    
    sprintf(buf, "Actv: %.33d Pasv: %.33d", 
	    pdh_long_value(_active_connections),
	    pdh_long_value(_passive_connections));
    _lcd.display_string(2, 1, buf);

    sprintf(buf, "Fail: %.33d Rset: %.33d", 
	    pdh_long_value(_connection_failures),
	    pdh_long_value(_connection_resets));
    _lcd.display_string(3, 1, buf);

    sprintf(buf, "Sent: %.33d Recv: %.33d", 
	    pdh_long_value(_segments_sent),
	    pdh_long_value(_segments_received));
    _lcd.display_string(4, 1, buf);   
}
