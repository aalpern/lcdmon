
ABOUT
----------------------------------------------------------------------

This is a preliminary release. It's a little short on configuration,
but everything else works quite well, although NT service support is
not complete yet.

Coming soon is a configuration file to let you set up which screens
are displayed. Displays screens are extensible at runtime through a
very simple plugin mechanism (see below).

 o To run:

     This distribution includes pre-built binaries in the bin and
     plugins directories. First run the set-mode.bat batch file in the
     utils directory to set the right setting on the serial port.  Then cd
     to the bin directory and run lcdmon.exe. It will default to
     opening the LCD on COM1. If you have your Matrix Orbital display
     on, say, COM2, call it like this:
 
       lcdmon \\.\COM2
   
     That's all there is currently. Much more coming soon (more
     screens, configuration)

 o Building

     To build everything on Windows/NT, with Microsoft Visual C++, simply
     execute:

	 nmake -F Makefile.NT install MODE=release

     from the top of the distibution. This will create three directories -
     bin, lib, and plugins, containing the programs, link libraries, and
     plugins respectively. For a debug build, omit the MODE=release
     argument to nmake.

 o Plugins

     Look in the directory src/plugins for examples of building
     plugin for LCD/Mon that define new screen types. LCD/Mon comes
     with 2 plugins defined for Windows

     The plugin interface is not documented yet, but it's quite simple
     - the example plugins should get anyone with a bit of C++
     knowledge started. You can create new screens pretty much by
     cut-and-paste from the bundled plugins.
 
 o C interface
   
     The MtxOrb.dll interface is primarily C++, but there is a C API
     as well, strictly for talking directly to the LCD panel. You can use
     this if you're writing your own C applications that talk to the LCD
     panel. It can also be useful for use in scripting languages or other
     environments (such as Dylan) which have a Foreign Function Interface
     for C, but not C++.

     Defining LCD/Mon plugins and screen types in C is not supported,
     however - you still need C++ for that (but hey, it's easy!)


WHERE TO GET IT
----------------------------------------------------------------------

The latest release of LCD/Mon can always be obtained from:

	http://home.brightware.com/~alpern/lcd/


Information on LCDProc for Linux, which inspired this program, is
available at http://lcdproc.omnipotent.net/

Information about Matrix Orbital LCD displays, including how to obtain
them, is available at http://www.matrix-orbital.com/



CAVEATS
----------------------------------------------------------------------

The controller class doesn't set the serial port settings properly yet
on Windows NT.  Use the included utils/set-mode.bat script to do so.



PLATFORM SUPPORT
----------------------------------------------------------------------

This code has been compiled on the following platforms and compilers:

	MS Windows NT 4.0, Microsoft Visual C++ 4.2
	MS Windows NT 4.0, Microsoft Visual C++ 5.0
	MS Windows NT 4.0, Microsoft Visual C++ 6.0
	BeOS R4, gcc 2.7.2 (I think - whatever version came with BeOS R4)

Due to the nature of C++ implementations, binary code compiled by one
C++ compiler is *not* compatible with any other compiler (often even
different versions of the same compiler). Thank you very much
ANSI/ISO. For this reason you can't, say, compile plugins using GCC
for a version built with Visual C++. Therefore, I will be (time
willing) providing precompiled versions using both Visual C++ and GNU
C++.



TO DO
----------------------------------------------------------------------

- Provide support (source & makefiles) for compiling with the
  Cygnus GNU Win32 environment.

- Linux support

- Improved BeOS support

- Performance gathering for Windows 95/98. The PDH library is Windows
  NT only. (Note: I have no plans to do this myself, but will, of
  course, accept code from others and integrate it)

- Documentation

- Support for running as a Windows NT Service

- Abstractify the LCD controller class to allow supporting other
  modules, output methods

- Render LCD-style output to a GIF using the GD library



LICENSE
----------------------------------------------------------------------

The MtxOrb libraries are distributed under the GNU Public Library
License (LGPL). See the file COPYING.LIB for the full text of this
License.

The LCDMon program is distributed under the terms of the GNU Public
License (GPL). See the file COPYING for the full text of this License.

