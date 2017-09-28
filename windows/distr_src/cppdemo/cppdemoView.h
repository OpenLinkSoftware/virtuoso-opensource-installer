// cppdemoView.h : interface of the CCppdemoView class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once


#include <atltime.h>
#include "ListCtrl.h"


class CColHeader
{
public:
	CColHeader()
	{
		m_label = _T( "#" );
		m_width = 6;
	}
	
	CColHeader(LPCTSTR lpszLabel, int width )
	{
		m_label = lpszLabel;
		m_width = width;
	}

	CString m_label;
	int m_width;
};


class CCppdemoView : public CDialogImpl<CCppdemoView>,
	public CDialogResize<CCppdemoView>
{
public:
	CCppdemoView()
	{
		m_wndListCtrl.RegisterClass();
		cEnv = NULL;
		cDbc = NULL;
		cStmt = NULL;
	
		fConnected = FALSE;
	    fData = FALSE;
		fResultSet = FALSE;
		//		m_bShowMessage = TRUE;
		//		m_bShowThemed = TRUE;
	}

	enum { IDD = IDD_CPPDEMO_FORM };

	BOOL PreTranslateMessage(MSG* pMsg)
	{
		return CWindow::IsDialogMessage(pMsg);
	}

	BEGIN_DLGRESIZE_MAP(CCppdemoView)
		DLGRESIZE_CONTROL(IDC_LISTCTRL, DLSZ_SIZE_X|DLSZ_SIZE_Y)
	END_DLGRESIZE_MAP()

	BEGIN_MSG_MAP(CCppdemoView)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		CHAIN_MSG_MAP(CDialogResize<CCppdemoView>)
		NOTIFY_HANDLER_EX(IDC_LISTCTRL, LCN_HYPERLINK, OnListHyperLink)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()


	// Handler prototypes (uncomment arguments if needed):
	//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

protected:
	CListCtrl m_wndListCtrl;
	CFont m_fntLink;
	CFont m_fntCustomFont1;
	CFont m_fntCustomFont2;
	//	BOOL m_bShowMessage;
	//	BOOL m_bShowThemed;

	CENV*		cEnv;				// Environment object
	CDBC*		cDbc;				// Connection object
	CSTMT*		cStmt;				// Statement object
	
	BOOL		fConnected;			// Connected to data source
	BOOL		fData;				// Data fetched from data source
	BOOL		fResultSet;			// Result set exists
	char		szDSN[SQL_MAX_DSN_LENGTH+1];// DSN buffer

    COLORREF m_rgbBackground;
    COLORREF m_rgbHyperLink;


	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		DlgResize_Init();

		m_wndListCtrl.SubclassWindow( GetDlgItem( IDC_LISTCTRL ) );

		m_wndListCtrl.SetFocusSubItem( TRUE );
		m_wndListCtrl.SetSingleSelect(TRUE);
		m_wndListCtrl.SetDragDrop(TRUE);

		cEnv = new CENV;
		if( !cEnv || !cEnv->Success(cEnv->m_rc))
			return FALSE;

		m_rgbBackground = GetSysColor( COLOR_WINDOW );
		m_rgbHyperLink = RGB( 0, 0, 200 );

/**
		HFONT fnt = m_wndListCtrl.GetFont();
		LOGFONT logFont = { 0 };
		if (::GetObject(fnt, &logFont))
		{
			logFont.lfUnderline = (BYTE)TRUE;
		}
**/		
		return TRUE;
	}


#if 0
	LRESULT OnTheme(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		//		m_bShowThemed = !m_bShowThemed;
		m_wndListCtrl.ShowThemed(TRUE);
		return 0;
	}
#endif

	LRESULT OnListHyperLink( LPNMHDR lpNMHDR )
	{
		CListNotify *pListNotify = reinterpret_cast<CListNotify *>( lpNMHDR );
		CString link = m_wndListCtrl.GetItemText(pListNotify->m_nItem, pListNotify->m_nSubItem);

#ifndef _WIN32_WCE
		DWORD_PTR dwRet = (DWORD_PTR)::ShellExecute(0, _T("open"), (LPCTSTR)link, 0, 0, SW_SHOWNORMAL);
#else // CE specific
		SHELLEXECUTEINFO shExeInfo = { sizeof(SHELLEXECUTEINFO), 0, 0, L"open", (LPCTSTR)link, 0, 0, SW_SHOWNORMAL, 0, 0, 0, 0, 0, 0, 0 };
		::ShellExecuteEx(&shExeInfo);
		DWORD_PTR dwRet = (DWORD_PTR)shExeInfo.hInstApp;
#endif // _WIN32_WCE

		return 0;
	}	
	

public:
	void ClearGrid()
	{
		m_wndListCtrl.DeleteAll();
	}

	void SetColumns(CListArray<CColHeader> &hdr)
	{
		m_wndListCtrl.SetFocusSubItem( TRUE );
		int cw = m_wndListCtrl.GetCharWidth();
		for(int i=0; i < hdr.GetSize(); i++) 
		{
			m_wndListCtrl.AddColumn( hdr[i].m_label, hdr[i].m_width*cw);
		}
	}

	void AddRow(CListArray<CString> &row)
	{
		CString v;
		int sz = row.GetSize();
		if (sz == 0)
			return;

		int nNewItem = m_wndListCtrl.AddItem(row[0]);
		v = row[0];
		if (_tcsnicmp((LPCTSTR)v,_T("http://"),7)==0 || _tcsnicmp((LPCTSTR)v,_T("https://"),7)==0) {
			m_wndListCtrl.SetItemFormat( nNewItem, 0, ITEM_FORMAT_HYPERLINK );
		    m_wndListCtrl.SetItemColours(nNewItem, 0, m_rgbBackground, m_rgbHyperLink);
		}

		for(int i=1; i < sz; i++)
		{
			v = row[i];
			m_wndListCtrl.SetItemText( nNewItem, i, row[i] );
			if (_tcsnicmp((LPCTSTR)v,_T("http://"),7)==0 || _tcsnicmp((LPCTSTR)v,_T("https://"),7)==0) {
				m_wndListCtrl.SetItemFormat( nNewItem, i, ITEM_FORMAT_HYPERLINK );
				m_wndListCtrl.SetItemColours(nNewItem, i, m_rgbBackground, m_rgbHyperLink);
			}
		}
	}


};


