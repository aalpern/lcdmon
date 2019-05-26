//
// $Header: /Projects/lcd/src/mtxorb/pdh-plugin.cxx 1     6/08/99 9:44a Alpern $
//
//    File: pdh-plugin.cxx
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

#include <matrix-orbital/pdh-screen.hxx>
#include <matrix-orbital/plugin.hxx>

// ----------------------------------------------------------------------
// Plugin factories
// ----------------------------------------------------------------------

#if 1
static LcdScreen* make_processor_screen(MatrixOrbitalController &lcd)
{
    return new ProcessorScreen(lcd);
}

static LcdScreen* make_multi_processor_screen(MatrixOrbitalController &lcd)
{
    return new MultiProcessorScreen(lcd);
}

static LcdScreen* make_cpu_mem_time_screen(MatrixOrbitalController &lcd)
{
    return new CpuMemoryTimeScreen(lcd);
}

static LcdScreen* make_cpu_mem_screen(MatrixOrbitalController &lcd)
{
    return new CpuMemoryScreen(lcd);
}

static LcdScreen* make_tcp_ip_screen(MatrixOrbitalController &lcd)
{
    return new TcpConnectionScreen(lcd);
}

static LcdRegisterScreen r_cpu("Processor", make_processor_screen);
static LcdRegisterScreen r_mpu("MultiProcessor", make_multi_processor_screen);
static LcdRegisterScreen r_tcp("TCP/IP", make_tcp_ip_screen);
static LcdRegisterScreen r_mem("CPU/Memory", make_cpu_mem_screen);
static LcdRegisterScreen r_cmt("CPU/Memory/Time", make_cpu_mem_time_screen);
#endif
