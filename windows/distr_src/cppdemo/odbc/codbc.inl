/*--------------------------------------------------------------------------
  CODBC.INL - Low-level ODBC object classes inline functions

  This code is furnished on an as-is basis as part of the ODBC SDK and is
  intended for example purposes only.

--------------------------------------------------------------------------*/

#ifndef __CODBC_INL
#define __CODBC_INL

#if defined(_AFX) && defined(_DEBUG) && !defined(new)
#define new DEBUG_NEW                  // Debugging version of new (MFC)
#endif

#if !defined(_AFX) || !defined(_DEBUG)
#define ASSERT(x)   (x)
#endif

////////////////////////////////////////////////////////////////////////////
// CODBC
inline CODBC::CODBC()
{
	m_rc = SQL_SUCCESS;
	return;
}

inline CODBC::~CODBC()
{
	return;
}

inline SQLRETURN CODBC::Error(HENV    henv,
							HDBC     hdbc,
							HSTMT    hstmt,
							SQLTCHAR *szSqlState,
							SQLINTEGER * pfNativeError,
							SQLTCHAR *    szErrorMsg,
							SQLSMALLINT   cbErrorMsgMax,
							SQLSMALLINT * cbErrorMsg)
{
	m_rc = ::SQLError(henv, hdbc, hstmt,
					szSqlState,
					pfNativeError,
					szErrorMsg,
					cbErrorMsgMax, cbErrorMsg);
	return m_rc;
}

inline BOOL CODBC::Success(SQLRETURN rc)
{
	return (rc >= 0);
}


#ifdef _AFX
////////////////////////////////////////////////////////////////////////////
// CODBCException
inline CODBCException::CODBCException(CODBC*   podbc,
										SQLUSMALLINT   fFunction,
										SQLRETURN rc)
{
	m_podbc     = podbc;
	m_fFunction = fFunction;
	m_rc        = rc;
	return;
}

inline void ThrowODBCException(CODBC*  podbc,
								SQLUSMALLINT   fFunction,
								SQLRETURN rc)
{
	THROW(new CODBCException(podbc, fFunction, rc));
	return;
}
#endif


////////////////////////////////////////////////////////////////////////////
// CENV
inline CENV::CENV()
{
	m_henv = SQL_NULL_HENV;
	m_rc   = ::SQLAllocEnv(&m_henv);
	CRIT_CHECK_EX(SQLALLOCENV);
	return;
}

inline CENV::~CENV()
{
	if (m_henv != SQL_NULL_HENV) {
		m_rc = ::SQLFreeEnv(m_henv);
		CRIT_CHECK_EX(SQLFREEENV);
	}
	return;
}

inline CENV::operator HENV() const
{
	return m_henv;
}

inline SQLRETURN CENV::Error(SQLTCHAR *szSqlState,
							SQLINTEGER * pfNativeError,
							SQLTCHAR *    szErrorMsg,
							SQLSMALLINT   cbErrorMsgMax,
							SQLSMALLINT * pcbErrorMsg)
{
	ASSERT(m_henv != SQL_NULL_HENV);
	return CODBC::Error(m_henv, SQL_NULL_HDBC, SQL_NULL_HSTMT,
						szSqlState, pfNativeError, szErrorMsg,
						cbErrorMsgMax, pcbErrorMsg);
}

inline CDBC* CENV::AllocConnect()
{
	ASSERT(m_henv != SQL_NULL_HENV);
	return new CDBC(this);
}

inline SQLRETURN CENV::DataSources(SQLUSMALLINT fDirection,
								SQLTCHAR *   szDSN,
								SQLTCHAR *   szDescription,
								SQLSMALLINT   cbDescription)
{
	return DataSources(fDirection, szDSN, SQL_MAX_DSN_LENGTH-1, NULL,
							szDescription, cbDescription, NULL);
}

inline SQLRETURN CENV::DataSources(SQLUSMALLINT fDirection,
								SQLTCHAR *   szDSN,
								SQLSMALLINT   cbDSNMax,
								LPSWORD pcbDSN,
								SQLTCHAR *   szDescription,
								SQLSMALLINT   cbDescriptionMax,
								LPSWORD pcbDescription)
{
	ASSERT(m_henv != SQL_NULL_HENV);
	m_rc = ::SQLDataSources(m_henv, fDirection,
							szDSN, cbDSNMax, pcbDSN,
							szDescription, cbDescriptionMax,
							pcbDescription);
	CHECK_EX(SQLDATASOURCES);
	return m_rc;
}

inline SQLRETURN CENV::Transact(SQLUSMALLINT fType)
{
	m_rc = ::SQLTransact(m_henv, SQL_NULL_HDBC, fType);
	CHECK_EX(SQLTRANSACT);
	return m_rc;
}


////////////////////////////////////////////////////////////////////////////
// CDBC
inline CDBC::CDBC(CENV& env)
{
	Initialize(&env);
	return;
}

inline CDBC::CDBC(CENV* penv)
{
	Initialize(penv);
	return;
}

inline void CDBC::Initialize(CENV* penv)
{
	ASSERT(penv != NULL);

	m_hdbc = SQL_NULL_HDBC;
	m_penv = penv;
	m_rc   = ::SQLAllocConnect(*penv, &m_hdbc);
	CRIT_CHECK_EX(SQLALLOCCONNECT);
	return;
}

inline CDBC::~CDBC()
{
	if (m_hdbc != SQL_NULL_HDBC) {
		m_rc = ::SQLFreeConnect(m_hdbc);
		CRIT_CHECK_EX(SQLFREECONNECT);
	}
	return;
}

inline CDBC::operator HDBC() const
{
	return m_hdbc;
}

inline CDBC::operator CENV*() const
{
	ASSERT(m_penv != NULL);
	return m_penv;
}

inline CDBC::operator CENV&() const
{
	ASSERT(m_penv != NULL);
	return *m_penv;
}

inline SQLRETURN CDBC::Error(SQLTCHAR *szSqlState,
							SQLINTEGER * pfNativeError,
							SQLTCHAR *    szErrorMsg,
							SQLSMALLINT   cbErrorMsgMax,
							SQLSMALLINT * pcbErrorMsg)
{
	ASSERT(m_hdbc != SQL_NULL_HDBC);
	return CODBC::Error(SQL_NULL_HENV, m_hdbc, SQL_NULL_HSTMT,
						szSqlState, pfNativeError, szErrorMsg,
						cbErrorMsgMax, pcbErrorMsg);
}

inline CSTMT* CDBC::AllocStmt()
{
	ASSERT(m_hdbc != SQL_NULL_HDBC);
	return new CSTMT(this);
}

inline SQLRETURN CDBC::BrowseConnect(SQLTCHAR * szConnStrIn,
									SQLTCHAR *  szConnStrOut,
									SQLSMALLINT  cbConnStrOutMax)
{
	return BrowseConnect(szConnStrIn, SQL_NTS, szConnStrOut, cbConnStrOutMax);
}

inline SQLRETURN CDBC::BrowseConnect(SQLTCHAR *  szConnStrIn,
									SQLSMALLINT   cbConnStrIn,
									SQLTCHAR *   szConnStrOut,
									SQLSMALLINT   cbConnStrOutMax,
									LPSWORD pcbConnStrOut)
{
	ASSERT(m_hdbc != SQL_NULL_HDBC);
	m_rc = ::SQLBrowseConnect(m_hdbc, szConnStrIn, cbConnStrIn,
								szConnStrOut, cbConnStrOutMax,
								pcbConnStrOut);
	CHECK_EX(SQLBROWSECONNECT);
	return m_rc;
}

inline SQLRETURN CDBC::Connect(SQLTCHAR * szDSN, SQLTCHAR * szUID, SQLTCHAR * szAuthStr)
{
	return Connect(szDSN, SQL_NTS, szUID, SQL_NTS, szAuthStr, SQL_NTS);
}

inline SQLRETURN CDBC::Connect(SQLTCHAR * szDSN,
							SQLSMALLINT    cbDSN,
							SQLTCHAR *   szUID,
							SQLSMALLINT    cbUID,
							SQLTCHAR *   szAuthStr,
							SQLSMALLINT    cbAuthStr)
{
	ASSERT(m_hdbc != SQL_NULL_HDBC);
	m_rc = ::SQLConnect(m_hdbc, szDSN, cbDSN,
						szUID, cbUID, 
						szAuthStr, cbAuthStr);
	CHECK_EX(SQLCONNECT);
	return m_rc;
}

inline SQLRETURN CDBC::Disconnect()
{
	ASSERT(m_hdbc != SQL_NULL_HDBC);
	m_rc = ::SQLDisconnect(m_hdbc);
	CHECK_EX(SQLDISCONNECT);
	return m_rc;
}

inline SQLRETURN CDBC::DriverConnect(SQLHWND   hwnd,
									SQLTCHAR * szConnStrIn,
									SQLUSMALLINT  fDriverCompletion)
{
	return DriverConnect(hwnd, szConnStrIn, SQL_NTS,
							NULL, 0, NULL, fDriverCompletion);
}

inline SQLRETURN CDBC::DriverConnect(SQLHWND   hwnd,
									SQLTCHAR * szConnStrIn,
									SQLTCHAR *  szConnStrOut,
									SQLSMALLINT  cbConnStrOutMax,
									SQLUSMALLINT  fDriverCompletion)
{
	return DriverConnect(hwnd, szConnStrIn, SQL_NTS,
					szConnStrOut, cbConnStrOutMax, NULL, fDriverCompletion);
}

inline SQLRETURN CDBC::DriverConnect(SQLHWND    hwnd,
									SQLTCHAR *  szConnStrIn,
									SQLSMALLINT   cbConnStrIn,
									SQLTCHAR *   szConnStrOut,
									SQLSMALLINT   cbConnStrOutMax,
									LPSWORD pcbConnStrOut,
									SQLUSMALLINT   fDriverCompletion)
{
	ASSERT(m_hdbc != SQL_NULL_HDBC);
	m_rc = ::SQLDriverConnect(m_hdbc, hwnd,
						szConnStrIn, cbConnStrIn,
						szConnStrOut, cbConnStrOutMax,
						pcbConnStrOut, fDriverCompletion);
	CHECK_EX(SQLDRIVERCONNECT);
	return m_rc;
}

inline SQLRETURN CDBC::GetConnectOption(SQLUSMALLINT fOption, PTR pvParam)
{
	ASSERT(m_hdbc != SQL_NULL_HDBC);
	m_rc = ::SQLGetConnectOption(m_hdbc, fOption, pvParam);
	CHECK_EX(SQLGETCONNECTOPTION);
	return m_rc;
}

inline SQLRETURN CDBC::GetFunctions(SQLUSMALLINT fFunction, SQLUSMALLINT *pfExists)
{
	ASSERT(m_hdbc != SQL_NULL_HDBC);
	m_rc = ::SQLGetFunctions(m_hdbc, fFunction, pfExists);
	CHECK_EX(SQLGETFUNCTIONS);
	return m_rc;
}

inline SQLRETURN CDBC::GetInfo(SQLUSMALLINT fInfoType,
							PTR     rgbInfoValue,
							SQLSMALLINT   cbInfoValueMax,
							LPSWORD pcbInfoValue)
{
	ASSERT(m_hdbc != SQL_NULL_HDBC);
	m_rc = ::SQLGetInfo(m_hdbc, fInfoType, 
						rgbInfoValue, cbInfoValueMax, pcbInfoValue);
	CHECK_EX(SQLGETFUNCTIONS);
	return m_rc;
}

inline SQLRETURN CDBC::NativeSql(SQLTCHAR *   szSqlStrIn,
								SQLTCHAR *    szSqlStr,
								SQLINTEGER   cbSqlStrMax,
								SQLINTEGER * pcbSqlStr)
{
	return NativeSql(szSqlStrIn, SQL_NTS, szSqlStr, cbSqlStrMax, pcbSqlStr);
}

inline SQLRETURN CDBC::NativeSql(SQLTCHAR *   szSqlStrIn,
								SQLINTEGER   cbSqlStrIn,
								SQLTCHAR *    szSqlStr,
								SQLINTEGER   cbSqlStrMax,
								SQLINTEGER * pcbSqlStr)
{
	ASSERT(m_hdbc != SQL_NULL_HDBC);
	m_rc = ::SQLNativeSql(m_hdbc, szSqlStrIn, cbSqlStrIn,
							szSqlStr, cbSqlStrMax, pcbSqlStr);
	CHECK_EX(SQLNATIVESQL);
	return m_rc;
}

inline SQLRETURN CDBC::SetConnectOption(SQLUSMALLINT fOption, SQLULEN vParam)
{
	ASSERT(m_hdbc != SQL_NULL_HDBC);
	m_rc = ::SQLSetConnectOption(m_hdbc, fOption, vParam);
	CHECK_EX(SQLSETCONNECTOPTION);
	return m_rc;
}

inline SQLRETURN CDBC::Transact(SQLUSMALLINT fType)
{
	ASSERT(m_hdbc != SQL_NULL_HDBC);
	m_rc = ::SQLTransact(SQL_NULL_HENV, m_hdbc, fType);
	CHECK_EX(SQLTRANSACT);
	return m_rc;
}


////////////////////////////////////////////////////////////////////////////
// CSTMT
inline CSTMT::CSTMT(CDBC& dbc)
{
	Initialize(&dbc);
	return;
}

inline CSTMT::CSTMT(CDBC* pdbc)
{
	Initialize(pdbc);
	return;
}

inline void CSTMT::Initialize(CDBC* pdbc)
{
	ASSERT(pdbc != NULL);

	m_hstmt = SQL_NULL_HSTMT;
	m_pdbc  = pdbc;
	m_penv  = (CENV*)pdbc;

    m_rc    = ::SQLAllocStmt(HDBC(*pdbc), &m_hstmt);
	CRIT_CHECK_EX(SQLALLOCSTMT);
	return;
}

inline CSTMT::~CSTMT()
{
	if (m_hstmt != SQL_NULL_HSTMT) {
		m_rc = ::SQLFreeStmt(m_hstmt, SQL_DROP);
		CRIT_CHECK_EX(SQLFREESTMT);
	}
	return;
}

inline CSTMT::operator HSTMT() const
{
	return m_hstmt;
}

inline CSTMT::operator CDBC*() const
{
	ASSERT(m_pdbc != NULL);
	return m_pdbc;
}

inline CSTMT::operator CDBC&() const
{
	ASSERT(m_pdbc != NULL);
	return *m_pdbc;
}

inline CSTMT::operator CENV*() const
{
	ASSERT(m_penv != NULL);
	return m_penv;
}

inline CSTMT::operator CENV&() const
{
	ASSERT(m_penv != NULL);
	return *m_penv;
}

inline SQLRETURN CSTMT::Error(SQLTCHAR *szSqlState,
							SQLINTEGER * pfNativeError,
							SQLTCHAR *    szErrorMsg,
							SQLSMALLINT   cbErrorMsgMax,
							SQLSMALLINT * pcbErrorMsg)
{
	ASSERT(m_hstmt != SQL_NULL_HSTMT);
	return CODBC::Error(SQL_NULL_HENV, SQL_NULL_HDBC, m_hstmt,
						szSqlState, pfNativeError, szErrorMsg,
						cbErrorMsgMax, pcbErrorMsg);
}

inline SQLRETURN CSTMT::BindCol(SQLUSMALLINT icol,
							SQLSMALLINT    fCType,
							PTR      rgbValue,
							SQLLEN   cbValueMax,
							SQLLEN * pcbValue)
{
	ASSERT(m_hstmt != SQL_NULL_HSTMT);
	m_rc = ::SQLBindCol(m_hstmt, icol, fCType, rgbValue, cbValueMax, pcbValue);
	CHECK_EX(SQLBINDCOL);
	return m_rc;
}

inline SQLRETURN CSTMT::Cancel()
{
	ASSERT(m_hstmt != SQL_NULL_HSTMT);
	m_rc = ::SQLCancel(m_hstmt);
	CHECK_EX(SQLCANCEL);
	return m_rc;
}

inline SQLRETURN CSTMT::ColAttributes(SQLUSMALLINT   icol,
									SQLUSMALLINT    fDescType,
									SQLLEN * pfDesc)
{
	return ColAttributes(icol, fDescType, NULL, 0, NULL, pfDesc);
}

inline SQLRETURN CSTMT::ColAttributes(SQLUSMALLINT   icol,
									SQLUSMALLINT    fDescType,
									PTR      rgbDesc,
									SQLSMALLINT    cbDescMax,
									LPSWORD  pcbDesc)
{
	return ColAttributes(icol, fDescType, rgbDesc, cbDescMax, pcbDesc, NULL);
}

inline SQLRETURN CSTMT::ColAttributes(SQLUSMALLINT   icol,
									SQLUSMALLINT    fDescType,
									PTR      rgbDesc,
									SQLSMALLINT    cbDescMax,
									LPSWORD  pcbDesc,
									SQLLEN * pfDesc)
{
	ASSERT(m_hstmt != SQL_NULL_HSTMT);
	m_rc = ::SQLColAttributes(m_hstmt, icol, fDescType, rgbDesc,
								cbDescMax, pcbDesc, pfDesc);
	CHECK_EX(SQLCOLATTRIBUTES);
	return m_rc;
}

inline SQLRETURN CSTMT::ColumnPrivileges(SQLTCHAR * szTableQualifier,
									SQLTCHAR *  szTableOwner,
									SQLTCHAR *  szTableName,
									SQLTCHAR *  szColumnName)
{
	return ColumnPrivileges(szTableQualifier, SQL_NTS,
							szTableOwner, SQL_NTS,
							szTableName, SQL_NTS,
							szColumnName, SQL_NTS);
}

inline SQLRETURN CSTMT::ColumnPrivileges(SQLTCHAR *  szTableQualifier,
										SQLSMALLINT   cbTableQualifier,
										SQLTCHAR *  szTableOwner,
										SQLSMALLINT   cbTableOwner,
										SQLTCHAR *  szTableName,
										SQLSMALLINT   cbTableName,
										SQLTCHAR *  szColumnName,
										SQLSMALLINT   cbColumnName)
{
	ASSERT(m_hstmt != SQL_NULL_HSTMT);
	m_rc = ::SQLColumnPrivileges(m_hstmt,
						szTableQualifier, cbTableQualifier,
						szTableOwner, cbTableOwner,
						szTableName, cbTableName,
						szColumnName, cbColumnName);
	CHECK_EX(SQLCOLUMNPRIVILEGES);
	return m_rc;
}

inline SQLRETURN CSTMT::Columns(SQLTCHAR *   szTableQualifier,
								SQLTCHAR *  szTableOwner,
								SQLTCHAR *  szTableName,
								SQLTCHAR *  szColumnName)
{
	return Columns(szTableQualifier, SQL_NTS,
					szTableOwner, SQL_NTS,
					szTableName, SQL_NTS,
					szColumnName, SQL_NTS);
}

inline SQLRETURN CSTMT::Columns(SQLTCHAR *   szTableQualifier,
								SQLSMALLINT   cbTableQualifier,
								SQLTCHAR *  szTableOwner,
								SQLSMALLINT   cbTableOwner,
								SQLTCHAR *  szTableName,
								SQLSMALLINT   cbTableName,
								SQLTCHAR *  szColumnName,
								SQLSMALLINT   cbColumnName)
{
	ASSERT(m_hstmt != SQL_NULL_HSTMT);
	m_rc = ::SQLColumns(m_hstmt,
						szTableQualifier, cbTableQualifier,
						szTableOwner, cbTableOwner,
						szTableName, cbTableName,
						szColumnName, cbColumnName);
	CHECK_EX(SQLCOLUMNS);
	return m_rc;
}

inline SQLRETURN CSTMT::DescribeCol(SQLUSMALLINT     icol,
									SQLTCHAR *    szColName,
									SQLSMALLINT    cbColNameMax,
									LPSWORD  pcbColName,
									LPSWORD  pfSqlType,
									SQLULEN * pcbColDef,
									LPSWORD  pibScale,
									LPSWORD  pfNullable)
{
	ASSERT(m_hstmt != SQL_NULL_HSTMT);
	m_rc = ::SQLDescribeCol(m_hstmt, icol,
							szColName, cbColNameMax,
							pcbColName, pfSqlType, pcbColDef,
							pibScale, pfNullable);
	CHECK_EX(SQLDESCRIBECOL);
	return m_rc;
}

inline SQLRETURN CSTMT::DescribeParam(SQLUSMALLINT   ipar,
									LPSWORD  pfSqlType,
									SQLULEN * pcbColDef,
									LPSWORD  pibScale,
									LPSWORD  pfNullable)
{
	ASSERT(m_hstmt != SQL_NULL_HSTMT);
	m_rc = ::SQLDescribeParam(m_hstmt, ipar, pfSqlType, pcbColDef,
									pibScale, pfNullable);
	CHECK_EX(SQLDESCRIBEPARAM);
	return m_rc;
}

inline SQLRETURN CSTMT::ExecDirect(SQLTCHAR * szSqlStr,
								SQLINTEGER   cbSqlStr)
{
	ASSERT(m_hstmt != SQL_NULL_HSTMT);
	m_rc = ::SQLExecDirect(m_hstmt, szSqlStr, cbSqlStr);
	CHECK_EX(SQLEXECDIRECT);
	return m_rc;
}

inline SQLRETURN CSTMT::Execute()
{
	ASSERT(m_hstmt != SQL_NULL_HSTMT);
	m_rc = ::SQLExecute(m_hstmt);
	CHECK_EX(SQLEXECUTE);
	return m_rc;
}

inline SQLRETURN CSTMT::ExtendedFetch(SQLUSMALLINT  fFetchType,
									SQLLEN   irow,
									SQLUSMALLINT *rgfRowStatus)
{
	return ExtendedFetch(fFetchType, irow, NULL, rgfRowStatus);
}

inline SQLRETURN CSTMT::ExtendedFetch(SQLUSMALLINT  fFetchType,
									SQLLEN   irow,
									SQLULEN * pcrow,
									SQLUSMALLINT *rgfRowStatus)
{
	ASSERT(m_hstmt != SQL_NULL_HSTMT);
	m_rc = ::SQLExtendedFetch(m_hstmt, fFetchType, irow, pcrow, rgfRowStatus);
	CHECK_EX(SQLEXTENDEDFETCH);
	return m_rc;
}

inline SQLRETURN CSTMT::Fetch()
{
	ASSERT(m_hstmt != SQL_NULL_HSTMT);
	m_rc = ::SQLFetch(m_hstmt);
	CHECK_EX(SQLFETCH);
	return m_rc;
}

inline SQLRETURN CSTMT::Fetch(SQLUSMALLINT  fFetchType,
							SQLLEN   irow,
							SQLUSMALLINT *rgfRowStatus)
{
	return ExtendedFetch(fFetchType, irow, NULL, rgfRowStatus);
}

inline SQLRETURN CSTMT::Fetch(SQLUSMALLINT  fFetchType,
							SQLLEN   irow,
							SQLULEN * pcrow,
							SQLUSMALLINT *rgfRowStatus)
{
	return ExtendedFetch(fFetchType, irow, pcrow, rgfRowStatus);
}

inline SQLRETURN CSTMT::ForeignKeys(SQLTCHAR * szPkTableQualifier,
									SQLTCHAR * szPkTableOwner,
									SQLTCHAR * szPkTableName,
									SQLTCHAR * szFkTableQualifier,
									SQLTCHAR * szFkTableOwner,
									SQLTCHAR * szFkTableName)
{
	return ForeignKeys(szPkTableQualifier, SQL_NTS,
					szPkTableOwner, SQL_NTS,
					szPkTableName, SQL_NTS,
					szFkTableQualifier, SQL_NTS,
					szFkTableOwner, SQL_NTS,
					szFkTableName, SQL_NTS);
						
}

inline SQLRETURN CSTMT::ForeignKeys(SQLTCHAR * szPkTableQualifier,
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
									SQLSMALLINT  cbFkTableName)
{
	ASSERT(m_hstmt != SQL_NULL_HSTMT);
	m_rc = ::SQLForeignKeys(m_hstmt,
						szPkTableQualifier, cbPkTableQualifier,
						szPkTableOwner, cbPkTableOwner,
						szPkTableName, cbPkTableName,
						szFkTableQualifier, cbFkTableQualifier,
						szFkTableOwner, cbFkTableOwner,
						szFkTableName, cbFkTableName);
	CHECK_EX(SQLFOREIGNKEYS);
	return m_rc;
}

inline SQLRETURN CSTMT::Close()
{
	return FreeStmt(SQL_CLOSE);
}

inline SQLRETURN CSTMT::Unbind()
{
	return FreeStmt(SQL_UNBIND);
}

inline SQLRETURN CSTMT::ResetParams()
{
	return FreeStmt(SQL_RESET_PARAMS);
}

inline SQLRETURN CSTMT::FreeStmt(SQLUSMALLINT fOption)
{
	ASSERT(m_hstmt != SQL_NULL_HSTMT);
	ASSERT(fOption != SQL_DROP);
	m_rc = ::SQLFreeStmt(m_hstmt, fOption);
	CHECK_EX(SQLFREESTMT);
	return m_rc;
}

inline SQLRETURN CSTMT::GetCursorName(SQLTCHAR * szCursor,
									SQLSMALLINT   cbCursorMax,
									LPSWORD pcbCursor)
{
	ASSERT(m_hstmt != SQL_NULL_HSTMT);
	m_rc = ::SQLGetCursorName(m_hstmt, szCursor, cbCursorMax, pcbCursor);
	CHECK_EX(SQLGETCURSORNAME);
	return m_rc;
}

inline SQLRETURN CSTMT::GetData(SQLUSMALLINT    icol,
								SQLSMALLINT    fCType,
								PTR      rgbValue,
								SQLLEN   cbValueMax,
								SQLLEN * pcbValue)
{
	ASSERT(m_hstmt != SQL_NULL_HSTMT);
	m_rc = ::SQLGetData(m_hstmt, icol, fCType, rgbValue, cbValueMax, pcbValue);
	CHECK_EX(SQLGETDATA);
	return m_rc;
}

inline SQLRETURN CSTMT::GetStmtOption(SQLUSMALLINT fOption, PTR pvParam)
{
	ASSERT(m_hstmt != SQL_NULL_HSTMT);
	m_rc = ::SQLGetStmtOption(m_hstmt, fOption, pvParam);
	CHECK_EX(SQLGETSTMTOPTION);
	return m_rc;
}

inline SQLRETURN CSTMT::GetTypeInfo(SQLSMALLINT fSqlType)
{
	ASSERT(m_hstmt != SQL_NULL_HSTMT);
	m_rc = ::SQLGetTypeInfo(m_hstmt, fSqlType);
	CHECK_EX(SQLGETINFO);
	return m_rc;
}

inline SQLRETURN CSTMT::MoreResults()
{
	ASSERT(m_hstmt != SQL_NULL_HSTMT);
	m_rc = ::SQLMoreResults(m_hstmt);
	CHECK_EX(SQLMORERESULTS);
	return m_rc;
}

inline SQLRETURN CSTMT::NumParams(LPSWORD pcpar)
{
	ASSERT(m_hstmt != SQL_NULL_HSTMT);
	m_rc = ::SQLNumParams(m_hstmt, pcpar);
	CHECK_EX(SQLNUMPARAMS);
	return m_rc;
}

inline SQLRETURN CSTMT::NumResultCols(LPSWORD pccol)
{
	ASSERT(m_hstmt != SQL_NULL_HSTMT);
	m_rc = ::SQLNumResultCols(m_hstmt, pccol);
	CHECK_EX(SQLNUMRESULTCOLS);
	return m_rc;
}

inline SQLRETURN CSTMT::ParamData(LPPTR prgbValue)
{
	ASSERT(m_hstmt != SQL_NULL_HSTMT);
	m_rc = ::SQLParamData(m_hstmt, prgbValue);
	CHECK_EX(SQLPARAMDATA);
	return m_rc;
}

inline SQLRETURN CSTMT::ParamOptions(SQLUINTEGER crow, SQLULEN * pirow)
{
	ASSERT(m_hstmt != SQL_NULL_HSTMT);
	m_rc = ::SQLParamOptions(m_hstmt, crow, pirow);
	CHECK_EX(SQLPARAMOPTIONS);
	return m_rc;
}

inline SQLRETURN CSTMT::Prepare(SQLTCHAR * szSqlStr,
								SQLINTEGER cbSqlStr)
{
	ASSERT(m_hstmt != SQL_NULL_HSTMT);
	m_rc = ::SQLPrepare(m_hstmt, szSqlStr, cbSqlStr);
	CHECK_EX(SQLPREPARE);
	return m_rc;
}

inline SQLRETURN CSTMT::PrimaryKeys(SQLTCHAR * szTableQualifier,
									SQLTCHAR * szTableOwner,
									SQLTCHAR * szTableName)
{
	return PrimaryKeys(szTableQualifier, SQL_NTS,
						szTableOwner, SQL_NTS,
						szTableName, SQL_NTS);
}

inline SQLRETURN CSTMT::PrimaryKeys(SQLTCHAR * szTableQualifier,
									SQLSMALLINT  cbTableQualifier,
									SQLTCHAR * szTableOwner,
									SQLSMALLINT  cbTableOwner,
									SQLTCHAR * szTableName,
									SQLSMALLINT  cbTableName)
{
	ASSERT(m_hstmt != SQL_NULL_HSTMT);
	m_rc = ::SQLPrimaryKeys(m_hstmt,
							szTableQualifier, cbTableQualifier,
							szTableOwner, cbTableOwner,
							szTableName, cbTableName);
	CHECK_EX(SQLPRIMARYKEYS);
	return m_rc;
}

inline SQLRETURN CSTMT::ProcedureColumns(SQLTCHAR * szProcQualifier,
										SQLTCHAR *  szProcOwner,
										SQLTCHAR *  szProcName,
										SQLTCHAR *  szColumnName)
{
	return ProcedureColumns(szProcQualifier, szProcOwner,
							szProcName, szColumnName);
}

inline SQLRETURN CSTMT::ProcedureColumns(SQLTCHAR * szProcQualifier,
										SQLSMALLINT   cbProcQualifier,
										SQLTCHAR *  szProcOwner,
										SQLSMALLINT   cbProcOwner,
										SQLTCHAR *  szProcName,
										SQLSMALLINT   cbProcName,
										SQLTCHAR *  szColumnName,
										SQLSMALLINT   cbColumnName)
{
	ASSERT(m_hstmt != SQL_NULL_HSTMT);
	m_rc = ::SQLProcedureColumns(m_hstmt,
							szProcQualifier, cbProcQualifier,
							szProcOwner, cbProcOwner,
							szProcName, cbProcName,
							szColumnName, cbColumnName);
	CHECK_EX(SQLPROCEDURECOLUMNS);
	return m_rc;
}

inline SQLRETURN CSTMT::Procedures(SQLTCHAR * szProcQualifier,
								SQLTCHAR *    szProcOwner,
								SQLTCHAR *    szProcName)
{
	return Procedures(szProcQualifier, SQL_NTS,
					szProcOwner, SQL_NTS,
					szProcName, SQL_NTS);
}

inline SQLRETURN CSTMT::Procedures(SQLTCHAR * szProcQualifier,
								SQLSMALLINT     cbProcQualifier,
								SQLTCHAR *    szProcOwner,
								SQLSMALLINT     cbProcOwner,
								SQLTCHAR *    szProcName,
								SQLSMALLINT     cbProcName)
{
	ASSERT(m_hstmt != SQL_NULL_HSTMT);
	m_rc = ::SQLProcedures(m_hstmt,
						szProcQualifier, cbProcQualifier,
						szProcOwner, cbProcOwner,
						szProcName, cbProcName);
	CHECK_EX(SQLPROCEDURES);
	return m_rc;
}

inline SQLRETURN CSTMT::PutData(PTR rgbValue, SQLLEN cbValue)
{
	ASSERT(m_hstmt != SQL_NULL_HSTMT);
	m_rc = ::SQLPutData(m_hstmt, rgbValue, cbValue);
	CHECK_EX(SQLPUTDATA);
	return m_rc;
}

inline SQLRETURN CSTMT::RowCount(SQLLEN * pcrow)
{
	ASSERT(m_hstmt != SQL_NULL_HSTMT);
	m_rc = ::SQLRowCount(m_hstmt, pcrow);
	CHECK_EX(SQLROWCOUNT);
	return m_rc;
}

inline SQLRETURN CSTMT::SetCursorName(SQLTCHAR * szCursor,
									SQLSMALLINT    cbCursor)
{
	ASSERT(m_hstmt != SQL_NULL_HSTMT);
	m_rc = ::SQLSetCursorName(m_hstmt, szCursor, cbCursor);
	CHECK_EX(SQLSETCURSORNAME);
	return m_rc;
}

inline SQLRETURN CSTMT::SetParam(SQLUSMALLINT   ipar,
								SQLSMALLINT    fCType,
								SQLSMALLINT    fSqlType,
								SQLULEN   cbColDef,
								SQLSMALLINT    ibScale,
								PTR      rgbValue,
								SQLLEN * pcbValue)
{
	ASSERT(m_hstmt != SQL_NULL_HSTMT);
	m_rc = ::SQLSetParam(m_hstmt, ipar, fCType, fSqlType,
						cbColDef, ibScale, rgbValue, pcbValue);
	CHECK_EX(SQLSETPARAM);
	return m_rc;
}

inline SQLRETURN CSTMT::SetPos(SQLUSMALLINT irow,
								BOOL fRefresh,
								BOOL fLock)
{
	ASSERT(m_hstmt != SQL_NULL_HSTMT);
	m_rc = ::SQLSetPos(m_hstmt, irow, fRefresh, fLock);
	CHECK_EX(SQLSETPOS);
	return m_rc;
}

inline SQLRETURN CSTMT::SetScrollOptions(SQLUSMALLINT fConcurrency,
										SQLLEN crowKeyset,
										SQLUSMALLINT  crowRowset)
{
	ASSERT(m_hstmt != SQL_NULL_HSTMT);
	m_rc = ::SQLSetScrollOptions(m_hstmt, fConcurrency,
								crowKeyset, crowRowset);
	CHECK_EX(SQLSETSCROLLOPTIONS);
	return m_rc;
}


inline SQLRETURN CSTMT::SetStmtOption(SQLUSMALLINT fOption, SQLULEN vParam)
{
	ASSERT(m_hstmt != SQL_NULL_HSTMT);
	m_rc = ::SQLSetStmtOption(m_hstmt, fOption, vParam);
	CHECK_EX(SQLSETSTMTOPTION);
	return m_rc;
}

inline SQLRETURN CSTMT::SpecialColumns(SQLUSMALLINT fColType,
									SQLTCHAR *   szTableQualifier,
									SQLTCHAR *   szTableOwner,
									SQLTCHAR *   szTableName,
									SQLUSMALLINT    fScope,
									SQLUSMALLINT    fNullable)
{
	return SpecialColumns(fColType, szTableQualifier, SQL_NTS,
						szTableOwner, SQL_NTS,
						szTableName, SQL_NTS,
						fScope, fNullable);
}

inline SQLRETURN CSTMT::SpecialColumns(SQLUSMALLINT fColType,
									SQLTCHAR *   szTableQualifier,
									SQLSMALLINT    cbTableQualifier,
									SQLTCHAR *   szTableOwner,
									SQLSMALLINT    cbTableOwner,
									SQLTCHAR *   szTableName,
									SQLSMALLINT    cbTableName,
									SQLUSMALLINT    fScope,
									SQLUSMALLINT    fNullable)
{
	ASSERT(m_hstmt != SQL_NULL_HSTMT);
	m_rc = ::SQLSpecialColumns(m_hstmt, fColType,
							szTableQualifier, cbTableQualifier,
							szTableOwner, cbTableOwner,
							szTableName, cbTableName,
							fScope, fNullable);
	CHECK_EX(SQLSPECIALCOLUMNS);
	return m_rc;
}

inline SQLRETURN CSTMT::Statistics(SQLTCHAR *  szTableQualifier,
									SQLTCHAR * szTableOwner,
									SQLTCHAR * szTableName,
									SQLUSMALLINT  fUnique,
									SQLUSMALLINT  fAccuracy)
{
	return Statistics(szTableQualifier, SQL_NTS,
					szTableOwner, SQL_NTS,
					szTableName, SQL_NTS,
					fUnique, fAccuracy);
}

inline SQLRETURN CSTMT::Statistics(SQLTCHAR *  szTableQualifier,
									SQLSMALLINT  cbTableQualifier,
									SQLTCHAR * szTableOwner,
									SQLSMALLINT  cbTableOwner,
									SQLTCHAR * szTableName,
									SQLSMALLINT  cbTableName,
									SQLUSMALLINT  fUnique,
									SQLUSMALLINT  fAccuracy)
{
	ASSERT(m_hstmt != SQL_NULL_HSTMT);
	m_rc = ::SQLStatistics(m_hstmt,
						szTableQualifier, cbTableQualifier,
						szTableOwner, cbTableOwner,
						szTableName, cbTableName,
						fUnique, fAccuracy);
	CHECK_EX(SQLSTATISTICS);
	return m_rc;
}

inline SQLRETURN CSTMT::TablePrivileges(SQLTCHAR * szTableQualifier,
										SQLTCHAR * szTableOwner,
										SQLTCHAR * szTableName)
{
	return TablePrivileges(szTableQualifier, SQL_NTS,
							szTableOwner, SQL_NTS,
							szTableName, SQL_NTS);
}

inline SQLRETURN CSTMT::TablePrivileges(SQLTCHAR * szTableQualifier,
										SQLSMALLINT  cbTableQualifier,
										SQLTCHAR * szTableOwner,
										SQLSMALLINT  cbTableOwner,
										SQLTCHAR * szTableName,
										SQLSMALLINT  cbTableName)
{
	ASSERT(m_hstmt != SQL_NULL_HSTMT);
	m_rc = ::SQLTablePrivileges(m_hstmt,
						szTableQualifier, cbTableQualifier,
						szTableOwner, cbTableOwner,
						szTableName, cbTableName);
	CHECK_EX(SQLTABLEPRIVILEGES);
	return m_rc;
}

inline SQLRETURN CSTMT::Tables(SQLTCHAR *  szTableQualifier,
								SQLTCHAR * szTableOwner,
								SQLTCHAR * szTableName,
								SQLTCHAR * szTableType)
{
	return Tables(szTableQualifier, SQL_NTS,
				szTableOwner, SQL_NTS,
				szTableName, SQL_NTS,
				szTableType, SQL_NTS);
}

inline SQLRETURN CSTMT::Tables(SQLTCHAR *  szTableQualifier,
								SQLSMALLINT  cbTableQualifier,
								SQLTCHAR * szTableOwner,
								SQLSMALLINT  cbTableOwner,
								SQLTCHAR * szTableName,
								SQLSMALLINT  cbTableName,
								SQLTCHAR * szTableType,
								SQLSMALLINT  cbTableType)
{
	ASSERT(m_hstmt != SQL_NULL_HSTMT);
	m_rc = ::SQLTables(m_hstmt,
						szTableQualifier, cbTableQualifier,
						szTableOwner, cbTableOwner,
						szTableName, cbTableName,
						szTableType, cbTableType);
	CHECK_EX(SQLTABLES);
	return m_rc;
}


#endif // __CODBC_INL
