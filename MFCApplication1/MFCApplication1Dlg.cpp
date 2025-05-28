
// toolsDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MFCApplication1Dlg.h"
#include "MFCApplication1.h"
#include "afxdialogex.h"




#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)

END_MESSAGE_MAP()



// CMFCApplication1Dlg 对话框



CMFCApplication1Dlg::CMFCApplication1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION1_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Control(pDX, IDC_RICHEDIT21, m_Edit);
}

BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_GETMINMAXINFO()
	ON_COMMAND(ID_32774, &CMFCApplication1Dlg::On32774)
	ON_COMMAND(ID_32771, &CMFCApplication1Dlg::On32771)

	ON_LBN_DBLCLK(IDC_LIST1, &CMFCApplication1Dlg::OnLbnDblclkList1)
	ON_COMMAND(ID_32775, &CMFCApplication1Dlg::On32775)
	ON_COMMAND(ID_32772, &CMFCApplication1Dlg::On32772)
	ON_MESSAGE(WM_MY_DIALOG_CLOSED, &CMFCApplication1Dlg::OnMtMessage)
	ON_COMMAND(ID_32776, &CMFCApplication1Dlg::On32776)
	ON_WM_CTLCOLOR()

	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_MY_LOADCLIPBOARDDLG, OnClipboardTextChanged)

	ON_COMMAND(ID_32779, &CMFCApplication1Dlg::On32779)
END_MESSAGE_MAP()



// CMFCApplication1Dlg 消息处理程序

BOOL CMFCApplication1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	menu.LoadMenu(IDR_MENU1);

	SetMenu(&menu);

	OnIntlist();

	m_Edit.LimitText(-1);//解除文本编辑框中，文本数据限制的问题

	ListBoxFormat();

	strTempFile = CreateTemphtml.CreateTempHtmlFromResource(IDR_HTML1);//初始化的时候来生成临时的html文件方便后续调用


	 if (!m_ClipboardHistoryManager.StartListening())
	 {
		 AfxMessageBox(_T("无法启动剪切板监听器！"));
	 }

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCApplication1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCApplication1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCApplication1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//菜单栏清除文本内容函数
void CMFCApplication1Dlg::On32774()
{
	// TODO: 在此添加命令处理程序代码
	GetDlgItem(IDC_RICHEDIT21)->SetWindowText(_T(""));

	//MessageBox(_T("XXXX"));
}

//值格式化函数
void CMFCApplication1Dlg::On32771()
{
	CString str;
	// TODO: 在此添加命令处理程序代码
	//GetDlgItem(IDC_EDIT3)->GetWindowText(str);
	int nLineCount = m_Edit.GetLineCount();
	int i = 0;
	CString size1;
	CString strText, strLine;
	CString CLNAME1;
	str = _T("");
	// Dump every line of text of the edit control.
	if (nLineCount > 980)
	{
		INPT.DoModal();
		CLNAME1 = INPT.Clname;

	}
	for (i = 0; i < nLineCount; i++)
	{
		// length of line i:
		int len = m_Edit.LineLength(m_Edit.LineIndex(i));
		m_Edit.GetLine(i, strText.GetBuffer(len), len);
		strText.ReleaseBuffer(len);
		strLine.Format(_T("%s"), strText);
		//MessageBox(strLine);//输出得到的每行数据
		strLine.Trim();
		if (nLineCount > 980)
		{
			str = DataProcess.DataProcessLogic(i, nLineCount, strLine, str, CLNAME1);

		}
		else
		{
			str = DataProcess.DataProcessLogic(i, nLineCount, strLine, str);
		}
		//MessageBox(str);
	}


	//str = "I AM  WMS_L1";
	GetDlgItem(IDC_RICHEDIT21)->SetWindowText(str);
	//MessageBox(_T("处理完成"));
}

//初始化列表框
void CMFCApplication1Dlg::OnIntlist()
{

	// TODO: 在此处添加实现代码.
	m_list.InsertString(0, _T("DB FORMAT"));


}



void CMFCApplication1Dlg::OnLbnDblclkList1()
{
	// TODO: 在此添加控件通知处理程序代码

	int m_row = 0;

	m_row = m_list.GetCurSel();

	CString str1;

	splspt.sqlmap.Lookup(m_row, str1);

	//AfxMessageBox(str1);

	GetDlgItem(IDC_RICHEDIT21)->SetWindowText(str1);


}





void CMFCApplication1Dlg::On32775()
{
	// TODO: 在此添加命令处理程序代码


	CString str1;

	splspt.sqlmap.Lookup(17, str1);

	//AfxMessageBox(str1);

	GetDlgItem(IDC_RICHEDIT21)->SetWindowText(str1);


}


void CMFCApplication1Dlg::On32772()
{


	// TODO: 在此添加命令处理程序代码
	if (pTD == NULL)
	{
		pTD = new repalcetxt();
		pTD->Create(IDD_DIALOG2, this);
		pTD->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_RICHEDIT21)->GetWindowText(Ptxt);
	}
	else
	{
		pTD->ShowWindow(SW_SHOW);
	}



}


afx_msg LRESULT CMFCApplication1Dlg::OnMtMessage(WPARAM wParam, LPARAM lParam)
{
	repalcetxt * pDialog = (repalcetxt*)wParam;
	CString reptxt1;
	CString reptxt2;
	reptxt1 = pDialog->txt1;
	reptxt2 = pDialog->txt2;

	Ptxt.Replace(reptxt1, reptxt2);

	GetDlgItem(IDC_RICHEDIT21)->SetWindowText(Ptxt);
	MessageBox(_T("字符替换成功"));
	pTD = NULL;
	delete pDialog;
	return 0;
}


void CMFCApplication1Dlg::On32776()
{
	// TODO: 在此添加命令处理程序代码

	

	// 2. 用默认浏览器打开
	if (!strTempFile.IsEmpty())
	{
		CreateTemphtml.OpenHtmlResourceViaDataUrl(strTempFile);

		// 可选：程序退出时删除临时文件
		m_strTempHtmlFile = strTempFile; // 保存到成员变量

	}
}


void CMFCApplication1Dlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// 设置最小宽度和高度（以像素为单位）
	lpMMI->ptMinTrackSize.x = 800; // 最小宽度
	lpMMI->ptMinTrackSize.y = 600; // 最小高度

	CDialogEx::OnGetMinMaxInfo(lpMMI);
}

HBRUSH CMFCApplication1Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	// 仅处理目标ListBox（IDC_LIST1）
	if (pWnd->GetDlgCtrlID() == IDC_LIST1)
	{
		pDC->SetBkColor(RGB(240, 240, 240));  // 设置背景色为浅灰色
		static CBrush brush(RGB(240, 240, 240)); // 静态画刷避免重复创建
		return (HBRUSH)brush.GetSafeHandle();
	}
	return CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CMFCApplication1Dlg::ListBoxFormat()
{
	// TODO: 在此处添加实现代码.



	splspt.readsqldata(IDR_TEXT1, 0);




	m_listFont.CreateFont(
		26,                        // 字体高度
		0,                         // 宽度
		0,                         // 倾斜角度
		0,                         // 方向角度
		FW_NORMAL,                 // 正常粗细
		FALSE,                     // 非斜体
		FALSE,                     // 无下划线
		0,                         // 无删除线
		DEFAULT_CHARSET,           // 字符集
		OUT_DEFAULT_PRECIS,        // 输出精度
		CLIP_DEFAULT_PRECIS,       // 剪裁精度
		DEFAULT_QUALITY,           // 输出质量
		DEFAULT_PITCH | FF_SWISS,  // 字体族
		_T("微软雅黑"));           // 字体名称

	// 2. 应用字体到ListBox（假设ID为IDC_LIST1）
	GetDlgItem(IDC_LIST1)->SetFont(&m_listFont);

}
CMFCApplication1Dlg ::~CMFCApplication1Dlg() {
	//TRACE(_T("123"));
		if (!m_strTempHtmlFile.IsEmpty() && PathFileExists(m_strTempHtmlFile))
		{
			DeleteFile(m_strTempHtmlFile);
			//TRACE(_T("321"));
		}

		delete pTD;
		
}

void CMFCApplication1Dlg::On32779()
{
	LoadClipboarddlg *m_dlg = nullptr;
	// TODO: 在此添加命令处理程序代码

	m_dlg = new LoadClipboarddlg();
	m_dlg->Create(IDD_DIALOG3, this);
	m_dlg->ShowWindow(SW_SHOW);

}
LRESULT CMFCApplication1Dlg::OnClipboardTextChanged(WPARAM wParam, LPARAM lParam)
{
	CString* pText = reinterpret_cast<CString*>(wParam); // 注意：这里接收的是堆分配的CString对象，必须手动删除

	if (pText != nullptr)
	{
		m_Edit.SetWindowTextW(*pText);

		delete pText;
	}

	return 0;
}