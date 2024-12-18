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
* Source file: Icon_emFTP_64x64                                      *
* Dimensions:  64 * 64                                               *
* NumColors:   213                                                   *
---------------------------END-OF-HEADER------------------------------
*/

#include <stdlib.h>

#include "GUI.h"

#ifndef GUI_CONST_STORAGE
  #define GUI_CONST_STORAGE const
#endif

extern GUI_CONST_STORAGE GUI_BITMAP bmIcon_emFTP_64x64;

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
static GUI_CONST_STORAGE GUI_COLOR _ColorsIcon_emFTP_64x64[] = {
#if (GUI_USE_ARGB == 0)
  0x00FFFFFF, 0x00FFFFFF, 0x00A95C00, 0x00292524,
  0x00FAFAFA, 0x00000000, 0x00111111, 0x00999999,
  0x00FDFCFA, 0x00FEFDFC, 0x00C69255, 0x00D7B388,
  0x00FBF7F3, 0x00B26D1A, 0x00BF8642, 0x00CC9F69,
  0x00D3AB7C, 0x00E7D1B7, 0x00D8D8D8, 0x00AB6006,
  0x00B9792E, 0x00DAB991, 0x00EEDECC, 0x00F4EBDF,
  0x00F9F4EE, 0x00C38E4E, 0x00D1A776, 0x00FEFEFD,
  0x00C8975D, 0x00ECDCC7, 0x00FEFCFB, 0x00AB5F05,
  0x00B26D1B, 0x00B67426, 0x00BF8540, 0x00C08744,
  0x00E0C4A2, 0x00EBD9C4, 0x00F0E3D3, 0x00AC6108,
  0x00B47222, 0x00B8792D, 0x00C08743, 0x00CC9E68,
  0x00D3AD7E, 0x00D6B084, 0x00D7B489, 0x00DCBD98,
  0x00E1C6A6, 0x00E4CCB0, 0x00E9D6BF, 0x00EDDDCA,
  0x00F5EBE0, 0x00F8F2EB, 0x00B16B17, 0x00C69457,
  0x00C79458, 0x002C2827, 0x00C69356, 0x00C79559,
  0x00B06915, 0x00B06A16, 0x00F3F3F3, 0x00F5F5F5,
  0x00FDFBF9, 0x00272727, 0x00A95D01, 0x00B06914,
  0x00B16B18, 0x00C38D4D, 0x00C48F50, 0x00C59052,
  0x00C59153, 0x00C7965A, 0x00D7B287, 0x00EEDFCD,
  0x00FCFAF7, 0x00FEFDFB, 0x00030303, 0x00101010,
  0x001D1D1D, 0x001E1E1E, 0x002A2625, 0x002D2D2D,
  0x00332F2E, 0x00AA5F04, 0x00AD640D, 0x00B36F1D,
  0x00B57223, 0x00B77629, 0x00BA7C32, 0x00BD813A,
  0x00C28B49, 0x00C28C4B, 0x00C49051, 0x00C59254,
  0x00C8965B, 0x00CEA36F, 0x00888888, 0x00919191,
  0x009B9B9B, 0x00A5A5A5, 0x00DDBE99, 0x00DBDBDB,
  0x00F2E6D8, 0x00F6EEE4, 0x00F4F4F4, 0x00FAF5F0,
  0x00FBF7F2, 0x00FBF8F4, 0x00FDFCFB, 0x00FFFEFE,
  0x00FFFFFE, 0x002D2928, 0x00312D2C, 0x00342F2E,
  0x003B3737, 0x003E3A3A, 0x003E3E3E, 0x00423F3D,
  0x00433F3F, 0x004B4B4B, 0x004C4C4C, 0x006F6F6F,
  0x00707070, 0x007A7A7A, 0x007C7C7C, 0x00AA5D02,
  0x00AB6007, 0x00AC620A, 0x00AE650E, 0x00AE6610,
  0x00AE6710, 0x00AF6813, 0x00B06B17, 0x00B47020,
  0x00B47121, 0x00B47221, 0x00B67628, 0x00BA7B31,
  0x00BA7D33, 0x00BB7F36, 0x00BB7F37, 0x00BC8038,
  0x00BC8039, 0x00BD823C, 0x00BE833D, 0x00C08945,
  0x00C28B4A, 0x00C38D4E, 0x00C48D4E, 0x00C59051,
  0x00C8965C, 0x00C99960, 0x00CA9B63, 0x00CB9D66,
  0x00CC9E67, 0x00CDA06A, 0x00CDA06B, 0x00CDA16C,
  0x00CEA26E, 0x00D0A775, 0x00D1A877, 0x00D3AA7A,
  0x00D2AB7B, 0x00D4AD7F, 0x00868484, 0x00898786,
  0x008C8A89, 0x008D8B8A, 0x00D4AE80, 0x00D5AE81,
  0x00D7B48A, 0x00D8B48A, 0x00D8B58B, 0x00DBBA93,
  0x00DBBB94, 0x00DBBB95, 0x00DEC09D, 0x00DFC3A1,
  0x00E0C5A4, 0x00E4CCAF, 0x00E5CDB1, 0x00E5CEB2,
  0x00E6D0B5, 0x00E7D2B8, 0x00E9D5BD, 0x00D9D9D9,
  0x00EAD7C0, 0x00EAD7C1, 0x00EAD8C1, 0x00EBDAC4,
  0x00EBDAC5, 0x00ECDBC6, 0x00EDDCC9, 0x00F1E3D4,
  0x00F1E4D4, 0x00F2E7DA, 0x00F4E9DD, 0x00EDEDED,
  0x00EEEEEE, 0x00F5ECE0, 0x00F5ECE2, 0x00F7F0E8,
  0x00F7F1E9, 0x00F8F1E9, 0x00F9F3ED, 0x00FAF5EF,
  0x00FBF6F2, 0x00FBF9F5, 0x00FCF9F6, 0x00FCFAF6,
  0x00FCFAF8
#else
  0xFFFFFFFF, 0xFFFFFFFF, 0xFF005CA9, 0xFF242529,
  0xFFFAFAFA, 0xFF000000, 0xFF111111, 0xFF999999,
  0xFFFAFCFD, 0xFFFCFDFE, 0xFF5592C6, 0xFF88B3D7,
  0xFFF3F7FB, 0xFF1A6DB2, 0xFF4286BF, 0xFF699FCC,
  0xFF7CABD3, 0xFFB7D1E7, 0xFFD8D8D8, 0xFF0660AB,
  0xFF2E79B9, 0xFF91B9DA, 0xFFCCDEEE, 0xFFDFEBF4,
  0xFFEEF4F9, 0xFF4E8EC3, 0xFF76A7D1, 0xFFFDFEFE,
  0xFF5D97C8, 0xFFC7DCEC, 0xFFFBFCFE, 0xFF055FAB,
  0xFF1B6DB2, 0xFF2674B6, 0xFF4085BF, 0xFF4487C0,
  0xFFA2C4E0, 0xFFC4D9EB, 0xFFD3E3F0, 0xFF0861AC,
  0xFF2272B4, 0xFF2D79B8, 0xFF4387C0, 0xFF689ECC,
  0xFF7EADD3, 0xFF84B0D6, 0xFF89B4D7, 0xFF98BDDC,
  0xFFA6C6E1, 0xFFB0CCE4, 0xFFBFD6E9, 0xFFCADDED,
  0xFFE0EBF5, 0xFFEBF2F8, 0xFF176BB1, 0xFF5794C6,
  0xFF5894C7, 0xFF27282C, 0xFF5693C6, 0xFF5995C7,
  0xFF1569B0, 0xFF166AB0, 0xFFF3F3F3, 0xFFF5F5F5,
  0xFFF9FBFD, 0xFF272727, 0xFF015DA9, 0xFF1469B0,
  0xFF186BB1, 0xFF4D8DC3, 0xFF508FC4, 0xFF5290C5,
  0xFF5391C5, 0xFF5A96C7, 0xFF87B2D7, 0xFFCDDFEE,
  0xFFF7FAFC, 0xFFFBFDFE, 0xFF030303, 0xFF101010,
  0xFF1D1D1D, 0xFF1E1E1E, 0xFF25262A, 0xFF2D2D2D,
  0xFF2E2F33, 0xFF045FAA, 0xFF0D64AD, 0xFF1D6FB3,
  0xFF2372B5, 0xFF2976B7, 0xFF327CBA, 0xFF3A81BD,
  0xFF498BC2, 0xFF4B8CC2, 0xFF5190C4, 0xFF5492C5,
  0xFF5B96C8, 0xFF6FA3CE, 0xFF888888, 0xFF919191,
  0xFF9B9B9B, 0xFFA5A5A5, 0xFF99BEDD, 0xFFDBDBDB,
  0xFFD8E6F2, 0xFFE4EEF6, 0xFFF4F4F4, 0xFFF0F5FA,
  0xFFF2F7FB, 0xFFF4F8FB, 0xFFFBFCFD, 0xFFFEFEFF,
  0xFFFEFFFF, 0xFF28292D, 0xFF2C2D31, 0xFF2E2F34,
  0xFF37373B, 0xFF3A3A3E, 0xFF3E3E3E, 0xFF3D3F42,
  0xFF3F3F43, 0xFF4B4B4B, 0xFF4C4C4C, 0xFF6F6F6F,
  0xFF707070, 0xFF7A7A7A, 0xFF7C7C7C, 0xFF025DAA,
  0xFF0760AB, 0xFF0A62AC, 0xFF0E65AE, 0xFF1066AE,
  0xFF1067AE, 0xFF1368AF, 0xFF176BB0, 0xFF2070B4,
  0xFF2171B4, 0xFF2172B4, 0xFF2876B6, 0xFF317BBA,
  0xFF337DBA, 0xFF367FBB, 0xFF377FBB, 0xFF3880BC,
  0xFF3980BC, 0xFF3C82BD, 0xFF3D83BE, 0xFF4589C0,
  0xFF4A8BC2, 0xFF4E8DC3, 0xFF4E8DC4, 0xFF5190C5,
  0xFF5C96C8, 0xFF6099C9, 0xFF639BCA, 0xFF669DCB,
  0xFF679ECC, 0xFF6AA0CD, 0xFF6BA0CD, 0xFF6CA1CD,
  0xFF6EA2CE, 0xFF75A7D0, 0xFF77A8D1, 0xFF7AAAD3,
  0xFF7BABD2, 0xFF7FADD4, 0xFF848486, 0xFF868789,
  0xFF898A8C, 0xFF8A8B8D, 0xFF80AED4, 0xFF81AED5,
  0xFF8AB4D7, 0xFF8AB4D8, 0xFF8BB5D8, 0xFF93BADB,
  0xFF94BBDB, 0xFF95BBDB, 0xFF9DC0DE, 0xFFA1C3DF,
  0xFFA4C5E0, 0xFFAFCCE4, 0xFFB1CDE5, 0xFFB2CEE5,
  0xFFB5D0E6, 0xFFB8D2E7, 0xFFBDD5E9, 0xFFD9D9D9,
  0xFFC0D7EA, 0xFFC1D7EA, 0xFFC1D8EA, 0xFFC4DAEB,
  0xFFC5DAEB, 0xFFC6DBEC, 0xFFC9DCED, 0xFFD4E3F1,
  0xFFD4E4F1, 0xFFDAE7F2, 0xFFDDE9F4, 0xFFEDEDED,
  0xFFEEEEEE, 0xFFE0ECF5, 0xFFE2ECF5, 0xFFE8F0F7,
  0xFFE9F1F7, 0xFFE9F1F8, 0xFFEDF3F9, 0xFFEFF5FA,
  0xFFF2F6FB, 0xFFF5F9FB, 0xFFF6F9FC, 0xFFF6FAFC,
  0xFFF8FAFC
#endif

};

static GUI_CONST_STORAGE GUI_LOGPALETTE _PalIcon_emFTP_64x64 = {
  213,  // Number of entries
  1,    // Has transparency
  &_ColorsIcon_emFTP_64x64[0]
};

static GUI_CONST_STORAGE unsigned char _acIcon_emFTP_64x64[] = {
  /* ABS: 003 Pixels @ 000,000 */ 0, 3, 0x76, 0x78, 0x39, 
  /* RLE: 058 Pixels @ 003,000 */ 58, 0x03, 
  /* ABS: 006 Pixels @ 061,000 */ 0, 6, 0x39, 0x77, 0x41, 0x75, 0xA9, 0x3F, 
  /* RLE: 058 Pixels @ 003,001 */ 58, 0x01, 
  /* ABS: 005 Pixels @ 061,001 */ 0, 5, 0x3F, 0xA8, 0x54, 0x39, 0x3E, 
  /* RLE: 060 Pixels @ 002,002 */ 60, 0x01, 
  /* ABS: 004 Pixels @ 062,002 */ 0, 4, 0x3E, 0x52, 0x03, 0x04, 
  /* RLE: 060 Pixels @ 002,003 */ 60, 0x01, 
  /* ABS: 004 Pixels @ 062,003 */ 0, 4, 0x04, 0x03, 0x03, 0x04, 
  /* RLE: 060 Pixels @ 002,004 */ 60, 0x01, 
  /* ABS: 004 Pixels @ 062,004 */ 0, 4, 0x04, 0x03, 0x03, 0x04, 
  /* RLE: 060 Pixels @ 002,005 */ 60, 0x01, 
  /* ABS: 004 Pixels @ 062,005 */ 0, 4, 0x04, 0x03, 0x03, 0x04, 
  /* RLE: 009 Pixels @ 002,006 */ 9, 0x01, 
  /* RLE: 001 Pixels @ 011,006 */ 1, 0x1B, 
  /* RLE: 018 Pixels @ 012,006 */ 18, 0x08, 
  /* RLE: 001 Pixels @ 030,006 */ 1, 0x09, 
  /* RLE: 031 Pixels @ 031,006 */ 31, 0x01, 
  /* ABS: 004 Pixels @ 062,006 */ 0, 4, 0x04, 0x03, 0x03, 0x04, 
  /* RLE: 009 Pixels @ 002,007 */ 9, 0x01, 
  /* ABS: 002 Pixels @ 011,007 */ 0, 2, 0x08, 0xB7, 
  /* RLE: 017 Pixels @ 013,007 */ 17, 0x0B, 
  /* RLE: 001 Pixels @ 030,007 */ 1, 0x09, 
  /* RLE: 031 Pixels @ 031,007 */ 31, 0x01, 
  /* ABS: 004 Pixels @ 062,007 */ 0, 4, 0x04, 0x03, 0x03, 0x04, 
  /* RLE: 009 Pixels @ 002,008 */ 9, 0x01, 
  /* ABS: 002 Pixels @ 011,008 */ 0, 2, 0x08, 0x3B, 
  /* RLE: 017 Pixels @ 013,008 */ 17, 0x02, 
  /* RLE: 001 Pixels @ 030,008 */ 1, 0x08, 
  /* RLE: 031 Pixels @ 031,008 */ 31, 0x01, 
  /* ABS: 004 Pixels @ 062,008 */ 0, 4, 0x04, 0x03, 0x03, 0x04, 
  /* RLE: 009 Pixels @ 002,009 */ 9, 0x01, 
  /* ABS: 002 Pixels @ 011,009 */ 0, 2, 0x08, 0x3B, 
  /* RLE: 017 Pixels @ 013,009 */ 17, 0x02, 
  /* RLE: 001 Pixels @ 030,009 */ 1, 0x08, 
  /* RLE: 031 Pixels @ 031,009 */ 31, 0x01, 
  /* ABS: 004 Pixels @ 062,009 */ 0, 4, 0x04, 0x03, 0x03, 0x04, 
  /* RLE: 009 Pixels @ 002,010 */ 9, 0x01, 
  /* ABS: 003 Pixels @ 011,010 */ 0, 3, 0x08, 0x38, 0x56, 
  /* RLE: 014 Pixels @ 014,010 */ 14, 0x0A, 
  /* ABS: 003 Pixels @ 028,010 */ 0, 3, 0x5D, 0x02, 0x1E, 
  /* RLE: 031 Pixels @ 031,010 */ 31, 0x01, 
  /* ABS: 004 Pixels @ 062,010 */ 0, 4, 0x04, 0x03, 0x03, 0x04, 
  /* RLE: 009 Pixels @ 002,011 */ 9, 0x01, 
  /* ABS: 003 Pixels @ 011,011 */ 0, 3, 0x6E, 0x38, 0x21, 
  /* RLE: 014 Pixels @ 014,011 */ 14, 0x0C, 
  /* ABS: 004 Pixels @ 028,011 */ 0, 4, 0x68, 0x02, 0x1E, 0xC9, 
  /* RLE: 009 Pixels @ 032,011 */ 9, 0x26, 
  /* ABS: 002 Pixels @ 041,011 */ 0, 2, 0xCA, 0x09, 
  /* RLE: 019 Pixels @ 043,011 */ 19, 0x01, 
  /* ABS: 004 Pixels @ 062,011 */ 0, 4, 0x04, 0x03, 0x03, 0x04, 
  /* RLE: 009 Pixels @ 002,012 */ 9, 0x01, 
  /* ABS: 002 Pixels @ 011,012 */ 0, 2, 0x6E, 0x37, 
  /* RLE: 017 Pixels @ 013,012 */ 17, 0x02, 
  /* ABS: 002 Pixels @ 030,012 */ 0, 2, 0x40, 0x19, 
  /* RLE: 009 Pixels @ 032,012 */ 9, 0x02, 
  /* ABS: 002 Pixels @ 041,012 */ 0, 2, 0x3A, 0x1E, 
  /* RLE: 019 Pixels @ 043,012 */ 19, 0x01, 
  /* ABS: 004 Pixels @ 062,012 */ 0, 4, 0x04, 0x03, 0x03, 0x04, 
  /* RLE: 009 Pixels @ 002,013 */ 9, 0x01, 
  /* ABS: 003 Pixels @ 011,013 */ 0, 3, 0x1E, 0x37, 0x57, 
  /* RLE: 014 Pixels @ 014,013 */ 14, 0x11, 
  /* ABS: 004 Pixels @ 028,013 */ 0, 4, 0x24, 0x02, 0x08, 0x45, 
  /* RLE: 009 Pixels @ 032,013 */ 9, 0x02, 
  /* ABS: 002 Pixels @ 041,013 */ 0, 2, 0x3A, 0x1E, 
  /* RLE: 019 Pixels @ 043,013 */ 19, 0x01, 
  /* ABS: 004 Pixels @ 062,013 */ 0, 4, 0x04, 0x03, 0x03, 0x04, 
  /* RLE: 009 Pixels @ 002,014 */ 9, 0x01, 
  /* ABS: 003 Pixels @ 011,014 */ 0, 3, 0x1E, 0x3A, 0x36, 
  /* RLE: 014 Pixels @ 014,014 */ 14, 0x15, 
  /* ABS: 004 Pixels @ 028,014 */ 0, 4, 0xAB, 0x02, 0x08, 0x99, 
  /* RLE: 008 Pixels @ 032,014 */ 8, 0x20, 
  /* ABS: 003 Pixels @ 040,014 */ 0, 3, 0x82, 0x3A, 0x08, 
  /* RLE: 019 Pixels @ 043,014 */ 19, 0x01, 
  /* ABS: 004 Pixels @ 062,014 */ 0, 4, 0x04, 0x03, 0x03, 0x04, 
  /* RLE: 009 Pixels @ 002,015 */ 9, 0x01, 
  /* ABS: 003 Pixels @ 011,015 */ 0, 3, 0x1E, 0x3A, 0x55, 
  /* RLE: 014 Pixels @ 014,015 */ 14, 0x0D, 
  /* ABS: 003 Pixels @ 028,015 */ 0, 3, 0x36, 0x02, 0x09, 
  /* RLE: 009 Pixels @ 031,015 */ 9, 0x01, 
  /* ABS: 003 Pixels @ 040,015 */ 0, 3, 0x4A, 0x37, 0x08, 
  /* RLE: 019 Pixels @ 043,015 */ 19, 0x01, 
  /* ABS: 004 Pixels @ 062,015 */ 0, 4, 0x04, 0x03, 0x03, 0x04, 
  /* RLE: 009 Pixels @ 002,016 */ 9, 0x01, 
  /* ABS: 003 Pixels @ 011,016 */ 0, 3, 0x1E, 0x0A, 0x59, 
  /* RLE: 014 Pixels @ 014,016 */ 14, 0x01, 
  /* ABS: 004 Pixels @ 028,016 */ 0, 4, 0x69, 0x02, 0x4D, 0x9F, 
  /* RLE: 008 Pixels @ 032,016 */ 8, 0x29, 
  /* ABS: 003 Pixels @ 040,016 */ 0, 3, 0x36, 0x37, 0x0C, 
  /* RLE: 009 Pixels @ 043,016 */ 9, 0x23, 
  /* RLE: 001 Pixels @ 052,016 */ 1, 0x47, 
  /* RLE: 009 Pixels @ 053,016 */ 9, 0x01, 
  /* ABS: 004 Pixels @ 062,016 */ 0, 4, 0x04, 0x03, 0x03, 0x04, 
  /* RLE: 009 Pixels @ 002,017 */ 9, 0x01, 
  /* ABS: 003 Pixels @ 011,017 */ 0, 3, 0x4D, 0x5F, 0x80, 
  /* RLE: 014 Pixels @ 014,017 */ 14, 0x14, 
  /* ABS: 004 Pixels @ 028,017 */ 0, 4, 0x8A, 0x02, 0x09, 0xB4, 
  /* RLE: 008 Pixels @ 032,017 */ 8, 0x2C, 
  /* ABS: 003 Pixels @ 040,017 */ 0, 3, 0x0E, 0x37, 0x6B, 
  /* RLE: 009 Pixels @ 043,017 */ 9, 0x02, 
  /* RLE: 001 Pixels @ 052,017 */ 1, 0x43, 
  /* RLE: 009 Pixels @ 053,017 */ 9, 0x01, 
  /* ABS: 004 Pixels @ 062,017 */ 0, 4, 0x04, 0x03, 0x03, 0x04, 
  /* RLE: 009 Pixels @ 002,018 */ 9, 0x01, 
  /* ABS: 003 Pixels @ 011,018 */ 0, 3, 0x4D, 0x5F, 0x85, 
  /* RLE: 014 Pixels @ 014,018 */ 14, 0x10, 
  /* ABS: 004 Pixels @ 028,018 */ 0, 4, 0xA0, 0x02, 0x09, 0xC5, 
  /* RLE: 008 Pixels @ 032,018 */ 8, 0x33, 
  /* ABS: 003 Pixels @ 040,018 */ 0, 3, 0x9D, 0x38, 0xCF, 
  /* RLE: 009 Pixels @ 043,018 */ 9, 0x02, 
  /* RLE: 001 Pixels @ 052,018 */ 1, 0x43, 
  /* RLE: 009 Pixels @ 053,018 */ 9, 0x01, 
  /* ABS: 004 Pixels @ 062,018 */ 0, 4, 0x04, 0x03, 0x03, 0x04, 
  /* RLE: 009 Pixels @ 002,019 */ 9, 0x01, 
  /* ABS: 003 Pixels @ 011,019 */ 0, 3, 0x09, 0x48, 0x87, 
  /* RLE: 014 Pixels @ 014,019 */ 14, 0x16, 
  /* ABS: 004 Pixels @ 028,019 */ 0, 4, 0xB8, 0x02, 0x1E, 0x5D, 
  /* RLE: 009 Pixels @ 032,019 */ 9, 0x02, 
  /* ABS: 002 Pixels @ 041,019 */ 0, 2, 0x38, 0xD2, 
  /* RLE: 008 Pixels @ 043,019 */ 8, 0x2F, 
  /* ABS: 002 Pixels @ 051,019 */ 0, 2, 0xA2, 0x43, 
  /* RLE: 009 Pixels @ 053,019 */ 9, 0x01, 
  /* ABS: 004 Pixels @ 062,019 */ 0, 4, 0x04, 0x03, 0x03, 0x04, 
  /* RLE: 009 Pixels @ 002,020 */ 9, 0x01, 
  /* ABS: 002 Pixels @ 011,020 */ 0, 2, 0x09, 0x48, 
  /* RLE: 017 Pixels @ 013,020 */ 17, 0x02, 
  /* ABS: 002 Pixels @ 030,020 */ 0, 2, 0x1B, 0xCD, 
  /* RLE: 008 Pixels @ 032,020 */ 8, 0x34, 
  /* ABS: 003 Pixels @ 040,020 */ 0, 3, 0x1A, 0x38, 0x4C, 
  /* RLE: 008 Pixels @ 043,020 */ 8, 0x31, 
  /* ABS: 002 Pixels @ 051,020 */ 0, 2, 0xAD, 0x3C, 
  /* RLE: 009 Pixels @ 053,020 */ 9, 0x01, 
  /* ABS: 004 Pixels @ 062,020 */ 0, 4, 0x04, 0x03, 0x03, 0x04, 
  /* RLE: 009 Pixels @ 002,021 */ 9, 0x01, 
  /* ABS: 003 Pixels @ 011,021 */ 0, 3, 0x09, 0x47, 0x58, 
  /* RLE: 014 Pixels @ 014,021 */ 14, 0x17, 
  /* ABS: 004 Pixels @ 028,021 */ 0, 4, 0xC1, 0x02, 0x09, 0xB0, 
  /* RLE: 008 Pixels @ 032,021 */ 8, 0x2B, 
  /* ABS: 003 Pixels @ 040,021 */ 0, 3, 0x8D, 0x38, 0x18, 
  /* RLE: 009 Pixels @ 043,021 */ 9, 0x02, 
  /* RLE: 001 Pixels @ 052,021 */ 1, 0x3C, 
  /* RLE: 009 Pixels @ 053,021 */ 9, 0x01, 
  /* ABS: 004 Pixels @ 062,021 */ 0, 4, 0x04, 0x03, 0x03, 0x04, 
  /* RLE: 009 Pixels @ 002,022 */ 9, 0x01, 
  /* ABS: 003 Pixels @ 011,022 */ 0, 3, 0x09, 0x47, 0x83, 
  /* RLE: 014 Pixels @ 014,022 */ 14, 0x0F, 
  /* ABS: 004 Pixels @ 028,022 */ 0, 4, 0x1C, 0x02, 0x09, 0xA3, 
  /* RLE: 008 Pixels @ 032,022 */ 8, 0x2A, 
  /* ABS: 002 Pixels @ 040,022 */ 0, 2, 0x58, 0x3B, 
  /* RLE: 009 Pixels @ 042,022 */ 9, 0x08, 
  /* ABS: 002 Pixels @ 051,022 */ 0, 2, 0x25, 0x3C, 
  /* RLE: 009 Pixels @ 053,022 */ 9, 0x01, 
  /* ABS: 004 Pixels @ 062,022 */ 0, 4, 0x04, 0x03, 0x03, 0x04, 
  /* RLE: 009 Pixels @ 002,023 */ 9, 0x01, 
  /* ABS: 003 Pixels @ 011,023 */ 0, 3, 0x09, 0x5E, 0x81, 
  /* RLE: 014 Pixels @ 014,023 */ 14, 0x0E, 
  /* ABS: 003 Pixels @ 028,023 */ 0, 3, 0x5B, 0x02, 0x6F, 
  /* RLE: 009 Pixels @ 031,023 */ 9, 0x01, 
  /* ABS: 003 Pixels @ 040,023 */ 0, 3, 0x4A, 0x3B, 0xD0, 
  /* RLE: 008 Pixels @ 043,023 */ 8, 0x19, 
  /* ABS: 002 Pixels @ 051,023 */ 0, 2, 0x92, 0x3C, 
  /* RLE: 009 Pixels @ 053,023 */ 9, 0x01, 
  /* ABS: 004 Pixels @ 062,023 */ 0, 4, 0x04, 0x03, 0x03, 0x04, 
  /* RLE: 009 Pixels @ 002,024 */ 9, 0x01, 
  /* ABS: 003 Pixels @ 011,024 */ 0, 3, 0x09, 0x5E, 0x59, 
  /* RLE: 014 Pixels @ 014,024 */ 14, 0x01, 
  /* ABS: 004 Pixels @ 028,024 */ 0, 4, 0x69, 0x02, 0x1B, 0x95, 
  /* RLE: 008 Pixels @ 032,024 */ 8, 0x1F, 
  /* ABS: 003 Pixels @ 040,024 */ 0, 3, 0x7F, 0x3B, 0x6C, 
  /* RLE: 008 Pixels @ 043,024 */ 8, 0x1C, 
  /* ABS: 002 Pixels @ 051,024 */ 0, 2, 0x5C, 0x3D, 
  /* RLE: 009 Pixels @ 053,024 */ 9, 0x01, 
  /* ABS: 004 Pixels @ 062,024 */ 0, 4, 0x04, 0x03, 0x03, 0x04, 
  /* RLE: 009 Pixels @ 002,025 */ 9, 0x01, 
  /* ABS: 003 Pixels @ 011,025 */ 0, 3, 0x09, 0x46, 0x02, 
  /* RLE: 014 Pixels @ 014,025 */ 14, 0x13, 
  /* ABS: 004 Pixels @ 028,025 */ 0, 4, 0x1F, 0x02, 0x1B, 0xBD, 
  /* RLE: 008 Pixels @ 032,025 */ 8, 0x30, 
  /* ABS: 003 Pixels @ 040,025 */ 0, 3, 0x37, 0x49, 0x40, 
  /* RLE: 008 Pixels @ 043,025 */ 8, 0x35, 
  /* ABS: 002 Pixels @ 051,025 */ 0, 2, 0xB9, 0x3D, 
  /* RLE: 009 Pixels @ 053,025 */ 9, 0x01, 
  /* ABS: 004 Pixels @ 062,025 */ 0, 4, 0x04, 0x03, 0x03, 0x04, 
  /* RLE: 009 Pixels @ 002,026 */ 9, 0x01, 
  /* ABS: 002 Pixels @ 011,026 */ 0, 2, 0x09, 0x46, 
  /* RLE: 016 Pixels @ 013,026 */ 16, 0x02, 
  /* ABS: 003 Pixels @ 029,026 */ 0, 3, 0x42, 0x70, 0xBF, 
  /* RLE: 008 Pixels @ 032,026 */ 8, 0x24, 
  /* ABS: 003 Pixels @ 040,026 */ 0, 3, 0x0A, 0x49, 0xCE, 
  /* RLE: 009 Pixels @ 043,026 */ 9, 0x02, 
  /* RLE: 001 Pixels @ 052,026 */ 1, 0x3D, 
  /* RLE: 009 Pixels @ 053,026 */ 9, 0x01, 
  /* ABS: 004 Pixels @ 062,026 */ 0, 4, 0x04, 0x03, 0x03, 0x04, 
  /* RLE: 009 Pixels @ 002,027 */ 9, 0x01, 
  /* ABS: 002 Pixels @ 011,027 */ 0, 2, 0x09, 0x46, 
  /* RLE: 012 Pixels @ 013,027 */ 12, 0x02, 
  /* ABS: 007 Pixels @ 025,027 */ 0, 7, 0x19, 0xB6, 0xAE, 0x10, 0xB3, 0x1B, 0x97, 
  /* RLE: 008 Pixels @ 032,027 */ 8, 0x27, 
  /* ABS: 003 Pixels @ 040,027 */ 0, 3, 0x55, 0x49, 0x4C, 
  /* RLE: 008 Pixels @ 043,027 */ 8, 0x32, 
  /* ABS: 002 Pixels @ 051,027 */ 0, 2, 0xB1, 0x3D, 
  /* RLE: 009 Pixels @ 053,027 */ 9, 0x01, 
  /* ABS: 004 Pixels @ 062,027 */ 0, 4, 0x04, 0x03, 0x03, 0x04, 
  /* RLE: 009 Pixels @ 002,028 */ 9, 0x01, 
  /* ABS: 002 Pixels @ 011,028 */ 0, 2, 0x1B, 0x19, 
  /* RLE: 012 Pixels @ 013,028 */ 12, 0x02, 
  /* RLE: 001 Pixels @ 025,028 */ 1, 0x91, 
  /* RLE: 014 Pixels @ 026,028 */ 14, 0x01, 
  /* ABS: 003 Pixels @ 040,028 */ 0, 3, 0x4A, 0x60, 0x0C, 
  /* RLE: 008 Pixels @ 043,028 */ 8, 0x2E, 
  /* ABS: 002 Pixels @ 051,028 */ 0, 2, 0x9E, 0x36, 
  /* RLE: 009 Pixels @ 053,028 */ 9, 0x01, 
  /* ABS: 004 Pixels @ 062,028 */ 0, 4, 0x04, 0x03, 0x03, 0x04, 
  /* RLE: 009 Pixels @ 002,029 */ 9, 0x01, 
  /* ABS: 002 Pixels @ 011,029 */ 0, 2, 0x1B, 0x19, 
  /* RLE: 012 Pixels @ 013,029 */ 12, 0x02, 
  /* ABS: 006 Pixels @ 025,029 */ 0, 6, 0x5B, 0x01, 0x01, 0x01, 0x70, 0xAA, 
  /* RLE: 009 Pixels @ 031,029 */ 9, 0x22, 
  /* ABS: 003 Pixels @ 040,029 */ 0, 3, 0x88, 0x60, 0x18, 
  /* RLE: 008 Pixels @ 043,029 */ 8, 0x28, 
  /* ABS: 002 Pixels @ 051,029 */ 0, 2, 0x0D, 0x36, 
  /* RLE: 009 Pixels @ 053,029 */ 9, 0x01, 
  /* ABS: 004 Pixels @ 062,029 */ 0, 4, 0x04, 0x03, 0x03, 0x04, 
  /* RLE: 009 Pixels @ 002,030 */ 9, 0x01, 
  /* ABS: 002 Pixels @ 011,030 */ 0, 2, 0x1B, 0x45, 
  /* RLE: 012 Pixels @ 013,030 */ 12, 0x02, 
  /* ABS: 005 Pixels @ 025,030 */ 0, 5, 0x0F, 0x01, 0x01, 0x01, 0xA5, 
  /* RLE: 011 Pixels @ 030,030 */ 11, 0x02, 
  /* ABS: 002 Pixels @ 041,030 */ 0, 2, 0x98, 0x09, 
  /* RLE: 008 Pixels @ 043,030 */ 8, 0x01, 
  /* ABS: 002 Pixels @ 051,030 */ 0, 2, 0xC2, 0x36, 
  /* RLE: 009 Pixels @ 053,030 */ 9, 0x01, 
  /* ABS: 004 Pixels @ 062,030 */ 0, 4, 0x04, 0x03, 0x03, 0x04, 
  /* RLE: 009 Pixels @ 002,031 */ 9, 0x01, 
  /* ABS: 002 Pixels @ 011,031 */ 0, 2, 0x1B, 0x45, 
  /* RLE: 011 Pixels @ 013,031 */ 11, 0x02, 
  /* ABS: 006 Pixels @ 024,031 */ 0, 6, 0x86, 0xC4, 0x01, 0x1E, 0xA1, 0x42, 
  /* RLE: 007 Pixels @ 030,031 */ 7, 0x02, 
  /* ABS: 006 Pixels @ 037,031 */ 0, 6, 0x61, 0x9B, 0x94, 0x5C, 0xB5, 0x6C, 
  /* RLE: 008 Pixels @ 043,031 */ 8, 0x21, 
  /* ABS: 002 Pixels @ 051,031 */ 0, 2, 0x57, 0x36, 
  /* RLE: 009 Pixels @ 053,031 */ 9, 0x01, 
  /* ABS: 004 Pixels @ 062,031 */ 0, 4, 0x04, 0x03, 0x03, 0x04, 
  /* RLE: 009 Pixels @ 002,032 */ 9, 0x01, 
  /* ABS: 002 Pixels @ 011,032 */ 0, 2, 0x09, 0x68, 
  /* RLE: 010 Pixels @ 013,032 */ 10, 0x1D, 
  /* ABS: 005 Pixels @ 023,032 */ 0, 5, 0xC0, 0xBE, 0x6F, 0xBC, 0x8F, 
  /* RLE: 009 Pixels @ 028,032 */ 9, 0x02, 
  /* RLE: 001 Pixels @ 037,032 */ 1, 0xB2, 
  /* RLE: 004 Pixels @ 038,032 */ 4, 0x01, 
  /* RLE: 001 Pixels @ 042,032 */ 1, 0xC3, 
  /* RLE: 008 Pixels @ 043,032 */ 8, 0x2D, 
  /* ABS: 002 Pixels @ 051,032 */ 0, 2, 0x9C, 0x44, 
  /* RLE: 009 Pixels @ 053,032 */ 9, 0x01, 
  /* ABS: 004 Pixels @ 062,032 */ 0, 4, 0x04, 0x03, 0x03, 0x04, 
  /* RLE: 021 Pixels @ 002,033 */ 21, 0x01, 
  /* ABS: 003 Pixels @ 023,033 */ 0, 3, 0xD3, 0x8E, 0x84, 
  /* RLE: 011 Pixels @ 026,033 */ 11, 0x02, 
  /* ABS: 006 Pixels @ 037,033 */ 0, 6, 0xAF, 0x01, 0x01, 0x01, 0x1B, 0x4B, 
  /* RLE: 008 Pixels @ 043,033 */ 8, 0x25, 
  /* ABS: 002 Pixels @ 051,033 */ 0, 2, 0x66, 0x44, 
  /* RLE: 009 Pixels @ 053,033 */ 9, 0x01, 
  /* ABS: 004 Pixels @ 062,033 */ 0, 4, 0x04, 0x03, 0x03, 0x04, 
  /* RLE: 021 Pixels @ 002,034 */ 21, 0x01, 
  /* RLE: 001 Pixels @ 023,034 */ 1, 0x6D, 
  /* RLE: 013 Pixels @ 024,034 */ 13, 0x02, 
  /* ABS: 005 Pixels @ 037,034 */ 0, 5, 0x11, 0x01, 0x01, 0x1B, 0x0A, 
  /* RLE: 010 Pixels @ 042,034 */ 10, 0x02, 
  /* RLE: 001 Pixels @ 052,034 */ 1, 0x44, 
  /* RLE: 009 Pixels @ 053,034 */ 9, 0x01, 
  /* ABS: 004 Pixels @ 062,034 */ 0, 4, 0x04, 0x03, 0x03, 0x04, 
  /* RLE: 021 Pixels @ 002,035 */ 21, 0x01, 
  /* RLE: 001 Pixels @ 023,035 */ 1, 0x6D, 
  /* RLE: 012 Pixels @ 024,035 */ 12, 0x02, 
  /* ABS: 005 Pixels @ 036,035 */ 0, 5, 0x20, 0x40, 0x01, 0x4C, 0x9A, 
  /* RLE: 007 Pixels @ 041,035 */ 7, 0x02, 
  /* ABS: 005 Pixels @ 048,035 */ 0, 5, 0x56, 0x02, 0x02, 0x02, 0x89, 
  /* RLE: 009 Pixels @ 053,035 */ 9, 0x01, 
  /* ABS: 004 Pixels @ 062,035 */ 0, 4, 0x04, 0x03, 0x03, 0x04, 
  /* RLE: 021 Pixels @ 002,036 */ 21, 0x01, 
  /* RLE: 001 Pixels @ 023,036 */ 1, 0x40, 
  /* RLE: 011 Pixels @ 024,036 */ 11, 0x1A, 
  /* ABS: 005 Pixels @ 035,036 */ 0, 5, 0x96, 0xC6, 0x01, 0x4B, 0x8B, 
  /* RLE: 008 Pixels @ 040,036 */ 8, 0x02, 
  /* ABS: 005 Pixels @ 048,036 */ 0, 5, 0x61, 0x18, 0x4B, 0xBA, 0xCB, 
  /* RLE: 009 Pixels @ 053,036 */ 9, 0x01, 
  /* ABS: 004 Pixels @ 062,036 */ 0, 4, 0x04, 0x03, 0x03, 0x04, 
  /* RLE: 033 Pixels @ 002,037 */ 33, 0x01, 
  /* ABS: 004 Pixels @ 035,037 */ 0, 4, 0xA4, 0xAC, 0x90, 0x42, 
  /* RLE: 009 Pixels @ 039,037 */ 9, 0x02, 
  /* RLE: 001 Pixels @ 048,037 */ 1, 0x48, 
  /* RLE: 013 Pixels @ 049,037 */ 13, 0x01, 
  /* ABS: 004 Pixels @ 062,037 */ 0, 4, 0x04, 0x03, 0x03, 0x04, 
  /* RLE: 028 Pixels @ 002,038 */ 28, 0x01, 
  /* ABS: 006 Pixels @ 030,038 */ 0, 6, 0xC7, 0x62, 0x62, 0xC8, 0x01, 0x8C, 
  /* RLE: 012 Pixels @ 036,038 */ 12, 0x02, 
  /* RLE: 001 Pixels @ 048,038 */ 1, 0x1C, 
  /* RLE: 013 Pixels @ 049,038 */ 13, 0x01, 
  /* ABS: 004 Pixels @ 062,038 */ 0, 4, 0x04, 0x03, 0x03, 0x04, 
  /* RLE: 028 Pixels @ 002,039 */ 28, 0x01, 
  /* ABS: 006 Pixels @ 030,039 */ 0, 6, 0x12, 0x05, 0x05, 0x67, 0x01, 0x5A, 
  /* RLE: 012 Pixels @ 036,039 */ 12, 0x02, 
  /* RLE: 001 Pixels @ 048,039 */ 1, 0x66, 
  /* RLE: 013 Pixels @ 049,039 */ 13, 0x01, 
  /* ABS: 004 Pixels @ 062,039 */ 0, 4, 0x04, 0x03, 0x03, 0x04, 
  /* RLE: 028 Pixels @ 002,040 */ 28, 0x01, 
  /* ABS: 006 Pixels @ 030,040 */ 0, 6, 0x12, 0x05, 0x05, 0x67, 0x01, 0x5A, 
  /* RLE: 011 Pixels @ 036,040 */ 11, 0x02, 
  /* ABS: 002 Pixels @ 047,040 */ 0, 2, 0x93, 0xD4, 
  /* RLE: 013 Pixels @ 049,040 */ 13, 0x01, 
  /* ABS: 004 Pixels @ 062,040 */ 0, 4, 0x04, 0x03, 0x03, 0x04, 
  /* RLE: 028 Pixels @ 002,041 */ 28, 0x01, 
  /* ABS: 006 Pixels @ 030,041 */ 0, 6, 0x12, 0x05, 0x05, 0xBB, 0x08, 0x6B, 
  /* RLE: 010 Pixels @ 036,041 */ 10, 0x18, 
  /* ABS: 003 Pixels @ 046,041 */ 0, 3, 0xCC, 0xD1, 0x1B, 
  /* RLE: 013 Pixels @ 049,041 */ 13, 0x01, 
  /* ABS: 004 Pixels @ 062,041 */ 0, 4, 0x04, 0x03, 0x03, 0x04, 
  /* RLE: 028 Pixels @ 002,042 */ 28, 0x01, 
  /* ABS: 004 Pixels @ 030,042 */ 0, 4, 0x12, 0x05, 0x05, 0x12, 
  /* RLE: 028 Pixels @ 034,042 */ 28, 0x01, 
  /* ABS: 004 Pixels @ 062,042 */ 0, 4, 0x04, 0x03, 0x03, 0x04, 
  /* RLE: 028 Pixels @ 002,043 */ 28, 0x01, 
  /* ABS: 004 Pixels @ 030,043 */ 0, 4, 0x12, 0x05, 0x05, 0x12, 
  /* RLE: 028 Pixels @ 034,043 */ 28, 0x01, 
  /* ABS: 004 Pixels @ 062,043 */ 0, 4, 0x04, 0x03, 0x03, 0x04, 
  /* RLE: 028 Pixels @ 002,044 */ 28, 0x01, 
  /* ABS: 004 Pixels @ 030,044 */ 0, 4, 0x12, 0x05, 0x05, 0x12, 
  /* RLE: 028 Pixels @ 034,044 */ 28, 0x01, 
  /* ABS: 004 Pixels @ 062,044 */ 0, 4, 0x04, 0x03, 0x03, 0x04, 
  /* RLE: 028 Pixels @ 002,045 */ 28, 0x01, 
  /* ABS: 004 Pixels @ 030,045 */ 0, 4, 0x12, 0x05, 0x05, 0x12, 
  /* RLE: 028 Pixels @ 034,045 */ 28, 0x01, 
  /* ABS: 004 Pixels @ 062,045 */ 0, 4, 0x04, 0x03, 0x03, 0x04, 
  /* RLE: 028 Pixels @ 002,046 */ 28, 0x01, 
  /* ABS: 004 Pixels @ 030,046 */ 0, 4, 0x12, 0x05, 0x05, 0x12, 
  /* RLE: 028 Pixels @ 034,046 */ 28, 0x01, 
  /* ABS: 004 Pixels @ 062,046 */ 0, 4, 0x04, 0x03, 0x03, 0x04, 
  /* RLE: 028 Pixels @ 002,047 */ 28, 0x01, 
  /* ABS: 004 Pixels @ 030,047 */ 0, 4, 0x12, 0x05, 0x05, 0x12, 
  /* RLE: 028 Pixels @ 034,047 */ 28, 0x01, 
  /* ABS: 004 Pixels @ 062,047 */ 0, 4, 0x04, 0x03, 0x03, 0x04, 
  /* RLE: 028 Pixels @ 002,048 */ 28, 0x01, 
  /* ABS: 004 Pixels @ 030,048 */ 0, 4, 0x64, 0x05, 0x05, 0x64, 
  /* RLE: 028 Pixels @ 034,048 */ 28, 0x01, 
  /* ABS: 004 Pixels @ 062,048 */ 0, 4, 0x04, 0x03, 0x03, 0x04, 
  /* RLE: 027 Pixels @ 002,049 */ 27, 0x01, 
  /* RLE: 001 Pixels @ 029,049 */ 1, 0x7D, 
  /* RLE: 004 Pixels @ 030,049 */ 4, 0x05, 
  /* RLE: 001 Pixels @ 034,049 */ 1, 0x7E, 
  /* RLE: 027 Pixels @ 035,049 */ 27, 0x01, 
  /* ABS: 004 Pixels @ 062,049 */ 0, 4, 0x04, 0x03, 0x03, 0x04, 
  /* RLE: 004 Pixels @ 002,050 */ 4, 0x01, 
  /* RLE: 001 Pixels @ 006,050 */ 1, 0x65, 
  /* RLE: 021 Pixels @ 007,050 */ 21, 0x07, 
  /* ABS: 002 Pixels @ 028,050 */ 0, 2, 0x63, 0x4E, 
  /* RLE: 004 Pixels @ 030,050 */ 4, 0x05, 
  /* ABS: 002 Pixels @ 034,050 */ 0, 2, 0x4E, 0x63, 
  /* RLE: 021 Pixels @ 036,050 */ 21, 0x07, 
  /* RLE: 001 Pixels @ 057,050 */ 1, 0x65, 
  /* RLE: 004 Pixels @ 058,050 */ 4, 0x01, 
  /* ABS: 004 Pixels @ 062,050 */ 0, 4, 0x04, 0x03, 0x03, 0x04, 
  /* RLE: 004 Pixels @ 002,051 */ 4, 0x01, 
  /* RLE: 001 Pixels @ 006,051 */ 1, 0x51, 
  /* RLE: 050 Pixels @ 007,051 */ 50, 0x05, 
  /* RLE: 001 Pixels @ 057,051 */ 1, 0x51, 
  /* RLE: 004 Pixels @ 058,051 */ 4, 0x01, 
  /* ABS: 004 Pixels @ 062,051 */ 0, 4, 0x04, 0x03, 0x03, 0x04, 
  /* RLE: 004 Pixels @ 002,052 */ 4, 0x01, 
  /* RLE: 001 Pixels @ 006,052 */ 1, 0x53, 
  /* RLE: 021 Pixels @ 007,052 */ 21, 0x06, 
  /* RLE: 001 Pixels @ 028,052 */ 1, 0x4F, 
  /* RLE: 006 Pixels @ 029,052 */ 6, 0x05, 
  /* RLE: 001 Pixels @ 035,052 */ 1, 0x4F, 
  /* RLE: 021 Pixels @ 036,052 */ 21, 0x06, 
  /* RLE: 001 Pixels @ 057,052 */ 1, 0x53, 
  /* RLE: 004 Pixels @ 058,052 */ 4, 0x01, 
  /* ABS: 004 Pixels @ 062,052 */ 0, 4, 0x04, 0x03, 0x03, 0x04, 
  /* RLE: 027 Pixels @ 002,053 */ 27, 0x01, 
  /* RLE: 001 Pixels @ 029,053 */ 1, 0x79, 
  /* RLE: 004 Pixels @ 030,053 */ 4, 0x05, 
  /* RLE: 001 Pixels @ 034,053 */ 1, 0x7A, 
  /* RLE: 027 Pixels @ 035,053 */ 27, 0x01, 
  /* ABS: 004 Pixels @ 062,053 */ 0, 4, 0x04, 0x03, 0x03, 0x04, 
  /* RLE: 027 Pixels @ 002,054 */ 27, 0x01, 
  /* ABS: 006 Pixels @ 029,054 */ 0, 6, 0x3F, 0x7B, 0x50, 0x50, 0x7C, 0x3F, 
  /* RLE: 027 Pixels @ 035,054 */ 27, 0x01, 
  /* ABS: 004 Pixels @ 062,054 */ 0, 4, 0x04, 0x03, 0x03, 0x04, 
  /* RLE: 060 Pixels @ 002,055 */ 60, 0x01, 
  /* ABS: 004 Pixels @ 062,055 */ 0, 4, 0x04, 0x03, 0x03, 0x04, 
  /* RLE: 060 Pixels @ 002,056 */ 60, 0x01, 
  /* ABS: 004 Pixels @ 062,056 */ 0, 4, 0x04, 0x03, 0x03, 0x04, 
  /* RLE: 060 Pixels @ 002,057 */ 60, 0x01, 
  /* ABS: 004 Pixels @ 062,057 */ 0, 4, 0x04, 0x03, 0x03, 0x04, 
  /* RLE: 060 Pixels @ 002,058 */ 60, 0x01, 
  /* ABS: 004 Pixels @ 062,058 */ 0, 4, 0x04, 0x03, 0x03, 0x04, 
  /* RLE: 060 Pixels @ 002,059 */ 60, 0x01, 
  /* ABS: 004 Pixels @ 062,059 */ 0, 4, 0x04, 0x03, 0x03, 0x04, 
  /* RLE: 060 Pixels @ 002,060 */ 60, 0x01, 
  /* ABS: 004 Pixels @ 062,060 */ 0, 4, 0x04, 0x03, 0x39, 0x3E, 
  /* RLE: 060 Pixels @ 002,061 */ 60, 0x01, 
  /* ABS: 005 Pixels @ 062,061 */ 0, 5, 0x3E, 0x52, 0x74, 0xA7, 0x6A, 
  /* RLE: 058 Pixels @ 003,062 */ 58, 0x01, 
  /* ABS: 006 Pixels @ 061,062 */ 0, 6, 0x6A, 0xA6, 0x73, 0x41, 0x54, 0x39, 
  /* RLE: 058 Pixels @ 003,063 */ 58, 0x03, 
  /* ABS: 003 Pixels @ 061,063 */ 0, 3, 0x71, 0x72, 0x41, 
  0
};  // 1420 bytes for 4096 pixels

GUI_CONST_STORAGE GUI_BITMAP bmIcon_emFTP_64x64 = {
  64, // xSize
  64, // ySize
  64, // BytesPerLine
  GUI_COMPRESS_RLE8, // BitsPerPixel
  _acIcon_emFTP_64x64,  // Pointer to picture data (indices)
  &_PalIcon_emFTP_64x64,  // Pointer to palette
  GUI_DRAW_RLE8
};

/*************************** End of file ****************************/
