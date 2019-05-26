//
// $Header: /Projects/lcd/src/mtxorb/plugin.cxx 6     6/15/99 9:37a Alpern $
//
// Copyright (C) 1998 Adam Alpern
// Email: alpern@brightware.com
//
//    File: plugin.cxx
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later
// version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Library General Public License for more details.
//
// You should have received a copy of the GNU Library General
// Public License along with this library; if not, write to the
// Free Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
// 02111-1307 USA
//

#define MATRIX_ORBITAL_EXPORT dllexport
#include <matrix-orbital/config.h>
#include <matrix-orbital/plugin.hxx>
#include <matrix-orbital/controller.hxx>
#include <matrix-orbital/screen.hxx>

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#if MATRIX_WINDOWS_PLATFORM
#include <windows.h>
#include <direct.h>
#elif MATRIX_UNIX_PLATFORM
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#elif MATRIX_BEOS_PLATFORM
#endif

#include <list>
#include <map>
#include <string>
#include <iostream>

#if MATRIX_WINDOWS_PLATFORM && _MSC_VER >= 1100
using namespace std;
#endif

#if MATRIX_WINDOWS_PLATFORM
typedef HMODULE library_handle;
#elif MATRIX_SOLARIS_PLATFORM
typedef void *library_handle;
#elif MATRIX_HPUX_PLATFORM
typedef shl_t library_handle;
#elif MATRIX_BEOS_PLATFORM
#else
typedef void *library_handle;
#endif

#if MATRIX_WINDOWS_PLATFORM
#define PATH_MAX _MAX_PATH
#define PATH_SEP '\\'
#elif MATRIX_UNIX_PLATFORM
#define PATH_MAX MAXPATHLEN
#define PATH_SEP '/'
#endif

#define MATRIX_PLUGIN_EXTENSION ".lcdx"
static string s_plugin_extension(MATRIX_PLUGIN_EXTENSION);

#if _DEBUG
#define MATRIX_PLUGIN_DIRNAME "plugins-debug"
#else
#define MATRIX_PLUGIN_DIRNAME "plugins"
#endif

typedef map< string, LcdFactoryFun > factory_container;
typedef list< library_handle >       library_handle_container;
typedef list< string >               string_list;

static factory_container        g_factories;
static string_list              g_plugin_names;
static library_handle_container g_libraries;

// + Local Prototypes +
int load_library(const char *pathname);
void close_library(library_handle lib);
void load_libraries(const char *directory, const char *extension);
void close_all_library_handles();
string get_current_working_directory();
string get_sibling_directory(const char *directory, const char *name);
// - Local Prototypes -

// ----------------------------------------------------------------------
// LcdRegisterScreen
// ----------------------------------------------------------------------

void mtxorb_report_on_plugins(ostream &os)
{
    os << "Loaded Plugins" << endl
       << "--------------" << endl;
    if ( g_plugin_names.size() <= 0 ) {
	os << "  [No plugins loaded]" << endl;
	return;
    }
    for ( string_list::const_iterator j = g_plugin_names.begin();
	  j != g_plugin_names.end(); j++)
    {
	os << "  " << *j << endl;
    }
    os << endl;

    os << "Registered LCD Screens" << endl
       << "----------------------" << endl;

    if ( g_factories.size() <= 0 ) {
	os << "  [No screens registered]" << endl;
	return;
    }

    for (factory_container::iterator i = g_factories.begin(); 
	 i != g_factories.end(); i++) 
    {
	os << "  " << (*i).first << endl;
    }
}

LcdRegisterScreen::LcdRegisterScreen(const char *screen_name, LcdFactoryFun factory_fn)
{
    g_factories.insert(factory_container::value_type(screen_name, factory_fn));
}

LcdScreen* mtxorb_create_screen(MatrixOrbitalController &lcd, const char *name)
{
    try {
	factory_container::const_iterator i = g_factories.find(name);
	if ( i == g_factories.end() )
	    return NULL;
	LcdFactoryFun fn = (*i).second;
	return (*fn)(lcd);
    } catch ( ... ) {
	return NULL;
    }
}

// ----------------------------------------------------------------------
// Dynamic Libraries
// ----------------------------------------------------------------------

static string get_plugin_directory()
{
    string plugin_path;
    const char *env_path = getenv("LCDMON_PLUGINS");

    if ( env_path ) {
	plugin_path = env_path;
    } else {
	plugin_path = get_sibling_directory(get_current_working_directory().c_str(), 
					    MATRIX_PLUGIN_DIRNAME);
    }    
    return plugin_path;
}

void set_current_working_directory(const char *path)
{
#if MATRIX_WINDOWS_PLATFORM
    SetCurrentDirectory(path);
#elif MATRIX_UNIX_PLATFORM
    setcwd(path)
#endif
}

string get_current_working_directory()
{
    char buf[PATH_MAX + 1];    
#if MATRIX_HPUX_PLATFORM || MATRIX_SOLARIS_PLATFORM
    getcwd(buf, PATH_MAX);
#elif MATRIX_UNIX_PLATFORM
    getcwd(buf);
#elif MATRIX_WINDOWS_PLATFORM
    _getcwd(buf, PATH_MAX);
#else
#error "No directory code for this platform"
#endif    
    int len = strlen(buf);
    // Make sure the path ends in a directory separator
    if ( (len > 0) && buf[len-1] != PATH_SEP ) {
	buf[len] = PATH_SEP;
	buf[++len] = '\0';
    }
    return string(buf);
}

static void ensure_trailing_separator(string &str)
{
    size_t pos = str.find_last_of(PATH_SEP);
    if ( pos != (str.length() - 1) ) {
	str += PATH_SEP;
    }
}

string get_sibling_directory(const char *dir, const char *name)
{
    string sib(dir);
    // back up the pathname to the previous separator
    size_t pos = sib.find_last_of(PATH_SEP);
    // if the last separator is the last char in the string, replace
    // it with a space and find again.
    if ( pos == (sib.length() - 1) ) {
	sib[pos] = ' ';
	pos = sib.find_last_of(PATH_SEP);
    }
    // truncate the string
    sib.resize(pos + 1);
    sib += name;
    ensure_trailing_separator(sib);
    return sib;
}

static bool ends_with(const string &str, const string &ext)
{
    return ( (str.length() >= ext.length()) 
	     && (str.substr(str.length() - 5) == ext) );
}

void load_libraries(const char *directory, const char *extension)
{
    // cerr << "load_libraries: " << directory << ", " << extension << endl;
    // we're assuming directory is ended by a separator
    int len = strlen(directory);
#if MATRIX_WINDOWS_PLATFORM
    WIN32_FIND_DATA data;
    char path[PATH_MAX];
    strncpy(path, directory, len);
    path[len++] = '*';
    path[len++] = '\0';
    HANDLE dir = FindFirstFile(path, &data);
    if ( dir == INVALID_HANDLE_VALUE )
	return;
    while ( 1 ) {
	string found_file_name = data.cFileName;
	if ( (found_file_name.length() > s_plugin_extension.length())
	     && ends_with(found_file_name, s_plugin_extension) )
	{
	    // cerr << "  loading " << found_file_name << endl;
	    load_library(found_file_name.c_str());
	}
	if ( !FindNextFile(dir, &data) )
	    break;
    }
#else
#error "No directory code for this platform"
#endif
}

static int last_error()
{
#if MATRIX_WINDOWS_PLATFORM
    return GetLastError();
#else
    return errno;
#endif
}

int load_library(const char *pathname)
{
    library_handle lib;

#if MATRIX_WINDOWS_PLATFORM
    string cwd = get_current_working_directory();
    string plugin_dir = get_plugin_directory();
    set_current_working_directory(plugin_dir.c_str());
    lib = LoadLibrary(pathname);
    set_current_working_directory(cwd.c_str());
#elif MATRIX_HPUX_PLATFORM
    lib = shl_load(pathname, BIND_IMMEDIATE, 0L);
#elif MATRIX_SOLARIS_PLATFORM
    lib = dlopen(pathname, RTLD_NOW);
#elif MATRIX_BEOS_PLATFORM
#else
#error "No dynamic loading code for this platform"
#endif
    
    // cerr << "Loading " << pathname << endl;

    if ( !lib ) {
	cerr << "Warning: failed to load " << pathname << endl
	     << "  " << strerror(last_error()) << endl;
	return last_error();
    }
    
    g_libraries.push_back(lib);
    g_plugin_names.push_back(pathname);
    return 0;
}

void close_library(library_handle lib)
{
#if MATRIX_WINDOWS_PLATFORM
    FreeLibrary(lib);
#elif MATRIX_SOLARIS_PLATFORM
    dlclose(lib);
#elif MATRIX_HPUX_PLATFORM
    shl_unload(lib)
#elif MATRIX_BEOS_PLATFORM
#else
#error "No dynamic loading code for this platform"
#endif
}

void close_all_library_handles()
{
    for (library_handle_container::iterator i = g_libraries.begin();
	 i != g_libraries.end(); i++) 
    {
	close_library(*i);
    }
}

class library_cleanup {
    library_cleanup() {}
    ~library_cleanup() {
	close_all_library_handles();
    }
    static library_cleanup instance;
};

void mtxorb_load_plugins()
{    
    load_libraries(get_plugin_directory().c_str(), MATRIX_PLUGIN_EXTENSION);
}
