// repalcetxt.cpp: 实现文件
//

#include "pch.h"
#include "MFCApplication1.h"
#include "repalcetxt.h"
#include "afxdialogex.h"


// repalcetxt 对话框

IMPLEMENT_DYNAMIC(repalcetxt, CDialogEx)

repalcetxt::repalcetxt(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
{

}

repalcetxt::~repalcetxt()
{
}

void repalcetxt::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(repalcetxt, CDialogEx)
	ON_BN_CLICKED(IDOK, &repalcetxt::OnBnClickedOk)
END_MESSAGE_MAP()


// repalcetxt 消息处理程序
void repalcetxt::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItemText(IDC_EDIT1, txt1);
	GetDlgItemText(IDC_EDIT2, txt2);
	if (txt1.IsEmpty() || txt2.IsEmpty())
	{
		MessageBox(_T("数据不能为空"));
	}
	else
	{
		if (::IsWindow(m_hParentWnd))
		{
			::PostMessage(m_hParentWnd, WM_MY_DIALOG_CLOSED, (WPARAM)this, 0);
			CDialogEx::OnOK();
		}
		else
		{
			return;
		}
	}

}
