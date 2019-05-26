//
// $Header: /Projects/lcd/src/mtxorb/be-screen.cxx 2     6/07/99 9:14a Alpern $
//
//    File: be-screen.cxx
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

#define MATRIX_ORBITAL_BE_DLL dllexport
#include <matrix-orbital/be-screen.hxx>

// ----------------------------------------------------------------------
// BeScreen
// ----------------------------------------------------------------------

BeScreen::BeScreen(lcd_type &lcd)
    : Inherited(lcd)
{
}

BeScreen::~BeScreen()
{
}

// ----------------------------------------------------------------------
// BePlatformScreen
// ----------------------------------------------------------------------

// ______________________
// |%% BeOS PLATFORM %%%|
// |CPU: PPro           |
// |OS:  BeOS R4.1      |
// |Platform: AT Clone  |
// ----------------------


BePlatformScreen::BePlatformScreen(lcd_type &lcd)
    : Inherited(lcd)
{
    status_t status = _get_system_info(&_initial_info, sizeof(system_info));
}

BePlatformScreen::~BePlatformScreen()
{
}

void
BePlatformScreen::do_draw()
{
    _lcd.draw_title_banner("BeOS PLATFORM");
}

// ----------------------------------------------------------------------
// Utils
// ----------------------------------------------------------------------

const char *
BeScreen::to_string(platform_type platform)
{
    switch ( platform ) {
      case B_BEBOX_PLATFORM:
	  return "BeBox"; 
      case B_MAC_PLATFORM:
	  return "Mac"; 
      case B_AT_CLONE_PLATFORM:
	  return "AT Clone"; 
      case B_ENIAC_PLATFORM:
	  return "Eniac"; 
      case B_APPLE_II_PLATFORM:
	  return "Apple II"; 
      case B_CRAY_PLATFORM:
	  return "Cray"; 
      case B_LISA_PLATFORM:
	  return "Lisa"; 
      case B_TI_994A_PLATFORM:
	  return "TI 994a"; 
      case B_TIMEX_SINCLAIR_PLATFORM:
	  return "Sinclair"; 
      case B_ORAC_1_PLATFORM:
	  return "ORAC-1"; 
      case B_HAL_PLATFORM:
	  return "HAL 9000"; 
      case B_BESM_6_PLATFORM:
	  return "BESM-6"; 
      case B_MK_61_PLATFORM:
	  return "MK 61"; 
      case B_NINTENDO_64_PLATFORM:
	  return "Nintendo 64";
      default:
	  return "Unknown";
    }
}

const char *
BeScreen::to_string(cpu_type cpu)
{
    switch ( cpu ) {
      case B_CPU_PPC_601:
	  return "PPC 601";
      case B_CPU_PPC_603:
	  return "PPC 603";
      case B_CPU_PPC_603e:
	  return "PPC 603e";
      case B_CPU_PPC_604:
	  return "PPC 604";
      case B_CPU_PPC_604e:
	  return "PPC 604e";
      case B_CPU_PPC_750:
	  return "PPC 760";
      case B_CPU_PPC_686:
	  return "PPC 686";
      case B_CPU_AMD_29K:
	  return "AMD 29k";
      case B_CPU_X86:
	  return "x86";
      case B_CPU_MC6502:
	  return "MC6502";
      case B_CPU_Z80:
	  return "Z80";
      case B_CPU_ALPHA:
	  return "Alpha";
      case B_CPU_MIPS:
	  return "MIPS";
      case B_CPU_HPPA:
	  return "HP-PA";
      case B_CPU_M68K:
	  return "M68k";
      case B_CPU_ARM:
	  return "ARM";
      case B_CPU_SH:
	  return "SH";
      case B_CPU_SPARC:
	  return "SPARC";
      
      case B_CPU_INTEL_X86:
	  return "ix86";
      case B_CPU_INTEL_PENTIUM:
	  return "Pentium";
      case B_CPU_INTEL_PENTIUM75:
	  return "Pentium 75";
      case B_CPU_INTEL_PENTIUM_486_OVERDRIVE:
	  return "486OD";
      case B_CPU_INTEL_PENTIUM_MMX:
	  return "Pentium MMX";
      case B_CPU_INTEL_PENTIUM_MMX_MODEL_4:
	  return "Pentium MMXm4";
      case B_CPU_INTEL_PENTIUM_MMX_MODEL_8:
	  return "Pentium MMXm8";
      case B_CPU_INTEL_PENTIUM75_486_OVERDRIVE:
	  return "";
      case B_CPU_INTEL_PENTIUM_PRO:
	  return "PPro";
      case B_CPU_INTEL_PENTIUM_II:
	  return "PII";
      case B_CPU_INTEL_PENTIUM_II_MODEL_3:
	  return "PII m3";
      case B_CPU_INTEL_PENTIUM_II_MODEL_5:
	  return "PII m5";
      case B_CPU_INTEL_CELERON:
	  return "Celeron";
      
      case B_CPU_AMD_X86:
	  return "AMD x86";
      case B_CPU_AMD_K5_MODEL0:
	  return "AMD K5m0";
      case B_CPU_AMD_K5_MODEL1:
	  return "AMD K5m1";
      case B_CPU_AMD_K5_MODEL2:
	  return "AMD K5m2";
      case B_CPU_AMD_K5_MODEL3:
	  return "AMD K5m3";
      
      case B_CPU_AMD_K6_MODEL6:
	  return "AMD K6m6";
      case B_CPU_AMD_K6_MODEL7:
	  return "AMD K6m7";
      case B_CPU_AMD_K6_MODEL8:
	  return "AMD K6m8";
      case B_CPU_AMD_K6_MODEL9:
	  return "AMD K6m9";
      
      case B_CPU_CYRIX_X86:
	  return "Cyrix x86";
      case B_CPU_CYRIX_GXm:
	  return "Cyrix GXm";
      case B_CPU_CYRIX_6x86MX:
	  return "Cyrix 6x86MX";
      default:
	  return "Unknown";
    }
}
