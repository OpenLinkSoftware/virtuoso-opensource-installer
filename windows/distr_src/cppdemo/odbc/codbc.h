/*--------------------------------------------------------------------------
  CODBC.H - Low-level ODBC object classes

  This code is furnished on an as-is basis as part of the ODBC SDK and is
  intended for example purposes only.

--------------------------------------------------------------------------*/

#ifndef __CODBC_H
#define __CODBC_H

#include    <sql.h>
#include    <sqlext.h>

#if defined(_AFX) && !defined(NOODBCEX)
#define CRIT_CHECK_EX(f)    { if( !Success(m_rc)) \
								ThrowODBCException(this, SQL_API_##f, m_rc); }
#define CHECK_EX(f)         { if (!Success(m_rc)) \
								ThrowODBCException(this, SQL_API_##f, m_rc); }
#elif !defined(_AFX) && !defined(NOODBCEX)
	// Without the MFC exception handling, we'll display a (very) simple
	// message box that at least displays the error code if the action was
	// not successful.  In the absence of the MFC exception handler, we'll
	// only check for exceptions in constructors and destructors, where it
	// is not possible for the executing program to check m_rc or Success if
	// the construction or destruction fails.
	//
	// This simple message box could be replaced by more elaborate application
	// specific error handling; it is only done this way as an example.
#define CRIT_CHECK_EX(f)    if( !Success(m_rc) ) \
							{   TCHAR szMsg[15]; \
								wsprintf(szMsg, _T("SQLRETURN %d"), m_rc); \
								MessageBox(GetActiveWindow(), szMsg, \
									_T("Error:"), MB_ICONINFORMATION | MB_OK); }
#define CHECK_EX(f)
#else
#define CRIT_CHECK_EX(f)
#define CHECK_EX(f)
#endif

#ifdef  _AFX
#define ODBC_OBJECT         : public CObject
#else
#define ODBC_OBJECT
#endif


typedef SQLPOINTER *LPPTR;
typedef SQLSMALLINT *LPSWORD;
//typedef SQLUSMALLINT FAR *LPSQLUSMALLINT;
//typedef SQLLEN FAR *LPSDWORD;
//typedef SQLULEN FAR *SQLULEN *;


////////////////////////////////////////////////////////////////////////////
// Classes declared
//
class CODBCException;                     // ODBC exception object

class CODBC;                              // ODBC base object
	class CENV;                            // ODBC environment object
	class CDBC;                            // ODBC connection object
	class CSTMT;                           // ODBC statement object


////////////////////////////////////////////////////////////////////////////
// CODBC
class CODBC ODBC_OBJECT {

#ifdef _AFX
	DECLARE_DYNAMIC(CODBC);                 // Required by MFC
#endif

	// Public members
	public:

		SQLRETURN m_rc;

		virtual SQLRETURN Error(SQLTCHAR *szSqlState,
							SQLINTEGER *pfNativeError,
							SQLTCHAR *    szErrorMsg,
							SQLSMALLINT   cbErrorMsgMax,
							SQLSMALLINT * pcbErrorMsg) = 0;

		BOOL Success(SQLRETURN rc);

	// Protected members
	protected:

		CODBC();
		virtual ~CODBC();

		SQLRETURN Error(HENV   henv,
					HDBC     hdbc,
					HSTMT    hstmt,
					SQLTCHAR *szSqlState,
					SQLINTEGER *pfNativeError,
					SQLTCHAR *    szErrorMsg,
					SQLSMALLINT   cbErrorMsgMax,
					SQLSMALLINT * pcbErrorMsg);

	// Private members
	private:

		CODBC(const CODBC& odbcSrc);          // No implementation
		void operator=(const CODBC& odbcSrc); // No implementation

};


#ifdef _AFX
////////////////////////////////////////////////////////////////////////////
// CODBCException
class CODBCException : public CException {

	DECLARE_DYNAMIC(CODBCException);       // Required by MFC

	// Public members
	public:

		CODBCException(CODBC* podbc, SQLUSMALLINT fFunction, SQLRETURN rc);

		CODBC*  m_podbc;                   // ODBC object
		SQLUSMALLINT   m_fFunction;               // ODBC function code
		SQLRETURN m_rc;                      // ODBC return code
};

void ThrowODBCException(CODBC* podbc, SQLUSMALLINT fFunction, SQLRETURN rc=SQL_ERROR);
#endif


////////////////////////////////////////////////////////////////////////////
// CENV
class CENV : public CODBC {

#ifdef _AFX
	DECLARE_DYNAMIC(CENV);                  // Required by MFC
#endif

	// Public members:
	public:

		CENV();
		~CENV();

		HENV    m_henv;                    // ODBC environment handle

		operator HENV() const;       // CENV to HENV conversion

		virtual SQLRETURN Error(SQLTCHAR *szSqlState,
							SQLINTEGER *pfNativeError,
							SQLTCHAR *    szErrorMsg,
							SQLSMALLINT   cbErrorMsgMax = SQL_MAX_MESSAGE_LENGTH-1,
							SQLSMALLINT * pcbErrorMsg = NULL);

		// ODBC functions
		CDBC*   AllocConnect();

		SQLRETURN DataSources(SQLUSMALLINT fDirection,
							SQLTCHAR * szDSN,
							SQLTCHAR * szDescription,
							SQLSMALLINT cbDescription=255);
	   SQLRETURN DataSources(SQLUSMALLINT   fDirection,
							SQLTCHAR *   szDSN,
							SQLSMALLINT   cbDSNMax,
							LPSWORD pcbDSN,
							SQLTCHAR *   szDescription,
							SQLSMALLINT   cbDescriptionMax,
							LPSWORD pcbDescription);

		SQLRETURN Transact(SQLUSMALLINT fType);

	// Protected members:
	protected:

};


////////////////////////////////////////////////////////////////////////////
// CDBC
class CDBC : public CODBC {

#ifdef _AFX
	DECLARE_DYNAMIC(CDBC);                  // Required by MFC
#endif

	// Public members
	public:

		CDBC(CENV& env);
		CDBC(CENV* penv);
		~CDBC();

		HDBC    m_hdbc;                    // ODBC connection handle
		CENV*   m_penv;                    // Associated environment object

		operator HDBC() const;        // CDBC to HDBC  conversion
		operator CENV*() const;       // CDBC to CENV* conversion
		operator CENV&() const;       // CDBC to CENV& conversion

		virtual SQLRETURN Error(SQLTCHAR *szSqlState,
							SQLINTEGER *pfNativeError,
							SQLTCHAR *    szErrorMsg,
							SQLSMALLINT   cbErrorMsgMax = SQL_MAX_MESSAGE_LENGTH-1,
							SQLSMALLINT * pcbErrorMsg = NULL);

		// ODBC functions
		CSTMT*  AllocStmt();

		SQLRETURN BrowseConnect(SQLTCHAR * szConnStrIn,
							SQLTCHAR *    szConnStrOut,
							SQLSMALLINT    cbConnStrOutMax);
		SQLRETURN BrowseConnect(SQLTCHAR * szConnStrIn,
							SQLSMALLINT    cbConnStrIn,
							SQLTCHAR *    szConnStrOut,
							SQLSMALLINT    cbConnStrOutMax,
							LPSWORD  pcbConnStrOut = NULL);

		SQLRETURN Connect(SQLTCHAR * szDSN,
						SQLTCHAR * szUID,
						SQLTCHAR * szAuthStr = NULL);
		SQLRETURN Connect(SQLTCHAR * szDSN,
						SQLSMALLINT  cbDSN,
						SQLTCHAR * szUID,
						SQLSMALLINT  cbUID,
						SQLTCHAR * szAuthStr,
						SQLSMALLINT  cbAuthStr);

		SQLRETURN Disconnect();

		SQLRETURN DriverConnect(
					SQLHWND   hwnd,
					SQLTCHAR * szConnStrIn,
					SQLUSMALLINT  fDriverCompletion = SQL_DRIVER_COMPLETE_REQUIRED);
		SQLRETURN DriverConnect(
					SQLHWND   hwnd,
					SQLTCHAR * szConnStrIn,
					SQLTCHAR * szConnStrOut,
					SQLSMALLINT  cbConnStrOutMax,
					SQLUSMALLINT  fDriverCompletion = SQL_DRIVER_COMPLETE_REQUIRED);
		SQLRETURN DriverConnect(
					SQLHWND    hwnd,
					SQLTCHAR *szConnStrIn,
					SQLSMALLINT   cbConnStrIn,
					SQLTCHAR *szConnStrOut,
					SQLSMALLINT   cbConnStrOutMax,
					LPSWORD pcbConnStrOut,
					SQLUSMALLINT   fDriverCompletion = SQL_DRIVER_COMPLETE_REQUIRED);

		SQLRETURN GetConnectOption(SQLUSMALLINT fOption, PTR pvParam);

		SQLRETURN GetFunctions(SQLUSMALLINT fFunction, SQLUSMALLINT *pfExists);

		SQLRETURN GetInfo(SQLUSMALLINT   fInfoType,
						PTR     rgbInfoValue,
						SQLSMALLINT   cbInfoValueMax = 4,
						LPSWORD pcbInfoValue = NULL);

		SQLRETURN NativeSql(SQLTCHAR * szSqlStrIn,
						SQLTCHAR *szSqlStr,
						SQLINTEGER   cbSqlStrMax,
						SQLINTEGER * pcbSqlStr = NULL);
		SQLRETURN NativeSql(SQLTCHAR * szSqlStrIn,
						SQLINTEGER  cbSqlStrIn,
						SQLTCHAR *szSqlStr,
						SQLINTEGER   cbSqlStrMax,
						SQLINTEGER * pcbSqlStr = NULL);

		SQLRETURN SetConnectOption(SQLUSMALLINT fOption, SQLULEN vParam);

		SQLRETURN Transact(SQLUSMALLINT fType);

	// Protected members:
	protected:

		void Initialize(CENV* penv);
};


////////////////////////////////////////////////////////////////////////////
// CSTMT
class CSTMT : public CODBC {

#ifdef _AFX
	DECLARE_DYNAMIC(CSTMT);                 // Required by MFC
#endif

	// Public members
	public:

		CSTMT(CDBC& dbc);
		CSTMT(CDBC* pdbc);
		~CSTMT();

		HSTMT   m_hstmt;                   // ODBC statement handle
		CDBC*   m_pdbc;                    // Associated connection  object
		CENV*   m_penv;                    // Associated environment object

		operator HSTMT() const;      // CSTMT to HSTMT conversion
		operator CDBC*() const;      // CSTMT to CDBC* conversion
		operator CDBC&() const;      // CSTMT to CDBC& conversion
		operator CENV*() const;      // CSTMT to CENV* conversion
		operator CENV&() const;      // CSTMT to CENV& conversion

		virtual SQLRETURN Error(SQLTCHAR *szSqlState,
							SQLINTEGER * pfNativeError,
							SQLTCHAR *    szErrorMsg,
							SQLSMALLINT   cbErrorMsgMax = SQL_MAX_MESSAGE_LENGTH-1,
							SQLSMALLINT * pcbErrorMsg = NULL);

		// ODBC functions
		SQLRETURN BindCol(SQLUSMALLINT    icol,
						SQLSMALLINT    fCType,
						PTR      rgbValue,
						SQLLEN   cbValueMax,
						SQLLEN * pcbValue);

		SQLRETURN Cancel();

		SQLRETURN ColAttributes(SQLUSMALLINT  icol,
							SQLUSMALLINT    fDescType,
							SQLLEN * pfDesc);
		SQLRETURN ColAttributes(SQLUSMALLINT  icol,
							SQLUSMALLINT    fDescType,
							PTR      rgbDesc,
							SQLSMALLINT    cbDescMax,
							LPSWORD  pcbDesc = NULL);
		SQLRETURN ColAttributes(SQLUSMALLINT  icol,
							SQLUSMALLINT    fDescType,
							PTR      rgbDesc,
							SQLSMALLINT    cbDescMax,
							LPSWORD  pcbDesc,
							SQLLEN * pfDesc);

		SQLRETURN ColumnPrivileges(SQLTCHAR * szTableQualifier,
								SQLTCHAR *  szTableOwner,
								SQLTCHAR *  szTableName,
								SQLTCHAR *  szColumnName);
		SQLRETURN ColumnPrivileges(SQLTCHAR * szTableQualifier,
								SQLSMALLINT   cbTableQualifier,
								SQLTCHAR *  szTableOwner,
								SQLSMALLINT   cbTableOwner,
								SQLTCHAR *  szTableName,
								SQLSMALLINT   cbTableName,
								SQLTCHAR *  szColumnName,
								SQLSMALLINT   cbColumnName);

		SQLRETURN Columns(SQLTCHAR *  szTableQualifier,
						SQLTCHAR *  szTableOwner,
						SQLTCHAR *  szTableName,
						SQLTCHAR *  szColumnName);
		SQLRETURN Columns(SQLTCHAR *  szTableQualifier,
						SQLSMALLINT   cbTableQualifier,
						SQLTCHAR *  szTableOwner,
						SQLSMALLINT   cbTableOwner,
						SQLTCHAR *  szTableName,
						SQLSMALLINT   cbTableName,
						SQLTCHAR *  szColumnName,
						SQLSMALLINT   cbColumnName);

		SQLRETURN DescribeCol(SQLUSMALLINT    icol,
							SQLTCHAR *    szColName,
							SQLSMALLINT    cbColNameMax,
							LPSWORD  pcbColName,
							LPSWORD  pfSqlType,
							SQLULEN * pcbColDef,
							LPSWORD  pibScale,
							LPSWORD  pfNullable);

		SQLRETURN DescribeParam(SQLUSMALLINT  ipar,
							LPSWORD  pfSqlType,
							SQLULEN * pcbColDef,
							LPSWORD  pibScale,
							LPSWORD  pfNullable);

		SQLRETURN ExecDirect(SQLTCHAR * szSqlStr,
						SQLINTEGER    cbSqlStr = SQL_NTS);

		SQLRETURN Execute();

		SQLRETURN ExtendedFetch(SQLUSMALLINT  fFetchType,
							SQLLEN   irow,
							SQLUSMALLINT  *rgfRowStatus);
		SQLRETURN ExtendedFetch(SQLUSMALLINT  fFetchType,
							SQLLEN   irow,
							SQLULEN * pcrow,
							SQLUSMALLINT *rgfRowStatus);

		SQLRETURN Fetch();
		SQLRETURN Fetch(SQLUSMALLINT  fFetchType,
					SQLLEN   irow,
					SQLUSMALLINT *rgfRowStatus);
		SQLRETURN Fetch(SQLUSMALLINT  fFetchType,
					SQLLEN   irow,
					SQLULEN * pcrow,
					SQLUSMALLINT *rgfRowStatus);

		SQLRETURN ForeignKeys(SQLTCHAR * szPkTableQualifier,
							SQLTCHAR * szPkTableOwner,
							SQLTCHAR * szPkTableName,
							SQLTCHAR * szFkTableQualifier,
							SQLTCHAR * szFkTableOwner,
							SQLTCHAR * szFkTableName);
		SQLRETURN ForeignKeys(SQLTCHAR * szPkTableQualifier,
							SQLSMALLINT  cbPkTableQualifier,
							SQLTCHAR * szPkTableOwner,
							SQLSMALLINT  cbPkTableOwner,
							SQLTCHAR * szPkTableName,
							SQLSMALLINT  cbPkTableName,
							SQLTCHAR * szFkTableQualifier,
							SQLSMALLINT  cbFkTableQualifier,
							SQLTCHAR * szFkTableOwner,
							SQLSMALLINT  cbFkTableOwner,
							SQLTCHAR * szFkTableName,
							SQLSMALLINT  cbFkTableName);

		SQLRETURN Close();
		SQLRETURN Unbind();
		SQLRETURN ResetParams();
		SQLRETURN FreeStmt(SQLUSMALLINT fOption);

		SQLRETURN GetCursorName(SQLTCHAR * szCursor,
							SQLSMALLINT   cbCursorMax,
							LPSWORD pcbCursor = NULL);

		SQLRETURN GetData(SQLUSMALLINT    icol,
						SQLSMALLINT    fCType,
						PTR      rgbValue,
						SQLLEN   cbValueMax,
						SQLLEN * pcbValue);

		SQLRETURN GetStmtOption(SQLUSMALLINT fOption, PTR pvParam);

		SQLRETURN GetTypeInfo(SQLSMALLINT fSqlType);

		SQLRETURN MoreResults();

		SQLRETURN NumParams(LPSWORD pcpar);

		SQLRETURN NumResultCols(LPSWORD pccol);

		SQLRETURN ParamData(LPPTR prgbValue);

		SQLRETURN ParamOptions(SQLUINTEGER crow, SQLULEN * pirow);

		SQLRETURN Prepare(SQLTCHAR * szSqlStr,
						SQLINTEGER cbSqlStr = SQL_NTS);

		SQLRETURN PrimaryKeys(SQLTCHAR * szTableQualifier,
							SQLTCHAR * szTableOwner,
							SQLTCHAR * szTableName);
		SQLRETURN PrimaryKeys(SQLTCHAR * szTableQualifier,
							SQLSMALLINT  cbTableQualifier,
							SQLTCHAR * szTableOwner,
							SQLSMALLINT  cbTableOwner,
							SQLTCHAR * szTableName,
							SQLSMALLINT  cbTableName);

		SQLRETURN ProcedureColumns(SQLTCHAR * szProcQualifier,
								SQLTCHAR *  szProcOwner,
								SQLTCHAR *  szProcName,
								SQLTCHAR *  szColumnName);
		SQLRETURN ProcedureColumns(SQLTCHAR * szProcQualifier,
								SQLSMALLINT   cbProcQualifier,
								SQLTCHAR *  szProcOwner,
								SQLSMALLINT   cbProcOwner,
								SQLTCHAR *  szProcName,
								SQLSMALLINT   cbProcName,
								SQLTCHAR *  szColumnName,
								SQLSMALLINT   cbColumnName);

		SQLRETURN Procedures(SQLTCHAR * szProcQualifier,
						SQLTCHAR *    szProcOwner,
						SQLTCHAR *    szProcName);
		SQLRETURN Procedures(SQLTCHAR * szProcQualifier,
						SQLSMALLINT     cbProcQualifier,
						SQLTCHAR *    szProcOwner,
						SQLSMALLINT     cbProcOwner,
						SQLTCHAR *    szProcName,
						SQLSMALLINT     cbProcName);

		SQLRETURN PutData(PTR rgbValue, SQLLEN cbValue);

		SQLRETURN RowCount(SQLLEN * pcrow);

		SQLRETURN SetCursorName(SQLTCHAR * szCursor,
							SQLSMALLINT    cbCursor = SQL_NTS);

		SQLRETURN SetParam(SQLUSMALLINT   ipar,
						SQLSMALLINT    fCType,
						SQLSMALLINT    fSqlType,
						SQLULEN   cbColDef,
						SQLSMALLINT    ibScale,
						PTR      rgbValue,
						SQLLEN * pcbValue = NULL);

		SQLRETURN SetPos(SQLUSMALLINT irow,
						BOOL fRefresh = TRUE,
						BOOL fLock = FALSE);

		SQLRETURN SetScrollOptions(SQLUSMALLINT fConcurrency = SQL_CONCUR_READ_ONLY,
								SQLLEN crowKeyset = SQL_SCROLL_FORWARD_ONLY,
								SQLUSMALLINT  crowRowset = 1);

		SQLRETURN SetStmtOption(SQLUSMALLINT fOption, SQLULEN vParam);

		SQLRETURN SpecialColumns(SQLUSMALLINT fColType,
							SQLTCHAR *   szTableQualifier,
							SQLTCHAR *   szTableOwner,
							SQLTCHAR *   szTableName,
							SQLUSMALLINT    fScope = SQL_SCOPE_TRANSACTION,
							SQLUSMALLINT    fNullable = SQL_NULLABLE);
		SQLRETURN SpecialColumns(SQLUSMALLINT fColType,
							SQLTCHAR *   szTableQualifier,
							SQLSMALLINT    cbTableQualifier,
							SQLTCHAR *   szTableOwner,
							SQLSMALLINT    cbTableOwner,
							SQLTCHAR *   szTableName,
							SQLSMALLINT    cbTableName,
							SQLUSMALLINT    fScope = SQL_SCOPE_TRANSACTION,
							SQLUSMALLINT    fNullable = SQL_NULLABLE);

		SQLRETURN Statistics(SQLTCHAR *  szTableQualifier,
							SQLTCHAR * szTableOwner,
							SQLTCHAR * szTableName,
							SQLUSMALLINT  fUnique = SQL_INDEX_UNIQUE,
							SQLUSMALLINT  fAccuracy = SQL_QUICK);
		SQLRETURN Statistics(SQLTCHAR *  szTableQualifier,
							SQLSMALLINT  cbTableQualifier,
							SQLTCHAR * szTableOwner,
							SQLSMALLINT  cbTableOwner,
							SQLTCHAR * szTableName,
							SQLSMALLINT  cbTableName,
							SQLUSMALLINT  fUnique = SQL_INDEX_UNIQUE,
							SQLUSMALLINT  fAccuracy = SQL_QUICK);

		SQLRETURN TablePrivileges(SQLTCHAR * szTableQualifier,
								SQLTCHAR * szTableOwner,
								SQLTCHAR * szTableName);
		SQLRETURN TablePrivileges(SQLTCHAR * szTableQualifier,
								SQLSMALLINT  cbTableQualifier,
								SQLTCHAR * szTableOwner,
								SQLSMALLINT  cbTableOwner,
								SQLTCHAR * szTableName,
								SQLSMALLINT  cbTableName);

		SQLRETURN Tables(SQLTCHAR *  szTableQualifier,
						SQLTCHAR * szTableOwner,
						SQLTCHAR * szTableName,
						SQLTCHAR * szTableType);
		SQLRETURN Tables(SQLTCHAR *  szTableQualifier,
						SQLSMALLINT  cbTableQualifier,
						SQLTCHAR * szTableOwner,
						SQLSMALLINT  cbTableOwner,
						SQLTCHAR * szTableName,
						SQLSMALLINT  cbTableName,
						SQLTCHAR * szTableType,
						SQLSMALLINT  cbTableType);

	// Protected members
	protected:
	
		void Initialize(CDBC* pdbc);
};


// Inline functions --------------------------------------------------------
#include    "codbc.inl"

#endif // __CODBC_H
