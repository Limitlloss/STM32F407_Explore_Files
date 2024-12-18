/*********************************************************************
*                    SEGGER Microcontroller GmbH                     *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2021  SEGGER Microcontroller GmbH                *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V6.22 - Graphical user interface for embedded applications **
emWin is protected by international copyright laws.   Knowledge of the
source code may not be used to write a similar product.  This file may
only  be used  in accordance  with  a license  and should  not be  re-
distributed in any way. We appreciate your understanding and fairness.
----------------------------------------------------------------------
* Source file: Icon_JLink_64x64                                      *
* Dimensions:  64 * 64                                               *
* NumColors:   154                                                   *
---------------------------END-OF-HEADER------------------------------
*/

#include <stdlib.h>

#include "GUI.h"

#ifndef GUI_CONST_STORAGE
  #define GUI_CONST_STORAGE const
#endif

extern GUI_CONST_STORAGE GUI_BITMAP bmIcon_JLink_64x64;

/*********************************************************************
*
*       Palette
*
*  Description
*    The following are the entries of the palette table.
*    The entries are stored as a 32-bit values of which 24 bits are
*    actually used according to the following bit mask: 0xBBGGRR
*
*    The lower   8 bits represent the Red   component.
*    The middle  8 bits represent the Green component.
*    The highest 8 bits represent the Blue  component.
*/
static GUI_CONST_STORAGE GUI_COLOR _ColorsIcon_JLink_64x64[] = {
#if (GUI_USE_ARGB == 0)
  0x00FFFFFF, 0x00FFFFFF, 0x001B1D1D, 0x00A95C00,
  0x00FAFAFA, 0x00D7B388, 0x00E8D4BB, 0x00C2C3C3,
  0x00B47222, 0x00C69255, 0x00F9F4EE, 0x00A4A5A5,
  0x00414242, 0x005D5F5F, 0x00F5ECE1, 0x00FBF7F3,
  0x001C1E1E, 0x00DFDFDF, 0x001B1C1C, 0x00DDBE99,
  0x001D1D1D, 0x001D1F1F, 0x00F1F2F2, 0x001B1E1E,
  0x004B4D4D, 0x004C4E4E, 0x00AF6812, 0x00B7772B,
  0x00F6EEE5, 0x00F4F4F4, 0x003F4040, 0x005C5E5E,
  0x005D5E5E, 0x00626363, 0x00747676, 0x00AA5E03,
  0x00AC6108, 0x00AD640D, 0x00B16B17, 0x00B36F1D,
  0x00D0A775, 0x00D2A979, 0x00999A9A, 0x00DAB992,
  0x00DFC3A1, 0x00E8D4BC, 0x00C3C4C4, 0x00CACBCB,
  0x00EEDFCD, 0x00E1E1E1, 0x00F2F2F2, 0x00FEFEFD,
  0x00FFFEFE, 0x001E2020, 0x00222424, 0x00232525,
  0x00242626, 0x00282A2A, 0x002D2F2F, 0x00303232,
  0x00323434, 0x00373838, 0x00393A3A, 0x003A3C3C,
  0x003B3D3D, 0x003E4040, 0x00494A4A, 0x00545656,
  0x00595A5A, 0x005A5C5C, 0x005C5D5D, 0x005E5F5F,
  0x00646666, 0x00656767, 0x00686969, 0x006C6E6E,
  0x006E6F6F, 0x006F7070, 0x00727373, 0x00797A7A,
  0x007D7E7E, 0x00AE6610, 0x00B47020, 0x00B57324,
  0x00B57425, 0x00B67426, 0x00B7772A, 0x00BA7C32,
  0x00BC8039, 0x00BD813A, 0x00BE833D, 0x00BE843E,
  0x00BF8540, 0x00C28B49, 0x00C38D4C, 0x00C9985E,
  0x00CA9B63, 0x00CB9C64, 0x00CDA06A, 0x00D0A674,
  0x00808181, 0x00858686, 0x00898A8A, 0x00929393,
  0x00A3A4A4, 0x00A5A5A5, 0x00A6A7A7, 0x00ABACAC,
  0x00ACADAD, 0x00B1B2B2, 0x00B2B3B3, 0x00B5B5B5,
  0x00BCBDBD, 0x00D7B287, 0x00D7B489, 0x00D8B48A,
  0x00DAB991, 0x00DBBA93, 0x00DCBD98, 0x00E0C4A2,
  0x00E0C5A4, 0x00E1C7A7, 0x00E5CEB3, 0x00E6CFB4,
  0x00E7D1B7, 0x00E8D3BA, 0x00E9D6BF, 0x00C8C8C8,
  0x00CBCCCC, 0x00D1D2D2, 0x00D5D5D5, 0x00D6D6D6,
  0x00D8D8D8, 0x00D9DADA, 0x00EAD8C2, 0x00F4EBDF,
  0x00E1E2E2, 0x00E5E5E5, 0x00E9E9E9, 0x00F5EBE0,
  0x00F6EEE4, 0x00F7F0E7, 0x00F8F2EB, 0x00F1F1F1,
  0x00F2F3F3, 0x00FAF6F1, 0x00FCF9F5, 0x00FCFAF7,
  0x00F8F8F8, 0x00FDFBF9, 0x00FEFCFB, 0x00FCFCFC,
  0x00FEFDFC, 0x00FEFEFE
#else
  0xFFFFFFFF, 0xFFFFFFFF, 0xFF1D1D1B, 0xFF005CA9,
  0xFFFAFAFA, 0xFF88B3D7, 0xFFBBD4E8, 0xFFC3C3C2,
  0xFF2272B4, 0xFF5592C6, 0xFFEEF4F9, 0xFFA5A5A4,
  0xFF424241, 0xFF5F5F5D, 0xFFE1ECF5, 0xFFF3F7FB,
  0xFF1E1E1C, 0xFFDFDFDF, 0xFF1C1C1B, 0xFF99BEDD,
  0xFF1D1D1D, 0xFF1F1F1D, 0xFFF2F2F1, 0xFF1E1E1B,
  0xFF4D4D4B, 0xFF4E4E4C, 0xFF1268AF, 0xFF2B77B7,
  0xFFE5EEF6, 0xFFF4F4F4, 0xFF40403F, 0xFF5E5E5C,
  0xFF5E5E5D, 0xFF636362, 0xFF767674, 0xFF035EAA,
  0xFF0861AC, 0xFF0D64AD, 0xFF176BB1, 0xFF1D6FB3,
  0xFF75A7D0, 0xFF79A9D2, 0xFF9A9A99, 0xFF92B9DA,
  0xFFA1C3DF, 0xFFBCD4E8, 0xFFC4C4C3, 0xFFCBCBCA,
  0xFFCDDFEE, 0xFFE1E1E1, 0xFFF2F2F2, 0xFFFDFEFE,
  0xFFFEFEFF, 0xFF20201E, 0xFF242422, 0xFF252523,
  0xFF262624, 0xFF2A2A28, 0xFF2F2F2D, 0xFF323230,
  0xFF343432, 0xFF383837, 0xFF3A3A39, 0xFF3C3C3A,
  0xFF3D3D3B, 0xFF40403E, 0xFF4A4A49, 0xFF565654,
  0xFF5A5A59, 0xFF5C5C5A, 0xFF5D5D5C, 0xFF5F5F5E,
  0xFF666664, 0xFF676765, 0xFF696968, 0xFF6E6E6C,
  0xFF6F6F6E, 0xFF70706F, 0xFF737372, 0xFF7A7A79,
  0xFF7E7E7D, 0xFF1066AE, 0xFF2070B4, 0xFF2473B5,
  0xFF2574B5, 0xFF2674B6, 0xFF2A77B7, 0xFF327CBA,
  0xFF3980BC, 0xFF3A81BD, 0xFF3D83BE, 0xFF3E84BE,
  0xFF4085BF, 0xFF498BC2, 0xFF4C8DC3, 0xFF5E98C9,
  0xFF639BCA, 0xFF649CCB, 0xFF6AA0CD, 0xFF74A6D0,
  0xFF818180, 0xFF868685, 0xFF8A8A89, 0xFF939392,
  0xFFA4A4A3, 0xFFA5A5A5, 0xFFA7A7A6, 0xFFACACAB,
  0xFFADADAC, 0xFFB2B2B1, 0xFFB3B3B2, 0xFFB5B5B5,
  0xFFBDBDBC, 0xFF87B2D7, 0xFF89B4D7, 0xFF8AB4D8,
  0xFF91B9DA, 0xFF93BADB, 0xFF98BDDC, 0xFFA2C4E0,
  0xFFA4C5E0, 0xFFA7C7E1, 0xFFB3CEE5, 0xFFB4CFE6,
  0xFFB7D1E7, 0xFFBAD3E8, 0xFFBFD6E9, 0xFFC8C8C8,
  0xFFCCCCCB, 0xFFD2D2D1, 0xFFD5D5D5, 0xFFD6D6D6,
  0xFFD8D8D8, 0xFFDADAD9, 0xFFC2D8EA, 0xFFDFEBF4,
  0xFFE2E2E1, 0xFFE5E5E5, 0xFFE9E9E9, 0xFFE0EBF5,
  0xFFE4EEF6, 0xFFE7F0F7, 0xFFEBF2F8, 0xFFF1F1F1,
  0xFFF3F3F2, 0xFFF1F6FA, 0xFFF5F9FC, 0xFFF7FAFC,
  0xFFF8F8F8, 0xFFF9FBFD, 0xFFFBFCFE, 0xFFFCFCFC,
  0xFFFCFDFE, 0xFFFEFEFE
#endif

};

static GUI_CONST_STORAGE GUI_LOGPALETTE _PalIcon_JLink_64x64 = {
  154,  // Number of entries
  1,    // Has transparency
  &_ColorsIcon_JLink_64x64[0]
};

static GUI_CONST_STORAGE unsigned char _acIcon_JLink_64x64[] = {
  /* ABS: 002 Pixels @ 000,000 */ 0, 2, 0x14, 0x17, 
  /* RLE: 060 Pixels @ 002,000 */ 60, 0x02, 
  /* ABS: 005 Pixels @ 062,000 */ 0, 5, 0x12, 0x14, 0x17, 0x20, 0x1D, 
  /* RLE: 058 Pixels @ 003,001 */ 58, 0x01, 
  /* ABS: 005 Pixels @ 061,001 */ 0, 5, 0x1D, 0x20, 0x12, 0x02, 0x16, 
  /* RLE: 060 Pixels @ 002,002 */ 60, 0x01, 
  /* ABS: 004 Pixels @ 062,002 */ 0, 4, 0x16, 0x02, 0x02, 0x04, 
  /* RLE: 060 Pixels @ 002,003 */ 60, 0x01, 
  /* ABS: 004 Pixels @ 062,003 */ 0, 4, 0x04, 0x02, 0x02, 0x04, 
  /* RLE: 060 Pixels @ 002,004 */ 60, 0x01, 
  /* ABS: 004 Pixels @ 062,004 */ 0, 4, 0x04, 0x02, 0x02, 0x04, 
  /* RLE: 060 Pixels @ 002,005 */ 60, 0x01, 
  /* ABS: 004 Pixels @ 062,005 */ 0, 4, 0x04, 0x02, 0x02, 0x04, 
  /* RLE: 060 Pixels @ 002,006 */ 60, 0x01, 
  /* ABS: 004 Pixels @ 062,006 */ 0, 4, 0x04, 0x02, 0x02, 0x04, 
  /* RLE: 018 Pixels @ 002,007 */ 18, 0x01, 
  /* ABS: 003 Pixels @ 020,007 */ 0, 3, 0x34, 0x7B, 0x73, 
  /* RLE: 019 Pixels @ 023,007 */ 19, 0x05, 
  /* ABS: 002 Pixels @ 042,007 */ 0, 2, 0x78, 0x93, 
  /* RLE: 018 Pixels @ 044,007 */ 18, 0x01, 
  /* ABS: 004 Pixels @ 062,007 */ 0, 4, 0x04, 0x02, 0x02, 0x04, 
  /* RLE: 018 Pixels @ 002,008 */ 18, 0x01, 
  /* RLE: 001 Pixels @ 020,008 */ 1, 0x29, 
  /* RLE: 022 Pixels @ 021,008 */ 22, 0x03, 
  /* RLE: 001 Pixels @ 043,008 */ 1, 0x5C, 
  /* RLE: 018 Pixels @ 044,008 */ 18, 0x01, 
  /* ABS: 004 Pixels @ 062,008 */ 0, 4, 0x04, 0x02, 0x02, 0x04, 
  /* RLE: 018 Pixels @ 002,009 */ 18, 0x01, 
  /* RLE: 001 Pixels @ 020,009 */ 1, 0x55, 
  /* RLE: 023 Pixels @ 021,009 */ 23, 0x03, 
  /* RLE: 001 Pixels @ 044,009 */ 1, 0x8E, 
  /* RLE: 017 Pixels @ 045,009 */ 17, 0x01, 
  /* ABS: 004 Pixels @ 062,009 */ 0, 4, 0x04, 0x02, 0x02, 0x04, 
  /* RLE: 008 Pixels @ 002,010 */ 8, 0x01, 
  /* RLE: 001 Pixels @ 010,010 */ 1, 0x2D, 
  /* RLE: 009 Pixels @ 011,010 */ 9, 0x05, 
  /* RLE: 001 Pixels @ 020,010 */ 1, 0x51, 
  /* RLE: 023 Pixels @ 021,010 */ 23, 0x03, 
  /* RLE: 001 Pixels @ 044,010 */ 1, 0x29, 
  /* RLE: 008 Pixels @ 045,010 */ 8, 0x05, 
  /* RLE: 001 Pixels @ 053,010 */ 1, 0x2D, 
  /* RLE: 008 Pixels @ 054,010 */ 8, 0x01, 
  /* ABS: 004 Pixels @ 062,010 */ 0, 4, 0x04, 0x02, 0x02, 0x04, 
  /* RLE: 008 Pixels @ 002,011 */ 8, 0x01, 
  /* RLE: 001 Pixels @ 010,011 */ 1, 0x1B, 
  /* RLE: 042 Pixels @ 011,011 */ 42, 0x03, 
  /* RLE: 001 Pixels @ 053,011 */ 1, 0x1B, 
  /* RLE: 008 Pixels @ 054,011 */ 8, 0x01, 
  /* ABS: 004 Pixels @ 062,011 */ 0, 4, 0x04, 0x02, 0x02, 0x04, 
  /* RLE: 008 Pixels @ 002,012 */ 8, 0x01, 
  /* RLE: 001 Pixels @ 010,012 */ 1, 0x1C, 
  /* RLE: 009 Pixels @ 011,012 */ 9, 0x06, 
  /* RLE: 001 Pixels @ 020,012 */ 1, 0x1A, 
  /* RLE: 023 Pixels @ 021,012 */ 23, 0x03, 
  /* RLE: 001 Pixels @ 044,012 */ 1, 0x79, 
  /* RLE: 008 Pixels @ 045,012 */ 8, 0x06, 
  /* RLE: 001 Pixels @ 053,012 */ 1, 0x1C, 
  /* RLE: 008 Pixels @ 054,012 */ 8, 0x01, 
  /* ABS: 004 Pixels @ 062,012 */ 0, 4, 0x04, 0x02, 0x02, 0x04, 
  /* RLE: 008 Pixels @ 002,013 */ 8, 0x01, 
  /* RLE: 001 Pixels @ 010,013 */ 1, 0x74, 
  /* RLE: 009 Pixels @ 011,013 */ 9, 0x09, 
  /* RLE: 001 Pixels @ 020,013 */ 1, 0x24, 
  /* RLE: 023 Pixels @ 021,013 */ 23, 0x03, 
  /* RLE: 001 Pixels @ 044,013 */ 1, 0x5E, 
  /* RLE: 008 Pixels @ 045,013 */ 8, 0x09, 
  /* RLE: 001 Pixels @ 053,013 */ 1, 0x2B, 
  /* RLE: 008 Pixels @ 054,013 */ 8, 0x01, 
  /* ABS: 004 Pixels @ 062,013 */ 0, 4, 0x04, 0x02, 0x02, 0x04, 
  /* RLE: 008 Pixels @ 002,014 */ 8, 0x01, 
  /* RLE: 001 Pixels @ 010,014 */ 1, 0x5A, 
  /* RLE: 042 Pixels @ 011,014 */ 42, 0x03, 
  /* RLE: 001 Pixels @ 053,014 */ 1, 0x5B, 
  /* RLE: 008 Pixels @ 054,014 */ 8, 0x01, 
  /* ABS: 004 Pixels @ 062,014 */ 0, 4, 0x04, 0x02, 0x02, 0x04, 
  /* RLE: 008 Pixels @ 002,015 */ 8, 0x01, 
  /* RLE: 001 Pixels @ 010,015 */ 1, 0x33, 
  /* RLE: 009 Pixels @ 011,015 */ 9, 0x0A, 
  /* RLE: 001 Pixels @ 020,015 */ 1, 0x26, 
  /* RLE: 023 Pixels @ 021,015 */ 23, 0x03, 
  /* RLE: 001 Pixels @ 044,015 */ 1, 0x30, 
  /* RLE: 008 Pixels @ 045,015 */ 8, 0x0A, 
  /* RLE: 001 Pixels @ 053,015 */ 1, 0x33, 
  /* RLE: 008 Pixels @ 054,015 */ 8, 0x01, 
  /* ABS: 004 Pixels @ 062,015 */ 0, 4, 0x04, 0x02, 0x02, 0x04, 
  /* RLE: 008 Pixels @ 002,016 */ 8, 0x01, 
  /* RLE: 001 Pixels @ 010,016 */ 1, 0x60, 
  /* RLE: 009 Pixels @ 011,016 */ 9, 0x08, 
  /* RLE: 001 Pixels @ 020,016 */ 1, 0x23, 
  /* RLE: 023 Pixels @ 021,016 */ 23, 0x03, 
  /* RLE: 001 Pixels @ 044,016 */ 1, 0x27, 
  /* RLE: 008 Pixels @ 045,016 */ 8, 0x08, 
  /* RLE: 001 Pixels @ 053,016 */ 1, 0x61, 
  /* RLE: 008 Pixels @ 054,016 */ 8, 0x01, 
  /* ABS: 004 Pixels @ 062,016 */ 0, 4, 0x04, 0x02, 0x02, 0x04, 
  /* RLE: 008 Pixels @ 002,017 */ 8, 0x01, 
  /* RLE: 001 Pixels @ 010,017 */ 1, 0x62, 
  /* RLE: 009 Pixels @ 011,017 */ 9, 0x08, 
  /* RLE: 001 Pixels @ 020,017 */ 1, 0x23, 
  /* RLE: 013 Pixels @ 021,017 */ 13, 0x03, 
  /* RLE: 001 Pixels @ 034,017 */ 1, 0x2B, 
  /* RLE: 005 Pixels @ 035,017 */ 5, 0x13, 
  /* ABS: 005 Pixels @ 040,017 */ 0, 5, 0x71, 0x03, 0x03, 0x03, 0x27, 
  /* RLE: 008 Pixels @ 045,017 */ 8, 0x08, 
  /* RLE: 001 Pixels @ 053,017 */ 1, 0x5F, 
  /* RLE: 008 Pixels @ 054,017 */ 8, 0x01, 
  /* ABS: 004 Pixels @ 062,017 */ 0, 4, 0x04, 0x02, 0x02, 0x04, 
  /* RLE: 008 Pixels @ 002,018 */ 8, 0x01, 
  /* RLE: 001 Pixels @ 010,018 */ 1, 0x98, 
  /* RLE: 009 Pixels @ 011,018 */ 9, 0x0A, 
  /* RLE: 001 Pixels @ 020,018 */ 1, 0x26, 
  /* RLE: 013 Pixels @ 021,018 */ 13, 0x03, 
  /* RLE: 001 Pixels @ 034,018 */ 1, 0x0F, 
  /* RLE: 005 Pixels @ 035,018 */ 5, 0x01, 
  /* ABS: 005 Pixels @ 040,018 */ 0, 5, 0x0E, 0x03, 0x03, 0x03, 0x30, 
  /* RLE: 008 Pixels @ 045,018 */ 8, 0x0A, 
  /* RLE: 001 Pixels @ 053,018 */ 1, 0x96, 
  /* RLE: 008 Pixels @ 054,018 */ 8, 0x01, 
  /* ABS: 004 Pixels @ 062,018 */ 0, 4, 0x04, 0x02, 0x02, 0x04, 
  /* RLE: 008 Pixels @ 002,019 */ 8, 0x01, 
  /* RLE: 001 Pixels @ 010,019 */ 1, 0x58, 
  /* RLE: 023 Pixels @ 011,019 */ 23, 0x03, 
  /* RLE: 001 Pixels @ 034,019 */ 1, 0x0F, 
  /* RLE: 005 Pixels @ 035,019 */ 5, 0x01, 
  /* RLE: 001 Pixels @ 040,019 */ 1, 0x0E, 
  /* RLE: 012 Pixels @ 041,019 */ 12, 0x03, 
  /* RLE: 001 Pixels @ 053,019 */ 1, 0x57, 
  /* RLE: 008 Pixels @ 054,019 */ 8, 0x01, 
  /* ABS: 004 Pixels @ 062,019 */ 0, 4, 0x04, 0x02, 0x02, 0x04, 
  /* RLE: 008 Pixels @ 002,020 */ 8, 0x01, 
  /* RLE: 001 Pixels @ 010,020 */ 1, 0x76, 
  /* RLE: 009 Pixels @ 011,020 */ 9, 0x09, 
  /* RLE: 001 Pixels @ 020,020 */ 1, 0x24, 
  /* RLE: 013 Pixels @ 021,020 */ 13, 0x03, 
  /* RLE: 001 Pixels @ 034,020 */ 1, 0x0F, 
  /* RLE: 005 Pixels @ 035,020 */ 5, 0x01, 
  /* ABS: 005 Pixels @ 040,020 */ 0, 5, 0x0E, 0x03, 0x03, 0x03, 0x5D, 
  /* RLE: 008 Pixels @ 045,020 */ 8, 0x09, 
  /* RLE: 001 Pixels @ 053,020 */ 1, 0x75, 
  /* RLE: 008 Pixels @ 054,020 */ 8, 0x01, 
  /* ABS: 004 Pixels @ 062,020 */ 0, 4, 0x04, 0x02, 0x02, 0x04, 
  /* RLE: 008 Pixels @ 002,021 */ 8, 0x01, 
  /* RLE: 001 Pixels @ 010,021 */ 1, 0x8B, 
  /* RLE: 009 Pixels @ 011,021 */ 9, 0x06, 
  /* RLE: 001 Pixels @ 020,021 */ 1, 0x1A, 
  /* RLE: 013 Pixels @ 021,021 */ 13, 0x03, 
  /* RLE: 001 Pixels @ 034,021 */ 1, 0x0F, 
  /* RLE: 005 Pixels @ 035,021 */ 5, 0x01, 
  /* ABS: 005 Pixels @ 040,021 */ 0, 5, 0x0E, 0x03, 0x03, 0x03, 0x2C, 
  /* RLE: 008 Pixels @ 045,021 */ 8, 0x06, 
  /* RLE: 001 Pixels @ 053,021 */ 1, 0x87, 
  /* RLE: 008 Pixels @ 054,021 */ 8, 0x01, 
  /* ABS: 004 Pixels @ 062,021 */ 0, 4, 0x04, 0x02, 0x02, 0x04, 
  /* RLE: 008 Pixels @ 002,022 */ 8, 0x01, 
  /* RLE: 001 Pixels @ 010,022 */ 1, 0x56, 
  /* RLE: 023 Pixels @ 011,022 */ 23, 0x03, 
  /* RLE: 001 Pixels @ 034,022 */ 1, 0x0F, 
  /* RLE: 005 Pixels @ 035,022 */ 5, 0x01, 
  /* RLE: 001 Pixels @ 040,022 */ 1, 0x0E, 
  /* RLE: 012 Pixels @ 041,022 */ 12, 0x03, 
  /* RLE: 001 Pixels @ 053,022 */ 1, 0x53, 
  /* RLE: 008 Pixels @ 054,022 */ 8, 0x01, 
  /* ABS: 004 Pixels @ 062,022 */ 0, 4, 0x04, 0x02, 0x02, 0x04, 
  /* RLE: 008 Pixels @ 002,023 */ 8, 0x01, 
  /* RLE: 001 Pixels @ 010,023 */ 1, 0x86, 
  /* RLE: 009 Pixels @ 011,023 */ 9, 0x05, 
  /* RLE: 001 Pixels @ 020,023 */ 1, 0x25, 
  /* RLE: 013 Pixels @ 021,023 */ 13, 0x03, 
  /* RLE: 001 Pixels @ 034,023 */ 1, 0x0F, 
  /* RLE: 005 Pixels @ 035,023 */ 5, 0x01, 
  /* ABS: 005 Pixels @ 040,023 */ 0, 5, 0x0E, 0x03, 0x03, 0x03, 0x28, 
  /* RLE: 008 Pixels @ 045,023 */ 8, 0x05, 
  /* RLE: 001 Pixels @ 053,023 */ 1, 0x7E, 
  /* RLE: 008 Pixels @ 054,023 */ 8, 0x01, 
  /* ABS: 004 Pixels @ 062,023 */ 0, 4, 0x04, 0x02, 0x02, 0x04, 
  /* RLE: 008 Pixels @ 002,024 */ 8, 0x01, 
  /* RLE: 001 Pixels @ 010,024 */ 1, 0x7D, 
  /* RLE: 009 Pixels @ 011,024 */ 9, 0x05, 
  /* RLE: 001 Pixels @ 020,024 */ 1, 0x25, 
  /* RLE: 013 Pixels @ 021,024 */ 13, 0x03, 
  /* RLE: 001 Pixels @ 034,024 */ 1, 0x0F, 
  /* RLE: 005 Pixels @ 035,024 */ 5, 0x01, 
  /* ABS: 005 Pixels @ 040,024 */ 0, 5, 0x0E, 0x03, 0x03, 0x03, 0x28, 
  /* RLE: 008 Pixels @ 045,024 */ 8, 0x05, 
  /* RLE: 001 Pixels @ 053,024 */ 1, 0x7C, 
  /* RLE: 008 Pixels @ 054,024 */ 8, 0x01, 
  /* ABS: 004 Pixels @ 062,024 */ 0, 4, 0x04, 0x02, 0x02, 0x04, 
  /* RLE: 008 Pixels @ 002,025 */ 8, 0x01, 
  /* RLE: 001 Pixels @ 010,025 */ 1, 0x1B, 
  /* RLE: 023 Pixels @ 011,025 */ 23, 0x03, 
  /* RLE: 001 Pixels @ 034,025 */ 1, 0x0F, 
  /* RLE: 005 Pixels @ 035,025 */ 5, 0x01, 
  /* RLE: 001 Pixels @ 040,025 */ 1, 0x0E, 
  /* RLE: 012 Pixels @ 041,025 */ 12, 0x03, 
  /* RLE: 001 Pixels @ 053,025 */ 1, 0x54, 
  /* RLE: 008 Pixels @ 054,025 */ 8, 0x01, 
  /* ABS: 004 Pixels @ 062,025 */ 0, 4, 0x04, 0x02, 0x02, 0x04, 
  /* RLE: 008 Pixels @ 002,026 */ 8, 0x01, 
  /* RLE: 001 Pixels @ 010,026 */ 1, 0x8D, 
  /* RLE: 009 Pixels @ 011,026 */ 9, 0x06, 
  /* RLE: 001 Pixels @ 020,026 */ 1, 0x1A, 
  /* RLE: 013 Pixels @ 021,026 */ 13, 0x03, 
  /* RLE: 001 Pixels @ 034,026 */ 1, 0x0F, 
  /* RLE: 005 Pixels @ 035,026 */ 5, 0x01, 
  /* ABS: 005 Pixels @ 040,026 */ 0, 5, 0x0E, 0x03, 0x03, 0x03, 0x2C, 
  /* RLE: 008 Pixels @ 045,026 */ 8, 0x06, 
  /* RLE: 001 Pixels @ 053,026 */ 1, 0x1C, 
  /* RLE: 008 Pixels @ 054,026 */ 8, 0x01, 
  /* ABS: 004 Pixels @ 062,026 */ 0, 4, 0x04, 0x02, 0x02, 0x04, 
  /* RLE: 018 Pixels @ 002,027 */ 18, 0x01, 
  /* RLE: 001 Pixels @ 020,027 */ 1, 0x52, 
  /* RLE: 013 Pixels @ 021,027 */ 13, 0x03, 
  /* RLE: 001 Pixels @ 034,027 */ 1, 0x0F, 
  /* RLE: 005 Pixels @ 035,027 */ 5, 0x01, 
  /* ABS: 005 Pixels @ 040,027 */ 0, 5, 0x0E, 0x03, 0x03, 0x03, 0x8C, 
  /* RLE: 017 Pixels @ 045,027 */ 17, 0x01, 
  /* ABS: 004 Pixels @ 062,027 */ 0, 4, 0x04, 0x02, 0x02, 0x04, 
  /* RLE: 018 Pixels @ 002,028 */ 18, 0x01, 
  /* RLE: 001 Pixels @ 020,028 */ 1, 0x63, 
  /* RLE: 013 Pixels @ 021,028 */ 13, 0x03, 
  /* RLE: 001 Pixels @ 034,028 */ 1, 0x0F, 
  /* RLE: 005 Pixels @ 035,028 */ 5, 0x01, 
  /* ABS: 004 Pixels @ 040,028 */ 0, 4, 0x0E, 0x03, 0x03, 0x59, 
  /* RLE: 018 Pixels @ 044,028 */ 18, 0x01, 
  /* ABS: 004 Pixels @ 062,028 */ 0, 4, 0x04, 0x02, 0x02, 0x04, 
  /* RLE: 018 Pixels @ 002,029 */ 18, 0x01, 
  /* ABS: 003 Pixels @ 020,029 */ 0, 3, 0x34, 0x7A, 0x72, 
  /* RLE: 011 Pixels @ 023,029 */ 11, 0x05, 
  /* RLE: 001 Pixels @ 034,029 */ 1, 0x95, 
  /* RLE: 005 Pixels @ 035,029 */ 5, 0x01, 
  /* ABS: 004 Pixels @ 040,029 */ 0, 4, 0x91, 0x05, 0x77, 0x92, 
  /* RLE: 018 Pixels @ 044,029 */ 18, 0x01, 
  /* ABS: 004 Pixels @ 062,029 */ 0, 4, 0x04, 0x02, 0x02, 0x04, 
  /* RLE: 060 Pixels @ 002,030 */ 60, 0x01, 
  /* ABS: 004 Pixels @ 062,030 */ 0, 4, 0x04, 0x02, 0x02, 0x04, 
  /* RLE: 060 Pixels @ 002,031 */ 60, 0x01, 
  /* ABS: 004 Pixels @ 062,031 */ 0, 4, 0x04, 0x02, 0x02, 0x04, 
  /* RLE: 060 Pixels @ 002,032 */ 60, 0x01, 
  /* ABS: 004 Pixels @ 062,032 */ 0, 4, 0x04, 0x02, 0x02, 0x04, 
  /* RLE: 009 Pixels @ 002,033 */ 9, 0x01, 
  /* ABS: 002 Pixels @ 011,033 */ 0, 2, 0x80, 0x69, 
  /* RLE: 021 Pixels @ 013,033 */ 21, 0x0B, 
  /* RLE: 006 Pixels @ 034,033 */ 6, 0x01, 
  /* RLE: 001 Pixels @ 040,033 */ 1, 0x90, 
  /* RLE: 010 Pixels @ 041,033 */ 10, 0x0B, 
  /* ABS: 002 Pixels @ 051,033 */ 0, 2, 0x6A, 0x82, 
  /* RLE: 009 Pixels @ 053,033 */ 9, 0x01, 
  /* ABS: 004 Pixels @ 062,033 */ 0, 4, 0x04, 0x02, 0x02, 0x04, 
  /* RLE: 008 Pixels @ 002,034 */ 8, 0x01, 
  /* RLE: 001 Pixels @ 010,034 */ 1, 0x2A, 
  /* RLE: 022 Pixels @ 011,034 */ 22, 0x02, 
  /* RLE: 001 Pixels @ 033,034 */ 1, 0x10, 
  /* RLE: 006 Pixels @ 034,034 */ 6, 0x01, 
  /* RLE: 001 Pixels @ 040,034 */ 1, 0x11, 
  /* RLE: 011 Pixels @ 041,034 */ 11, 0x02, 
  /* ABS: 002 Pixels @ 052,034 */ 0, 2, 0x15, 0x6E, 
  /* RLE: 008 Pixels @ 054,034 */ 8, 0x01, 
  /* ABS: 004 Pixels @ 062,034 */ 0, 4, 0x04, 0x02, 0x02, 0x04, 
  /* RLE: 008 Pixels @ 002,035 */ 8, 0x01, 
  /* RLE: 001 Pixels @ 010,035 */ 1, 0x42, 
  /* RLE: 022 Pixels @ 011,035 */ 22, 0x02, 
  /* RLE: 001 Pixels @ 033,035 */ 1, 0x10, 
  /* RLE: 006 Pixels @ 034,035 */ 6, 0x01, 
  /* RLE: 001 Pixels @ 040,035 */ 1, 0x11, 
  /* RLE: 012 Pixels @ 041,035 */ 12, 0x02, 
  /* RLE: 001 Pixels @ 053,035 */ 1, 0x48, 
  /* RLE: 008 Pixels @ 054,035 */ 8, 0x01, 
  /* ABS: 004 Pixels @ 062,035 */ 0, 4, 0x04, 0x02, 0x02, 0x04, 
  /* RLE: 008 Pixels @ 002,036 */ 8, 0x01, 
  /* RLE: 001 Pixels @ 010,036 */ 1, 0x0C, 
  /* RLE: 022 Pixels @ 011,036 */ 22, 0x02, 
  /* RLE: 001 Pixels @ 033,036 */ 1, 0x10, 
  /* RLE: 006 Pixels @ 034,036 */ 6, 0x01, 
  /* RLE: 001 Pixels @ 040,036 */ 1, 0x11, 
  /* RLE: 012 Pixels @ 041,036 */ 12, 0x02, 
  /* RLE: 001 Pixels @ 053,036 */ 1, 0x0D, 
  /* RLE: 008 Pixels @ 054,036 */ 8, 0x01, 
  /* ABS: 004 Pixels @ 062,036 */ 0, 4, 0x04, 0x02, 0x02, 0x04, 
  /* RLE: 008 Pixels @ 002,037 */ 8, 0x01, 
  /* RLE: 001 Pixels @ 010,037 */ 1, 0x0C, 
  /* RLE: 022 Pixels @ 011,037 */ 22, 0x02, 
  /* RLE: 001 Pixels @ 033,037 */ 1, 0x10, 
  /* RLE: 006 Pixels @ 034,037 */ 6, 0x01, 
  /* RLE: 001 Pixels @ 040,037 */ 1, 0x11, 
  /* RLE: 012 Pixels @ 041,037 */ 12, 0x02, 
  /* RLE: 001 Pixels @ 053,037 */ 1, 0x0D, 
  /* RLE: 008 Pixels @ 054,037 */ 8, 0x01, 
  /* ABS: 004 Pixels @ 062,037 */ 0, 4, 0x04, 0x02, 0x02, 0x04, 
  /* RLE: 008 Pixels @ 002,038 */ 8, 0x01, 
  /* RLE: 001 Pixels @ 010,038 */ 1, 0x0C, 
  /* RLE: 008 Pixels @ 011,038 */ 8, 0x02, 
  /* RLE: 001 Pixels @ 019,038 */ 1, 0x1F, 
  /* RLE: 006 Pixels @ 020,038 */ 6, 0x07, 
  /* RLE: 001 Pixels @ 026,038 */ 1, 0x1E, 
  /* RLE: 006 Pixels @ 027,038 */ 6, 0x02, 
  /* RLE: 001 Pixels @ 033,038 */ 1, 0x10, 
  /* RLE: 006 Pixels @ 034,038 */ 6, 0x01, 
  /* RLE: 001 Pixels @ 040,038 */ 1, 0x11, 
  /* RLE: 012 Pixels @ 041,038 */ 12, 0x02, 
  /* RLE: 001 Pixels @ 053,038 */ 1, 0x0D, 
  /* RLE: 008 Pixels @ 054,038 */ 8, 0x01, 
  /* ABS: 004 Pixels @ 062,038 */ 0, 4, 0x04, 0x02, 0x02, 0x04, 
  /* RLE: 008 Pixels @ 002,039 */ 8, 0x01, 
  /* RLE: 001 Pixels @ 010,039 */ 1, 0x0C, 
  /* RLE: 008 Pixels @ 011,039 */ 8, 0x02, 
  /* RLE: 001 Pixels @ 019,039 */ 1, 0x22, 
  /* RLE: 006 Pixels @ 020,039 */ 6, 0x01, 
  /* RLE: 001 Pixels @ 026,039 */ 1, 0x19, 
  /* RLE: 006 Pixels @ 027,039 */ 6, 0x02, 
  /* RLE: 001 Pixels @ 033,039 */ 1, 0x10, 
  /* RLE: 006 Pixels @ 034,039 */ 6, 0x01, 
  /* RLE: 001 Pixels @ 040,039 */ 1, 0x11, 
  /* RLE: 012 Pixels @ 041,039 */ 12, 0x02, 
  /* RLE: 001 Pixels @ 053,039 */ 1, 0x0D, 
  /* RLE: 008 Pixels @ 054,039 */ 8, 0x01, 
  /* ABS: 004 Pixels @ 062,039 */ 0, 4, 0x04, 0x02, 0x02, 0x04, 
  /* RLE: 008 Pixels @ 002,040 */ 8, 0x01, 
  /* RLE: 001 Pixels @ 010,040 */ 1, 0x0C, 
  /* RLE: 008 Pixels @ 011,040 */ 8, 0x02, 
  /* RLE: 001 Pixels @ 019,040 */ 1, 0x22, 
  /* RLE: 006 Pixels @ 020,040 */ 6, 0x01, 
  /* RLE: 001 Pixels @ 026,040 */ 1, 0x19, 
  /* RLE: 006 Pixels @ 027,040 */ 6, 0x02, 
  /* RLE: 001 Pixels @ 033,040 */ 1, 0x15, 
  /* RLE: 006 Pixels @ 034,040 */ 6, 0x01, 
  /* RLE: 001 Pixels @ 040,040 */ 1, 0x11, 
  /* RLE: 012 Pixels @ 041,040 */ 12, 0x02, 
  /* RLE: 001 Pixels @ 053,040 */ 1, 0x0D, 
  /* RLE: 008 Pixels @ 054,040 */ 8, 0x01, 
  /* ABS: 004 Pixels @ 062,040 */ 0, 4, 0x04, 0x02, 0x02, 0x04, 
  /* RLE: 008 Pixels @ 002,041 */ 8, 0x01, 
  /* RLE: 001 Pixels @ 010,041 */ 1, 0x0C, 
  /* RLE: 008 Pixels @ 011,041 */ 8, 0x02, 
  /* RLE: 001 Pixels @ 019,041 */ 1, 0x4D, 
  /* RLE: 006 Pixels @ 020,041 */ 6, 0x01, 
  /* RLE: 001 Pixels @ 026,041 */ 1, 0x19, 
  /* RLE: 006 Pixels @ 027,041 */ 6, 0x02, 
  /* RLE: 001 Pixels @ 033,041 */ 1, 0x3A, 
  /* RLE: 006 Pixels @ 034,041 */ 6, 0x01, 
  /* RLE: 001 Pixels @ 040,041 */ 1, 0x85, 
  /* RLE: 012 Pixels @ 041,041 */ 12, 0x02, 
  /* RLE: 001 Pixels @ 053,041 */ 1, 0x0D, 
  /* RLE: 008 Pixels @ 054,041 */ 8, 0x01, 
  /* ABS: 004 Pixels @ 062,041 */ 0, 4, 0x04, 0x02, 0x02, 0x04, 
  /* RLE: 008 Pixels @ 002,042 */ 8, 0x01, 
  /* RLE: 001 Pixels @ 010,042 */ 1, 0x0C, 
  /* RLE: 008 Pixels @ 011,042 */ 8, 0x02, 
  /* RLE: 001 Pixels @ 019,042 */ 1, 0x49, 
  /* RLE: 006 Pixels @ 020,042 */ 6, 0x01, 
  /* RLE: 001 Pixels @ 026,042 */ 1, 0x43, 
  /* RLE: 006 Pixels @ 027,042 */ 6, 0x02, 
  /* RLE: 001 Pixels @ 033,042 */ 1, 0x1E, 
  /* RLE: 006 Pixels @ 034,042 */ 6, 0x01, 
  /* RLE: 001 Pixels @ 040,042 */ 1, 0x81, 
  /* RLE: 012 Pixels @ 041,042 */ 12, 0x02, 
  /* RLE: 001 Pixels @ 053,042 */ 1, 0x0D, 
  /* RLE: 008 Pixels @ 054,042 */ 8, 0x01, 
  /* ABS: 004 Pixels @ 062,042 */ 0, 4, 0x04, 0x02, 0x02, 0x04, 
  /* RLE: 008 Pixels @ 002,043 */ 8, 0x01, 
  /* RLE: 001 Pixels @ 010,043 */ 1, 0x0C, 
  /* RLE: 008 Pixels @ 011,043 */ 8, 0x02, 
  /* RLE: 001 Pixels @ 019,043 */ 1, 0x1F, 
  /* RLE: 006 Pixels @ 020,043 */ 6, 0x01, 
  /* RLE: 001 Pixels @ 026,043 */ 1, 0x4F, 
  /* RLE: 006 Pixels @ 027,043 */ 6, 0x02, 
  /* RLE: 001 Pixels @ 033,043 */ 1, 0x21, 
  /* RLE: 006 Pixels @ 034,043 */ 6, 0x01, 
  /* RLE: 001 Pixels @ 040,043 */ 1, 0x7F, 
  /* RLE: 012 Pixels @ 041,043 */ 12, 0x02, 
  /* RLE: 001 Pixels @ 053,043 */ 1, 0x0D, 
  /* RLE: 008 Pixels @ 054,043 */ 8, 0x01, 
  /* ABS: 004 Pixels @ 062,043 */ 0, 4, 0x04, 0x02, 0x02, 0x04, 
  /* RLE: 008 Pixels @ 002,044 */ 8, 0x01, 
  /* RLE: 001 Pixels @ 010,044 */ 1, 0x0C, 
  /* RLE: 008 Pixels @ 011,044 */ 8, 0x02, 
  /* RLE: 001 Pixels @ 019,044 */ 1, 0x41, 
  /* RLE: 006 Pixels @ 020,044 */ 6, 0x01, 
  /* ABS: 002 Pixels @ 026,044 */ 0, 2, 0x83, 0x38, 
  /* RLE: 004 Pixels @ 028,044 */ 4, 0x02, 
  /* ABS: 002 Pixels @ 032,044 */ 0, 2, 0x10, 0x70, 
  /* RLE: 006 Pixels @ 034,044 */ 6, 0x01, 
  /* RLE: 001 Pixels @ 040,044 */ 1, 0x6C, 
  /* RLE: 012 Pixels @ 041,044 */ 12, 0x02, 
  /* RLE: 001 Pixels @ 053,044 */ 1, 0x0D, 
  /* RLE: 008 Pixels @ 054,044 */ 8, 0x01, 
  /* ABS: 004 Pixels @ 062,044 */ 0, 4, 0x04, 0x02, 0x02, 0x04, 
  /* RLE: 008 Pixels @ 002,045 */ 8, 0x01, 
  /* RLE: 001 Pixels @ 010,045 */ 1, 0x0C, 
  /* RLE: 008 Pixels @ 011,045 */ 8, 0x02, 
  /* ABS: 002 Pixels @ 019,045 */ 0, 2, 0x15, 0x1D, 
  /* RLE: 006 Pixels @ 021,045 */ 6, 0x01, 
  /* ABS: 006 Pixels @ 027,045 */ 0, 6, 0x84, 0x4E, 0x18, 0x3F, 0x44, 0x6F, 
  /* RLE: 007 Pixels @ 033,045 */ 7, 0x01, 
  /* RLE: 001 Pixels @ 040,045 */ 1, 0x65, 
  /* RLE: 012 Pixels @ 041,045 */ 12, 0x02, 
  /* RLE: 001 Pixels @ 053,045 */ 1, 0x0D, 
  /* RLE: 008 Pixels @ 054,045 */ 8, 0x01, 
  /* ABS: 004 Pixels @ 062,045 */ 0, 4, 0x04, 0x02, 0x02, 0x04, 
  /* RLE: 008 Pixels @ 002,046 */ 8, 0x01, 
  /* RLE: 001 Pixels @ 010,046 */ 1, 0x0C, 
  /* RLE: 009 Pixels @ 011,046 */ 9, 0x02, 
  /* RLE: 001 Pixels @ 020,046 */ 1, 0x0B, 
  /* RLE: 018 Pixels @ 021,046 */ 18, 0x01, 
  /* ABS: 002 Pixels @ 039,046 */ 0, 2, 0x99, 0x40, 
  /* RLE: 012 Pixels @ 041,046 */ 12, 0x02, 
  /* RLE: 001 Pixels @ 053,046 */ 1, 0x0D, 
  /* RLE: 008 Pixels @ 054,046 */ 8, 0x01, 
  /* ABS: 004 Pixels @ 062,046 */ 0, 4, 0x04, 0x02, 0x02, 0x04, 
  /* RLE: 008 Pixels @ 002,047 */ 8, 0x01, 
  /* RLE: 001 Pixels @ 010,047 */ 1, 0x0C, 
  /* RLE: 009 Pixels @ 011,047 */ 9, 0x02, 
  /* ABS: 002 Pixels @ 020,047 */ 0, 2, 0x3D, 0x8F, 
  /* RLE: 017 Pixels @ 022,047 */ 17, 0x01, 
  /* RLE: 001 Pixels @ 039,047 */ 1, 0x6D, 
  /* RLE: 013 Pixels @ 040,047 */ 13, 0x02, 
  /* RLE: 001 Pixels @ 053,047 */ 1, 0x0D, 
  /* RLE: 008 Pixels @ 054,047 */ 8, 0x01, 
  /* ABS: 004 Pixels @ 062,047 */ 0, 4, 0x04, 0x02, 0x02, 0x04, 
  /* RLE: 008 Pixels @ 002,048 */ 8, 0x01, 
  /* RLE: 001 Pixels @ 010,048 */ 1, 0x0C, 
  /* RLE: 010 Pixels @ 011,048 */ 10, 0x02, 
  /* ABS: 002 Pixels @ 021,048 */ 0, 2, 0x47, 0x94, 
  /* RLE: 015 Pixels @ 023,048 */ 15, 0x01, 
  /* ABS: 002 Pixels @ 038,048 */ 0, 2, 0x31, 0x3B, 
  /* RLE: 013 Pixels @ 040,048 */ 13, 0x02, 
  /* RLE: 001 Pixels @ 053,048 */ 1, 0x0D, 
  /* RLE: 008 Pixels @ 054,048 */ 8, 0x01, 
  /* ABS: 004 Pixels @ 062,048 */ 0, 4, 0x04, 0x02, 0x02, 0x04, 
  /* RLE: 008 Pixels @ 002,049 */ 8, 0x01, 
  /* RLE: 001 Pixels @ 010,049 */ 1, 0x0C, 
  /* RLE: 011 Pixels @ 011,049 */ 11, 0x02, 
  /* ABS: 002 Pixels @ 022,049 */ 0, 2, 0x18, 0x89, 
  /* RLE: 013 Pixels @ 024,049 */ 13, 0x01, 
  /* ABS: 002 Pixels @ 037,049 */ 0, 2, 0x2F, 0x3C, 
  /* RLE: 014 Pixels @ 039,049 */ 14, 0x02, 
  /* RLE: 001 Pixels @ 053,049 */ 1, 0x0D, 
  /* RLE: 008 Pixels @ 054,049 */ 8, 0x01, 
  /* ABS: 004 Pixels @ 062,049 */ 0, 4, 0x04, 0x02, 0x02, 0x04, 
  /* RLE: 008 Pixels @ 002,050 */ 8, 0x01, 
  /* RLE: 001 Pixels @ 010,050 */ 1, 0x0C, 
  /* RLE: 012 Pixels @ 011,050 */ 12, 0x02, 
  /* ABS: 003 Pixels @ 023,050 */ 0, 3, 0x37, 0x50, 0x31, 
  /* RLE: 008 Pixels @ 026,050 */ 8, 0x01, 
  /* ABS: 004 Pixels @ 034,050 */ 0, 4, 0x97, 0x2F, 0x4B, 0x15, 
  /* RLE: 015 Pixels @ 038,050 */ 15, 0x02, 
  /* RLE: 001 Pixels @ 053,050 */ 1, 0x0D, 
  /* RLE: 008 Pixels @ 054,050 */ 8, 0x01, 
  /* ABS: 004 Pixels @ 062,050 */ 0, 4, 0x04, 0x02, 0x02, 0x04, 
  /* RLE: 008 Pixels @ 002,051 */ 8, 0x01, 
  /* RLE: 001 Pixels @ 010,051 */ 1, 0x0C, 
  /* RLE: 015 Pixels @ 011,051 */ 15, 0x02, 
  /* ABS: 009 Pixels @ 026,051 */ 0, 9, 0x3E, 0x4C, 0x66, 0x2A, 0x68, 0x67, 0x64, 0x21, 0x39, 
  /* RLE: 018 Pixels @ 035,051 */ 18, 0x02, 
  /* RLE: 001 Pixels @ 053,051 */ 1, 0x0D, 
  /* RLE: 008 Pixels @ 054,051 */ 8, 0x01, 
  /* ABS: 004 Pixels @ 062,051 */ 0, 4, 0x04, 0x02, 0x02, 0x04, 
  /* RLE: 008 Pixels @ 002,052 */ 8, 0x01, 
  /* RLE: 001 Pixels @ 010,052 */ 1, 0x0C, 
  /* RLE: 042 Pixels @ 011,052 */ 42, 0x02, 
  /* RLE: 001 Pixels @ 053,052 */ 1, 0x0D, 
  /* RLE: 008 Pixels @ 054,052 */ 8, 0x01, 
  /* ABS: 004 Pixels @ 062,052 */ 0, 4, 0x04, 0x02, 0x02, 0x04, 
  /* RLE: 008 Pixels @ 002,053 */ 8, 0x01, 
  /* RLE: 001 Pixels @ 010,053 */ 1, 0x18, 
  /* RLE: 042 Pixels @ 011,053 */ 42, 0x02, 
  /* RLE: 001 Pixels @ 053,053 */ 1, 0x4A, 
  /* RLE: 008 Pixels @ 054,053 */ 8, 0x01, 
  /* ABS: 004 Pixels @ 062,053 */ 0, 4, 0x04, 0x02, 0x02, 0x04, 
  /* RLE: 008 Pixels @ 002,054 */ 8, 0x01, 
  /* ABS: 002 Pixels @ 010,054 */ 0, 2, 0x6B, 0x35, 
  /* RLE: 040 Pixels @ 012,054 */ 40, 0x02, 
  /* ABS: 002 Pixels @ 052,054 */ 0, 2, 0x36, 0x2E, 
  /* RLE: 008 Pixels @ 054,054 */ 8, 0x01, 
  /* ABS: 004 Pixels @ 062,054 */ 0, 4, 0x04, 0x02, 0x02, 0x04, 
  /* RLE: 009 Pixels @ 002,055 */ 9, 0x01, 
  /* RLE: 001 Pixels @ 011,055 */ 1, 0x88, 
  /* RLE: 039 Pixels @ 012,055 */ 39, 0x07, 
  /* ABS: 002 Pixels @ 051,055 */ 0, 2, 0x2E, 0x8A, 
  /* RLE: 009 Pixels @ 053,055 */ 9, 0x01, 
  /* ABS: 004 Pixels @ 062,055 */ 0, 4, 0x04, 0x02, 0x02, 0x04, 
  /* RLE: 060 Pixels @ 002,056 */ 60, 0x01, 
  /* ABS: 004 Pixels @ 062,056 */ 0, 4, 0x04, 0x02, 0x02, 0x04, 
  /* RLE: 060 Pixels @ 002,057 */ 60, 0x01, 
  /* ABS: 004 Pixels @ 062,057 */ 0, 4, 0x04, 0x02, 0x02, 0x04, 
  /* RLE: 060 Pixels @ 002,058 */ 60, 0x01, 
  /* ABS: 004 Pixels @ 062,058 */ 0, 4, 0x04, 0x02, 0x02, 0x04, 
  /* RLE: 060 Pixels @ 002,059 */ 60, 0x01, 
  /* ABS: 004 Pixels @ 062,059 */ 0, 4, 0x04, 0x02, 0x02, 0x04, 
  /* RLE: 060 Pixels @ 002,060 */ 60, 0x01, 
  /* ABS: 004 Pixels @ 062,060 */ 0, 4, 0x04, 0x02, 0x02, 0x16, 
  /* RLE: 060 Pixels @ 002,061 */ 60, 0x01, 
  /* ABS: 005 Pixels @ 062,061 */ 0, 5, 0x16, 0x02, 0x17, 0x46, 0x32, 
  /* RLE: 058 Pixels @ 003,062 */ 58, 0x01, 
  /* ABS: 005 Pixels @ 061,062 */ 0, 5, 0x32, 0x45, 0x12, 0x14, 0x12, 
  /* RLE: 060 Pixels @ 002,063 */ 60, 0x02, 
  /* ABS: 002 Pixels @ 062,063 */ 0, 2, 0x12, 0x14, 
  0
};  // 1360 bytes for 4096 pixels

GUI_CONST_STORAGE GUI_BITMAP bmIcon_JLink_64x64 = {
  64, // xSize
  64, // ySize
  64, // BytesPerLine
  GUI_COMPRESS_RLE8, // BitsPerPixel
  _acIcon_JLink_64x64,  // Pointer to picture data (indices)
  &_PalIcon_JLink_64x64,  // Pointer to palette
  GUI_DRAW_RLE8
};

/*************************** End of file ****************************/
