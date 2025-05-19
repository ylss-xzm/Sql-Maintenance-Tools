
// toolsDlg.h: 头文件
//

#pragma once
#include"INPTBOX.h"
#include "sqlscripts.h"
#include"repalcetxt.h"
#include <shellapi.h> 
#include <windows.h>


// CtoolsDlg 对话框
class CMFCApplication1Dlg : public CDialogEx
{
	// 构造
public:
	CMFCApplication1Dlg(CWnd* pParent = nullptr);	// 标准构造函数

	~CMFCApplication1Dlg() {
		if (!m_strTempHtmlFile.IsEmpty() && PathFileExists(m_strTempHtmlFile))
		{
			DeleteFile(m_strTempHtmlFile);
		}
	}

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TOOLS_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	CFont m_listFont;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()



public:

	CMenu menu;
	afx_msg void On32774();
	afx_msg void On32771();
	void OnIntlist();

	afx_msg void OnLbnDblclkList1();
	CListBox m_list;
	CEdit m_Edit;
	INPTBOX INPT;
	sqlscripts splspt;
	CString DataProcessLogic(int i, int nLineCount, CString strLine, CString str);
	CString DataProcessLogica(int i, int nLineCount, CString strLine, CString str, CString CLNAME1);
	afx_msg void On32775();
	afx_msg void On32772();
	CFont m_font;

private:
	repalcetxt *pTD = NULL;
	CString ptxt1;
	CString ptxt2;
	CString Ptxt;
	CString CMFCApplication1Dlg::CreateTempHtmlFromResource(UINT nIDResource);
	CString m_strTempHtmlFile;

protected:
	afx_msg LRESULT OnMtMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void On32776();
	afx_msg void OnEnChangeEdit3();
public:

	void ListBoxFormat();
};




