#pragma once
#include "ClipboardHistoryManager.h"
#define WM_MY_LOADCLIPBOARDDLG (WM_USER + 101) 

// LoadClipboarddlg 对话框

class LoadClipboarddlg : public CDialogEx
{
	DECLARE_DYNAMIC(LoadClipboarddlg)

public:
	LoadClipboarddlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~LoadClipboarddlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();

	void ListBoxFormat();



	void  OnLoadClipboard(LPCTSTR lpszFilePath);

	void  LoadClipboardHistory();

public:
	CListBox m_LoadClipboard_list;
	afx_msg void OnLbnDblclkList1();
	CArray<CString> m_arrRawData; // 存储原始数据

	CFont m_listFont;

	ClipboardHistoryManager m_ClipboardHistoryManager;

	virtual void PostNcDestroy() override;


	afx_msg void OnBnClickedButton2();
};
