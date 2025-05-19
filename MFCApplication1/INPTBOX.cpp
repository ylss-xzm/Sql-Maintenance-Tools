// INPTBOX.cpp: 实现文件
//

#include "pch.h"
#include "MFCApplication1.h"
#include "INPTBOX.h"
#include "afxdialogex.h"


// INPTBOX 对话框

IMPLEMENT_DYNAMIC(INPTBOX, CDialogEx)

INPTBOX::INPTBOX(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

INPTBOX::~INPTBOX()
{
}

void INPTBOX::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(INPTBOX, CDialogEx)
END_MESSAGE_MAP()


// INPTBOX 消息处理程序


void INPTBOX::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItemText(IDC_EDIT1, Clname);

	CDialogEx::OnOK();
}
