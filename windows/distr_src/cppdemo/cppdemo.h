/*--------------------------------------------------------------------------
  CPPDEMO.H -- Shared constants, types, prototypes, variables

  This code is furnished on an as-is basis as part of the ODBC SDK and is
  intended for example purposes only.

--------------------------------------------------------------------------*/
      
#ifndef __CPPDEMO_H
#define __CPPDEMO_H

#ifdef	INCL_GLOBAL
#define	GLOBAL
#define	INITHAND(name, type)		type name = NULL;
#define	CONSTSTR(name, str)			const LPTSTR name = str
#else
#define	GLOBAL						extern
#define	INITHAND(name, type)		extern type name
#define CONSTSTR(name, str)			extern const LPTSTR name
#endif


//////////////////////////////////////////////////////////////////////////////
//
//	Defines
//
#ifdef _AFX
IMPLEMENT_DYNAMIC(CODBCException, CException);
IMPLEMENT_DYNAMIC(CODBC, CObject);
IMPLEMENT_DYNAMIC(CENV,  CODBC);
IMPLEMENT_DYNAMIC(CDBC,  CODBC);
IMPLEMENT_DYNAMIC(CSTMT, CODBC);
#endif

#ifdef WIN32
#define EXPFUNC CALLBACK
#define INTFUNC WINAPI
#else
#ifndef EXPORT
#define EXPORT		_export
#endif
#define EXPFUNC EXPORT CALLBACK
#define INTFUNC WINAPI
#endif

#ifdef	_DEBUG
#define	DASSERT(x)		assert(x)
#else
#define	DASSERT(x)		(x)
#endif

#define	AllocPtr(x)		GlobalAllocPtr(GHND, (x))
#define	FreePtr(x)		if( (x) ) GlobalFreePtr((x))

#define	MF_ENABLE		MF_ENABLED | MF_BYCOMMAND
#define MF_DISABLE		MF_GRAYED | MF_BYCOMMAND

#define	HOURGLASS(h)	{\
						h = SetCursor(LoadCursor(NULL, IDC_WAIT));\
						ShowCursor(TRUE);\
						}
#define ARROW(h)		{\
						ShowCursor(FALSE);\
						SetCursor(h);\
                        }

#ifndef min
#define min(a,b)        (((a) < (b)) ? (a) : (b))
#endif

#ifndef max
#define max(a,b)        (((a) > (b)) ? (a) : (b))
#endif


//////////////////////////////////////////////////////////////////////////////
//
//	Constants
//

// Miscellaneous
GLOBAL  const int       cbSTRLEN        = 256;      // Max string length
GLOBAL  const int       cbMAXSQL        = 512;      // Max SQL string length
GLOBAL  const int       cPOINTS         = 10;       // Font point size
GLOBAL  const int       cxBORDER        = 6;        // Column border
GLOBAL  const int       cbCOLMEMMAX     = 10000L;   // Maximum column mem size
GLOBAL  const int       HLP_CPPDEMO     = 20;       // Help context number

// Strings
CONSTSTR(szCLASSNAME,   _T("ODBC_CPP_DEMO"));           // Window class name
CONSTSTR(szDATATRUNC,   _T("01004"));                   // ODBC truncation error
CONSTSTR(szDSNKEY,      _T("DSN="));                    // Data source name key
CONSTSTR(szDASH,        _T(" - "));                     // Title separator
CONSTSTR(szNODSN,       _T("no DSN"));                  // DSN name when no DSN
CONSTSTR(szFONT,        _T("MS Sans Serif"));           // Window font name
CONSTSTR(szSCROLLCLASS, _T("SCROLLBAR"));               // Scrollbar class name
CONSTSTR(szHELPFILE,    _T("..\\help\\odbcsmpl.hlp"));  // Samples help file


#endif	// __CPPDEMO_H
