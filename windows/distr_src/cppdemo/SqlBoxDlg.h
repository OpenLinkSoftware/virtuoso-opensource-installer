// aboutdlg.h : interface of the CAboutDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CSqlBoxDlg : public CDialogImpl<CSqlBoxDlg>,
			public CDialogResize<CSqlBoxDlg>

{
public:
	CSqlBoxDlg(LPCTSTR sql, int max_rows)
	{
		m_sql_text = sql;
		m_max_rows = max_rows;
	}

	enum { IDD = IDD_SQLBOX };

	BEGIN_DLGRESIZE_MAP(CSqlBoxDlg)
		DLGRESIZE_CONTROL(IDC_EDIT1, DLSZ_SIZE_X|DLSZ_SIZE_Y)
		DLGRESIZE_CONTROL(IDRUN,	 DLSZ_MOVE_X|DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDCANCEL,  DLSZ_MOVE_X|DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_MAX_LABEL, DLSZ_MOVE_X|DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_MAXROWS,  DLSZ_MOVE_X|DLSZ_MOVE_Y)
	END_DLGRESIZE_MAP()

	BEGIN_MSG_MAP(CSqlBoxDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDRUN, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
		CHAIN_MSG_MAP(CDialogResize<CSqlBoxDlg>)
	END_MSG_MAP()

	CString m_sql_text;
	int		m_max_rows;


protected:
	CEdit   mc_sql_text;
	CEdit   mc_max_rows;

public:
	// Handler prototypes (uncomment arguments if needed):
	//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		DlgResize_Init();
		CenterWindow(GetParent());
		mc_sql_text.Attach(GetDlgItem(IDC_EDIT1));
		mc_sql_text.SetLimitText(32768);
		mc_max_rows.Attach(GetDlgItem(IDC_MAXROWS));
		SetData();
		return TRUE;
	}

	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		GetData();
		EndDialog(wID==IDRUN?IDOK:wID);
		return 0;
	}

	void GetData()
	{
		TCHAR v[32768];
		mc_sql_text.GetWindowText(v, sizeof(v)/sizeof(TCHAR));  m_sql_text = v;
		mc_max_rows.GetWindowText(v, sizeof(v)/sizeof(TCHAR));  
		m_max_rows = _tstoi(v);
	}

	void SetData()
	{
		TCHAR v[32768];
		mc_sql_text.SetWindowText(m_sql_text);
		if (!_itot_s(m_max_rows, v, sizeof(v)/sizeof(TCHAR), 10))
			mc_max_rows.SetWindowText(v);
	}
};
