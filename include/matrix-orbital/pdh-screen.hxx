//
// $Header: /Projects/lcd/include/matrix-orbital/pdh-screen.hxx 2     6/09/99 12:31a Alpern $
//
//    File: <matrix-orbital/pdh-screen.hxx>
//
//    A helper class which simplifies building new screens which use
//    the Performance Data Helper library from the Windows Platform
//    SDK.
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

#ifndef MATRIX_ORBITAL_PDH_SCREEN_HXX
#define MATRIX_ORBITAL_PDH_SCREEN_HXX 1

#include <matrix-orbital/screen.hxx>
#include "pdh.h"
#include "pdhmsg.h"

// [abstract class] PdhScreen
//
//    A helper class for Windows-NT performance-measuring LCD
//    modes. Uses the Performance Data Helper library from the Windows
//    Platform SDK. Subclasses must implement do_draw();

class MATRIX_ORBITAL_PDH_DLL PdhScreen : public LcdScreen
{
  public:
    typedef LcdScreen Inherited;

    PdhScreen(lcd_type &lcd,
	      unsigned long display_time = 5000,
	      unsigned long display_interval = 500);
    virtual ~PdhScreen();
    virtual void cycle();

    PDH_STATUS status() const;
    HQUERY query() const { return _pdh_query; }

    PDH_STATUS pdh_open_query();
    PDH_STATUS pdh_close_query();
    PDH_STATUS pdh_collect_data();
    PDH_STATUS pdh_add_counter(const char *path, HCOUNTER *counter);
    
    static long pdh_long_value(HCOUNTER counter);
    static double pdh_double_value(HCOUNTER counter);
    static LONGLONG pdh_large_value(HCOUNTER counter);

  private:
    static unsigned long s_pdh_query_count;
    static unsigned long s_pdh_counter_count;

    HQUERY     _pdh_query;
    PDH_STATUS _pdh_status;
};

// ______________________
// |0%   CPU Usage  100%|
// |%%%%%%%%%           |
// |%%%%                |
// |0M Memory Usage 128M|
// ----------------------

class MATRIX_ORBITAL_PDH_DLL CpuMemoryScreen : public PdhScreen
{
  public:
    typedef PdhScreen Inherited;

    CpuMemoryScreen(lcd_type &lcd);
    virtual ~CpuMemoryScreen();

    long memory_percentage();
    long processor_percentage();

  protected:    
    HCOUNTER _memory_counter;
    HCOUNTER _processor_counter;

    virtual void do_draw();
};

// ______________________
// |0%   Total CPU  100%|
// |%%%%%%%%%           |
// |User   Priv.   Int. |
// |%%%%   %%      %    |
// ----------------------

class MATRIX_ORBITAL_PDH_DLL ProcessorScreen : public PdhScreen
{
  public:
    typedef PdhScreen Inherited;

    ProcessorScreen(lcd_type &lcd);
    virtual ~ProcessorScreen();

  protected:
    virtual void do_draw();

    HCOUNTER _total_counter;
    HCOUNTER _user_counter;
    HCOUNTER _priv_counter;
    HCOUNTER _int_counter;
};

class MATRIX_ORBITAL_PDH_DLL MultiProcessorScreen : public PdhScreen
{
  public:
    typedef PdhScreen Inherited;

    MultiProcessorScreen(lcd_type &lcd);
    MultiProcessorScreen(lcd_type &lcd, unsigned short n_cpus);
    virtual ~MultiProcessorScreen();

  protected:
    virtual void do_draw();
    size_t _n_cpus;

    HCOUNTER _total_counter1;
    HCOUNTER _total_counter2;
    HCOUNTER _total_counter3;
    HCOUNTER _total_counter4;
};

// ______________________
// |  Wed, May 13 1998  |
// |    06:17:36 PM     |
// |CPU %%%%%%%%%       |
// |RAM %%%             |
// ----------------------

class MATRIX_ORBITAL_PDH_DLL CpuMemoryTimeScreen : public CpuMemoryScreen
{
  public:
    typedef CpuMemoryScreen Inherited;

    CpuMemoryTimeScreen(lcd_type &lcd);
    virtual ~CpuMemoryTimeScreen();

  protected:
    virtual void do_draw();
};

// ______________________
// |C: 2.0 GB capacity  |
// |   1.0 GB used (50%)|
// |D: 8.4 GB capacity  |
// |   2.1 GB used (25%)|
// ----------------------

class MATRIX_ORBITAL_PDH_DLL LogicalDiskScreen : public PdhScreen
{
  public:
  protected:
};


/* 
class MailboxScreen : public LcdScreen
{
  public:
    MailboxScreen(lcd_type &lcd);
    virtual ~MailboxScreen();
    
    void create_characters();
    void display_closed_icon(short x, short y);
    void display_open_icon(short x, short y);
}; 
*/

class MATRIX_ORBITAL_PDH_DLL ClockScreen : public PdhScreen
{
  public:
    typedef PdhScreen Inherited;

    ClockScreen(lcd_type &lcd);
    virtual ~ClockScreen();

  protected:
    virtual void do_draw();
};

// ______________________
// |%% TCP CONNECTIONS %|
// |Actv: 23  Pasv: 18  |
// |Sent: 108 Recv: 80  |
// |Dgrm: 274           |
// ----------------------

class MATRIX_ORBITAL_PDH_DLL TcpConnectionScreen : public PdhScreen
{
  public:
    typedef PdhScreen Inherited;

    TcpConnectionScreen(lcd_type &lcd);
    virtual ~TcpConnectionScreen();

  protected:
    virtual void do_draw();

    HCOUNTER _active_connections;
    HCOUNTER _passive_connections;
    HCOUNTER _connection_failures;
    HCOUNTER _connection_resets;
    HCOUNTER _segments_received;
    HCOUNTER _segments_sent;
};

#endif // MATRIX_ORBITAL_PDH_SCREEN_HXX
