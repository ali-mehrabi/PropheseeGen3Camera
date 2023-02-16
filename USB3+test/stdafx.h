
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions





#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC support for ribbons and control bars

#include <windows.h>
#undef _MP

#include <dbt.h>
#define ONE_MB                              (1000*1000)
#define ENABLE_COMMANDLINE_INPUT            1

#define WM_STARTWORK                        WM_APP + 0x1 // sent from the end of OnInitDialog to start our operation

#define PAYLOAD_RECORDER_DIRECTORY_NAME     _T("FT600DataLoopback_Output")
#define APPLICATION_LOG_FILE_NAME           _T("FT600DataLoopback.txt")
#define APPLICATION_TITLE                   _T("FT600 Data Loopback")

typedef unsigned(__stdcall* PTHREAD_START) (void*);



//
// Define when linking with static library
// Undefine when linking with dynamic library
//
#define FTD3XX_STATIC


//
// Include D3XX library
//
//#include "FTD3XXLibrary\FTD3XX.h"
#pragma comment(lib, "FTD3XX.lib")


//
// FPGA FIFO master buffer size
//
#define FIFO_SIZE 4096


//#define DISALLOW_STOPPING 1
//#define DISALLOW_CLOSING 1

#pragma once
