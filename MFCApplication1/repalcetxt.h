#pragma once
#define WM_MY_DIALOG_CLOSED (WM_USER + 100)  // WM_USER 是 Windows 预留的用户消息起始值

// repalcetxt 对话框

class repalcetxt : public CDialogEx
{
	DECLARE_DYNAMIC(repalcetxt)

public:
	repalcetxt(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~repalcetxt();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();

	CString txt1;
	CString txt2;

private:
	HWND m_hParentWnd;  // 保存句柄（比指针更安全）
public:
	BOOL Create(UINT nIDTemplate, CWnd* pParentWnd)
	{
		m_hParentWnd = pParentWnd->GetSafeHwnd();  // 存储句柄
		return CDialog::Create(nIDTemplate, pParentWnd);
	}
};
