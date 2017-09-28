// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#define MAX_WIDTH       80


typedef struct tagErrorText
    {
    TCHAR        szText[1024];
    TCHAR        szFmt[cbSTRLEN];
    TCHAR        szSQLState[6];
    TCHAR        szError[SQL_MAX_MESSAGE_LENGTH];
    }
    ERRORTEXT, *LPERRORTEXT;


class CMainFrame : 
	public CFrameWindowImpl<CMainFrame>, 
	public CUpdateUI<CMainFrame>,
	public CMessageFilter, public CIdleHandler
{
public:
	CMainFrame()
	{
		cEnv = NULL;
		cDbc = NULL;
		cStmt = NULL;
	
		fConnected = FALSE;
	    fData = FALSE;
		fResultSet = FALSE;
	}

	
	DECLARE_FRAME_WND_CLASS(NULL, IDR_MAINFRAME)

	CCppdemoView m_view;
	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		if(CFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg))
			return TRUE;

		return m_view.PreTranslateMessage(pMsg);
	}

	virtual BOOL OnIdle()
	{
		UIUpdateToolBar();
		return FALSE;
	}
	BEGIN_UPDATE_UI_MAP(CMainFrame)
		UPDATE_ELEMENT(ID_VIEW_TOOLBAR, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_STATUS_BAR, UPDUI_MENUPOPUP)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainFrame)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		COMMAND_ID_HANDLER(ID_APP_EXIT, OnFileExit)
		COMMAND_ID_HANDLER(ID_FILE_NEW, OnFileNew)
		COMMAND_ID_HANDLER(ID_VIEW_TOOLBAR, OnViewToolBar)
		COMMAND_ID_HANDLER(ID_VIEW_STATUS_BAR, OnViewStatusBar)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(ID_ENV_ADD, OnEnvAdd)
		COMMAND_ID_HANDLER(ID_ENV_OPEN, OnEnvOpen)
		COMMAND_ID_HANDLER(ID_ENV_CLOSE, OnEnvClose)
		COMMAND_ID_HANDLER(ID_SQL_EXEC, OnSqlExec)
		COMMAND_ID_HANDLER(ID_SQL_MORE_RESULT, OnSqlMore)
		CHAIN_MSG_MAP(CUpdateUI<CMainFrame>)
		CHAIN_MSG_MAP(CFrameWindowImpl<CMainFrame>)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)


	CENV*		cEnv;				// Environment object
	CDBC*		cDbc;				// Connection object
	CSTMT*		cStmt;				// Statement object
	
	BOOL		fConnected;			// Connected to data source
	BOOL		fData;				// Data fetched from data source
	BOOL		fResultSet;			// Result set exists
	TCHAR		szDSN[SQL_MAX_DSN_LENGTH+1];// DSN buffer
	CString		szSQL;
	TCHAR		szNull[cbSTRLEN];	// <NULL> string
	UWORD		cMaxRow;



	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CreateSimpleStatusBar();

		m_hWndClient = m_view.Create(m_hWnd);

		UISetCheck(ID_VIEW_STATUS_BAR, 1);

		// register object for message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->AddMessageFilter(this);
		pLoop->AddIdleHandler(this);

		cEnv = new CENV;
		if( !cEnv || !cEnv->Success(cEnv->m_rc))
			return -1;

		szSQL.LoadString(IDS_SQLDEF);
		vEnableMenus();

		CString title;
		title.LoadString(IDS_MAIN);
		this->SetWindowText(title);

		AtlLoadString(IDS_SZNULL, szNull, cbSTRLEN);

		cMaxRow = 1000;

		return 0;
	}

	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		// unregister message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->RemoveMessageFilter(this);
		pLoop->RemoveIdleHandler(this);

		bHandled = FALSE;
		return 1;
	}

	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL &bHandled)
	{
		LRESULT iRes;
		CConfirmDlg dlg;
		iRes = dlg.DoModal();
		UpdateWindow();
		if( iRes == IDOK && fDisconnect())
			bHandled = FALSE;
		return 0;
	}

	LRESULT OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		PostMessage(WM_CLOSE);
		return 0;
	}

	LRESULT OnFileNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		// TODO: add code to initialize document

		return 0;
	}

	LRESULT OnViewToolBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		BOOL bVisible = !::IsWindowVisible(m_hWndToolBar);
		::ShowWindow(m_hWndToolBar, bVisible ? SW_SHOWNOACTIVATE : SW_HIDE);
		UISetCheck(ID_VIEW_TOOLBAR, bVisible);
		UpdateLayout();
		return 0;
	}

	LRESULT OnViewStatusBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		BOOL bVisible = !::IsWindowVisible(m_hWndStatusBar);
		::ShowWindow(m_hWndStatusBar, bVisible ? SW_SHOWNOACTIVATE : SW_HIDE);
		UISetCheck(ID_VIEW_STATUS_BAR, bVisible);
		UpdateLayout();
		return 0;
	}

	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CAboutDlg dlg;
		dlg.DoModal();
		return 0;
	}

	LRESULT OnEnvAdd(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		SQLCreateDataSource(m_hWnd, NULL);
		return TRUE;
	}

	LRESULT OnEnvOpen(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		// Disconnect in order to reconnect..
		if(fConnected )
		{
			LRESULT iRes;

			// Bring up a confirmation dialog box
			CConfirmDlg dlg;
			iRes = dlg.DoModal();
			UpdateWindow();

			if( iRes != IDOK || !fDisconnect() )
				return TRUE;

			// Disable appropriate menu items, in
			// case the new connection fails
			vEnableMenus();
		}

		// Make the connection and enable the appropriate menu
		// items if it is successfully made

		if( fConnect() )
			vEnableMenus();
		return TRUE;
	}

	LRESULT OnEnvClose(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		LRESULT iRes;

		// If we're not connected, there's something wrong

		DASSERT(fConnected);

		// Bring up a confirmation dialog box
		CConfirmDlg dlg;
		iRes = dlg.DoModal();
		UpdateWindow();

		// Disconnect and disable appropriate menu
		// items if the disconnection was successful

		if( iRes == IDOK && fDisconnect() )
			vEnableMenus();

		return TRUE;
	}

	LRESULT OnSqlExec(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		LRESULT iRes;
		DASSERT(fConnected && cStmt);

		// Bring up the SQL statement dialog box and execute
		// a SQL statement if the user gives us one to use

		CSqlBoxDlg dlg((LPCTSTR)szSQL, cMaxRow);
		iRes = dlg.DoModal();
		if( iRes != IDOK )
			return 0;
	        szSQL = dlg.m_sql_text;
		cMaxRow = dlg.m_max_rows;

		UpdateWindow();
		vDoSQL();

		// Enable menu items
		vEnableMenus();
		return 0;
	}

	LRESULT OnSqlMore(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		vDoMoreResult();
		return 0;
	}


	//////////////////////////////////////////////////////////////////////////////
	//
	//	fConnect
	//
	//      Open a connection to a data source.  Assumes that there is currently
	//      no connection.
	//
	//  Params:
	//      hwnd        -- window handle
	//
	//  Returns:
	//      BOOL        -- TRUE if connection succeeded, FALSE otherwise
	//
	BOOL fConnect()
	{
		SQLRETURN     rc;                 // SQL return code
		SQLTCHAR     sz[cbMAXSQL];       // DSN string
		HCURSOR     hCurs;              // cursor handle
		SWORD       swInfoValue;        // total # of bytes available to return

		// If we're already connected, that's bad
		DASSERT(!fConnected);

		// Allocate a new connection
		HOURGLASS(hCurs);								
		cDbc = cEnv->AllocConnect();
		ARROW(hCurs);

		// If that's not successful, return failure
		if( !cDbc )
		{
			vDoMessage(IDS_NOMEMORY);
			return FALSE;
		}
		else if( fODBCError(cDbc->m_rc) )
		{
			delete cDbc;
			cDbc = NULL;
			return FALSE;	
		}
		// Don't use the cursor library for this connection, since there's no
		// need for it; make the connection to a user-chosen data source
		HOURGLASS(hCurs);
		rc = cDbc->DriverConnect(m_hWnd, NULL, sz, sizeof(sz)/sizeof(SQLTCHAR), SQL_DRIVER_COMPLETE);
		ARROW(hCurs);

		// Check for errors in making the connection
		if( rc == SQL_NO_DATA_FOUND || fODBCError(rc) )
		{
			delete cDbc;
			cDbc = NULL;
			return FALSE;
		}
		// Get the data source name
		*szDSN = '\0';
		fODBCError(cDbc->GetInfo(SQL_DATA_SOURCE_NAME, szDSN, (SQL_MAX_DSN_LENGTH + 1)*sizeof(SQLTCHAR),	&swInfoValue));

		if( !(*szDSN) )
			lstrcpy(szDSN, szNODSN);

		// Allocate a statement

		HOURGLASS(hCurs);
		cStmt = cDbc->AllocStmt();
		ARROW(hCurs);

		// See if there was a problem with the statement allocation..

		if( !cStmt )
		{
			delete cDbc;
			cDbc = NULL;
			vDoMessage(IDS_NOMEMORY);
			return FALSE;
		}
		else if( fODBCError(cStmt->m_rc) )
		{
			delete cDbc;
			cDbc = NULL;
			delete cStmt;
			cStmt = NULL;
			return FALSE;
		}

		// Finally, the connection is good at this point
		fConnected = TRUE;

		// Add the DSN to the window title
		CString title;
		title.LoadString(IDS_MAIN);
		this->SetWindowText(title + szDASH + szDSN);

		return TRUE;
	}


	//////////////////////////////////////////////////////////////////////////////
	//
	//	fDisconnect
	//
	//      Close a connection to a data source.
	//
	//  Params:
	//      hwnd        -- window handle
	//
	//  Returns:
	//      BOOL        -- TRUE if the disconnect succeeded, FALSE otherwise
	//
	BOOL INTFUNC fDisconnect()
	{
		HCURSOR     hCurs;              // cursor handle

		// If we're not connected, then we're now disconnected :)

		if( !fConnected )
			return TRUE;

		// Release the associated statement resources if we've
		// not already done so..

		if( cStmt )
		{
			vFreeStmt(SQL_DROP);
			delete cStmt;
			cStmt = NULL;
		}

		// Disconnect..

		HOURGLASS(hCurs);
		if( !fODBCError(cDbc->Disconnect()) )
		{
			delete cDbc;
			cDbc = NULL;
		}
		else
		{
			ARROW(hCurs);
			vEnableMenus();
			return FALSE;
		}
		ARROW(hCurs);

		// Flag the connection closed and remove the DSN from the
		// Window title bar display

		fConnected = FALSE;
		szDSN[0] = '\0';

		CString title;
		title.LoadString(IDS_MAIN);
		this->SetWindowText(title);

		return TRUE;
	}


	//////////////////////////////////////////////////////////////////////////////
	//
	//	vDoMessage
	//
	//      Put up an informational message box.
	//
	//  Params:
	//      hwnd        -- window handle
	//      id          -- string resource ID
	//
	//  Returns:
	//      none
	//
	void INTFUNC vDoMessage(UINT id)
	{
		AtlMessageBox(m_hWnd, id, IDS_MAIN);
	}

	//////////////////////////////////////////////////////////////////////////////
	//
	//	vEnableMenus
	//
	//      Enable all connection-related menu items.
	//
	//  Params:
	//      hwnd        -- window handle
	//
	//  Returns:
	//      none
	//
	void INTFUNC vEnableMenus()
	{
		HMENU hmenu = GetMenu();

		if(fConnected )
		{
			EnableMenuItem(hmenu, ID_ENV_CLOSE, 	MF_ENABLE);
			EnableMenuItem(hmenu, ID_SQL_EXEC,		MF_ENABLE);
			if( fResultSet)
				EnableMenuItem(hmenu, ID_SQL_FETCH,	MF_ENABLE);
			else
				EnableMenuItem(hmenu, ID_SQL_FETCH,	MF_DISABLE);
		}
		else
		{
			EnableMenuItem(hmenu, ID_ENV_CLOSE, 	MF_DISABLE);
			EnableMenuItem(hmenu, ID_SQL_EXEC,		MF_DISABLE);
			EnableMenuItem(hmenu, ID_SQL_FETCH,		MF_DISABLE);
		}

		if(fConnected && !cStmt )
		{
			EnableMenuItem(hmenu, ID_SQL_EXEC,		MF_DISABLE);
			EnableMenuItem(hmenu, ID_SQL_FETCH,		MF_DISABLE);
		}
		return;
	}

	//////////////////////////////////////////////////////////////////////////////
	//
	//	fODBCError
	//
	//		Check for an ODBC error; SQL_NO_DATA_FOUND and SQL_STILL_EXECUTING
	//		are not considered errors.  If an error is found, bring up a message
	//      box detailing the error.  In the case of SQL_SUCCESS_WITH_INFO,
	//      a message box with the additional information is shown, but this is
	//      also not considered an error.
	//
	//  Params:
	//      hwnd        -- window handle
	//      rc          -- SQL return code to check
	//
	//  Returns:
	//      BOOL        -- TRUE if there was an ODBC error, FALSE otherwise
	//
	BOOL INTFUNC fODBCError(SQLRETURN rc)
	{
		SDWORD      fNative;            // native error code
		SWORD       cbError;            // length of returned error string
		ERRORTEXT	tErr;              // error text structure
		HENV        henv;               // environment handle
		HDBC        hdbc;               // connection handle
		HSTMT       hstmt;              // statement handle

		// Check for non-errors first

		if( rc == SQL_SUCCESS )
			return FALSE;

		if( rc == SQL_NO_DATA_FOUND )
		{
			vDoMessage(IDS_NODATAFOUND);
			return FALSE;
		}

		if( rc == SQL_STILL_EXECUTING )
		{
			vDoMessage(IDS_STILLEXECUTING);
			return FALSE;
		}            

		AtlLoadString(IDS_MSGFMT,  tErr.szFmt, sizeof(tErr.szFmt)/sizeof(SQLTCHAR));

		// Retrieve and display messages until they're all gone
		henv	= cEnv	? HENV(*(cEnv))		: SQL_NULL_HENV;
		hdbc	= cDbc	? HDBC(*(cDbc))		: SQL_NULL_HDBC;
		hstmt	= cStmt	? HSTMT(*(cStmt))	: SQL_NULL_HSTMT;
		while( SQLError(henv, hdbc, hstmt,
			(SQLTCHAR*)tErr.szSQLState,
			&fNative,
			(SQLTCHAR*)tErr.szError,
			SQL_MAX_MESSAGE_LENGTH - 1,
			&cbError) != SQL_NO_DATA_FOUND )
		{
			if( lstrcmpi(tErr.szSQLState, szDATATRUNC) )
			{
				wsprintf(tErr.szText, tErr.szFmt, tErr.szSQLState,
					fNative, tErr.szError);
				AtlMessageBox(m_hWnd, tErr.szText, IDS_MAIN,
					rc == SQL_SUCCESS_WITH_INFO ?
					MB_ICONINFORMATION | MB_OK :
					MB_ICONEXCLAMATION | MB_OK);
			}
		}

		return (rc != SQL_SUCCESS_WITH_INFO);
	}



	//////////////////////////////////////////////////////////////////////////////
	//
	//	vFreeStmt
	//
	//		Release memory and resources at the same time that we issue
	//      SQLFreeStmt calls.
	//
	//  Params:
	//      hwnd        -- window handle
	//      fOption     -- option flag, either SQL_DROP or SQL_UNBIND
	//
	//  Returns:
	//      none
	//
	void INTFUNC vFreeStmt(UWORD fOption)
	{
		if(!cStmt)                
			return;

		// Issue the SQLFreeStmt call, unless fOption == SQL_DROP, since
		// SQL_DROP will be handled by the destructor for the CSTMT class;
		// in addition, we'll always close the current cursor so that we
		// can be assured of having a clean operation next time

		if( fODBCError(cStmt->Close()) )
			return;
		if( fOption != SQL_DROP )
			if( fODBCError(cStmt->FreeStmt(fOption)) )
				return;

		m_view.ClearGrid();
		UpdateWindow();
		return;
	}



	//////////////////////////////////////////////////////////////////////////////
	//
	//	vDoSQL
	//
	//      Execute a SQL statement and, if there is a result set from this
	//      execution, make bindings, allocate storage, and do the first data
	//      fetch.
	//
	void INTFUNC vDoSQL()
	{
		HCURSOR     hCurs;              // cursor handle
		HOURGLASS(hCurs);

		// If there's already an outstanding result set, unbind it
		if(fResultSet )
			vFreeStmt(SQL_UNBIND);

		// Execute the SQL statement with SQLExecDirect
		LPCTSTR sql = (LPCTSTR)szSQL;
		if( fODBCError(cStmt->ExecDirect((SQLTCHAR*)sql, SQL_NTS)) )
		{
			ARROW(hCurs);
			return;
		}

		vFetchResult();
	}


	//////////////////////////////////////////////////////////////////////////////
	//
	//	vDoMoreResult
	//
	void INTFUNC vDoMoreResult()
	{
		HCURSOR     hCurs;              // cursor handle

		HOURGLASS(hCurs);

		// If there's already an outstanding result set, unbind it
		if(!fResultSet)
			return;

		SQLRETURN rc = cStmt->MoreResults();
		if (rc == SQL_SUCCESS) 
		{
			vFetchResult();
			return;
		}
		if (fODBCError(rc) || rc == SQL_NO_DATA_FOUND)
		{
			m_view.ClearGrid();
		}
		ARROW(hCurs);
	}


	void INTFUNC vFetchResult()
	{
		HCURSOR     hCurs;              // cursor handle
		SWORD       cCol;               // columns in result set

		HOURGLASS(hCurs);

		BOOL sparql_exec = (_tcsnicmp(szSQL.TrimLeft(),_T("sparql"),6)==0);
		// Determine the number of columns in the result set

		cCol = 0;		
		if( fODBCError(cStmt->NumResultCols(&cCol)) )
		{
			ARROW(hCurs);
			return;
		}

		// If there's a row set, we've got data. Otherwise the operation was
		// successful without returning any data (probably not a SELECT,
		// therefore!); display a dialog to indicate this success

		if(cCol )
			fResultSet = TRUE;
		else
		{
			COkDlg dlg;
			dlg.DoModal();
			ARROW(hCurs);
			return;
		}

		CListArray<CColHeader> hdr;
		for(int i=1; i <= cCol; i++)
		{
			TCHAR szName[cbSTRLEN];
			SQLLEN	cb;				// Column width (transfer width)
			SQLLEN	cbc;			// Column width (display  width)
			SQLLEN	fSqlType;		// ODBC SQL data type

			// Get column name
			if( fODBCError(cStmt->ColAttributes(i, SQL_COLUMN_NAME, szName, sizeof(szName)/sizeof(SQLTCHAR), NULL)) )
			{
				vFreeStmt(SQL_DROP);
				ARROW(hCurs);
				return;
			}

			// Get actual column length
			if( fODBCError(cStmt->ColAttributes(i, SQL_COLUMN_LENGTH, &cb)) )
			{
				vFreeStmt(SQL_DROP);
				ARROW(hCurs);
				return;
			}

			// Get display width
			if( fODBCError(cStmt->ColAttributes(i, SQL_COLUMN_DISPLAY_SIZE,	&cbc)) )
			{
				vFreeStmt(SQL_DROP);
				ARROW(hCurs);
				return;
			}

			// All columns need to be at least long enough for the column name
			// and long enough to hold the <NULL> string, just in case

			if( lstrlen(szName) > cbc )
				cbc = lstrlen(szName);
			if( lstrlen(szNull) > cbc )
				cbc = lstrlen(szNull);

			// Get column SQL data type
			if( fODBCError(cStmt->ColAttributes(i, SQL_COLUMN_TYPE,	&fSqlType)) )
			{
				vFreeStmt(SQL_DROP);
				ARROW(hCurs);
				return;
			}

			// If the SQL data type is a CHAR form, make sure that the
			// display width at least as large as the transfer width
			if( (fSqlType == SQL_CHAR || fSqlType == SQL_VARCHAR || fSqlType == SQL_LONGVARCHAR ||
				 fSqlType == SQL_WCHAR || fSqlType == SQL_WVARCHAR || fSqlType == SQL_WLONGVARCHAR) && cbc < cb )
				cbc = cb;

			// Change the transfer length to be the same length as the display
			// width and increase the storage size by one to hold the final \0
			// if this can be done without truncation
			// Otherwise, adjust the size of the transfers so that memory
			// allocations can be done in cbCOLMEMMAX blocks
			cb = cbc;

			if (sparql_exec && cb <= 10)
				cb = MAX_WIDTH;

			if (cb > MAX_WIDTH)
				cb = MAX_WIDTH;

			hdr.Add(CColHeader(szName, cb));
		}

		m_view.ClearGrid();
		m_view.SetColumns(hdr);


		int nRows = 0;
		while(nRows < cMaxRow)
		{
			CListArray<CString> row;

			SQLRETURN sts = cStmt->Fetch();
			if (sts == SQL_NO_DATA_FOUND)
				break;

			if (sts != SQL_SUCCESS && sts != SQL_SUCCESS_WITH_INFO)
			{
				fODBCError(sts);
				vFreeStmt(SQL_DROP);
				ARROW(hCurs);
				return;
			}

			for (int i = 1; i <= cCol; i++)
			{
			    SQLTCHAR fetchBuffer[1024];
			    SQLLEN colInd;

				sts = cStmt->GetData(i, SQL_C_TCHAR, fetchBuffer, sizeof(fetchBuffer), &colInd);
				if (sts != SQL_SUCCESS_WITH_INFO && sts != SQL_SUCCESS)
				{
					fODBCError(sts);
					vFreeStmt(SQL_DROP);
					ARROW(hCurs);
					return;
				}

				/*
				*  Show NULL fields as ****
				*/
				if (colInd == SQL_NULL_DATA)
					row.Add(CString(szNull));
				else
					row.Add(CString(fetchBuffer));
			}
			m_view.AddRow(row);
			nRows++;
		}

		ARROW(hCurs);
		return;
	}


};
