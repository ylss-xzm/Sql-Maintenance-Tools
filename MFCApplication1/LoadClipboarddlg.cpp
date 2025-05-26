// LoadClipboarddlg.cpp: 实现文件
//

#include "pch.h"
#include "MFCApplication1.h"
#include "LoadClipboarddlg.h"
#include "afxdialogex.h"
#include <shlobj.h> // 用于SHGetFolderPath
#include <shlwapi.h> // 用于PathAppend
#include <algorithm>


// LoadClipboarddlg 对话框

IMPLEMENT_DYNAMIC(LoadClipboarddlg, CDialogEx)

LoadClipboarddlg::LoadClipboarddlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG3, pParent)
{

}

LoadClipboarddlg::~LoadClipboarddlg()
{
	
}

void LoadClipboarddlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_LoadClipboard_list);
}


BEGIN_MESSAGE_MAP(LoadClipboarddlg, CDialogEx)
	ON_LBN_DBLCLK(IDC_LIST1, &LoadClipboarddlg::OnLbnDblclkList1)
	
	ON_BN_CLICKED(IDC_BUTTON2, &LoadClipboarddlg::OnBnClickedButton2)
END_MESSAGE_MAP()


BOOL LoadClipboarddlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	LoadClipboardHistory(); // 加载数据

	ListBoxFormat();

	return 0;
}

// LoadClipboarddlg 消息处理程序

// 在任意需要的地方调用
void  LoadClipboarddlg::LoadClipboardHistory()
{
	m_LoadClipboard_list.ResetContent();

	CMFCApplication1App* pApp = (CMFCApplication1App*)AfxGetApp();

	CString strPath = pApp->m_strClipboardHistoryPath;

	CFileStatus status;

	CString strPathBak = strPath + _T(".bak");

	if (CFile::GetStatus(strPathBak, status) != NULL)
	{
		OnLoadClipboard(strPath);

		

		OnLoadClipboard(strPathBak);

	}
	else
	{
		OnLoadClipboard(strPath);

		
	}


}

void  LoadClipboarddlg::OnLoadClipboard(LPCTSTR lpszFilePath)
{
	struct ClipItem {
		time_t time;
		CString content;
	};

	CArray<ClipItem> arrItems;

	try {
		CStdioFile file;
		if (!file.Open(lpszFilePath, CFile::modeRead | CFile::typeText)) {
			AfxMessageBox(_T("无法打开历史文件"));
			return;
		}

		CString strLine;
		CString strText;
		int i = 0;
		ClipItem item;


		while (file.ReadString(strLine)) {

			if (!strLine.IsEmpty()) {
				int nSepPos = strLine.Find(_T('|'));
				int nSepPos1 = strLine.Find(_T('###'));

				if (nSepPos != -1 && i == 0 && nSepPos1 == -1) {
					item.time = _ttoi64(strLine.Left(nSepPos));
					strText = strLine.Mid(nSepPos + 1);
					i = 1;
					//AfxMessageBox(_T("1"));
				}
				else if (i == 1 && nSepPos == -1 && nSepPos1 == -1)
				{
					strText = strText + strLine;
					//AfxMessageBox(_T("2"));
				}
				else if (nSepPos1 != -1 && i == 1 && nSepPos == -1)
				{
					strText = strText + strLine.Left(nSepPos1);
					i = 0;
					item.content = strText;
					arrItems.Add(item);
					//AfxMessageBox(_T("3"));

				}
				else if (nSepPos != -1 && nSepPos1 != -1 && i == 0)
				{
					nSepPos = nSepPos + 1;
					item.time = _ttoi64(strLine.Left(nSepPos));
					strText = strLine.Mid(nSepPos, nSepPos1 - nSepPos);
					item.content = strText;
					arrItems.Add(item);
				}

			}
		}

		// 安全排序
		if (arrItems.GetSize() > 0) {
			std::sort(arrItems.GetData(),
				arrItems.GetData() + arrItems.GetSize(),
				[](const ClipItem& a, const ClipItem& b) {
				return a.time > b.time;
			});
		}

		//CString str1;
		//str1.Format(_T("%d"), arrItems.GetSize());
		//AfxMessageBox(str1);

		// 安全添加项
		for (int i = 0; i < arrItems.GetSize() && i < 100; i++) {
			CString strDisplay;
			CString strText;
			strDisplay.Format(_T("[%s] %s"),
				CTime(arrItems[i].time).Format("%Y-%m-%d %H:%M:%S"),
				arrItems[i].content);
			m_arrRawData.Add(arrItems[i].content.Trim());
			//strText = arrItems[i].content;
			//AfxMessageBox(strText);
			CString PreviewText;
			PreviewText = strDisplay.Left(100);
			PreviewText.Replace(_T("\r\n"), _T("\n")); // 统一为\n
			PreviewText.Replace(_T("\r"), _T("\n"));   // 确保无\r
			PreviewText.Replace(_T("\n"), _T(" "));
			m_LoadClipboard_list.InsertString(-1, PreviewText);

		}

	}
	catch (CFileException* e) {
		e->Delete();
		AfxMessageBox(_T("文件操作异常"));
	}
	catch (...) {
		AfxMessageBox(_T("未知异常"));
	}

}

void LoadClipboarddlg::OnLbnDblclkList1()
{
	// TODO: 在此添加控件通知处理程序代码

	int nIndex = m_LoadClipboard_list.GetCurSel();
	if (nIndex >= 0 && nIndex < m_arrRawData.GetSize())
	{
		CString strFullText = m_arrRawData[nIndex];
		//AfxMessageBox(strFullText); // 显示完整内容

		if (OpenClipboard()) {
			EmptyClipboard();

			HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE,
				(strFullText.GetLength() + 1) * sizeof(TCHAR));
			if (hMem) {
				LPTSTR pMem = (LPTSTR)GlobalLock(hMem);
				_tcscpy_s(pMem, strFullText.GetLength() + 1, strFullText);
				GlobalUnlock(hMem);

				SetClipboardData(CF_UNICODETEXT, hMem);

				// 4. 提示用户（可选）

				//AfxMessageBox(_T("剪切板已复制"));
			}
			CloseClipboard();
		}
	}
	DestroyWindow();
}


void LoadClipboarddlg::ListBoxFormat()
{
	// TODO: 在此处添加实现代码.

	m_listFont.CreateFont(
		21,                        // 字体高度
		0,                         // 宽度
		0,                         // 倾斜角度
		0,                         // 方向角度
		FW_NORMAL,                 // 正常粗细
		FALSE,                     // 非斜体
		true,                     // 无下划线
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

//关闭窗口回收指针和内存
void LoadClipboarddlg::PostNcDestroy()
{
	
	delete this; // 关键：自销毁

	CDialogEx::PostNcDestroy();
}

void LoadClipboarddlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码

	CMFCApplication1App* pApp = (CMFCApplication1App*)AfxGetApp();

	CString strPath = pApp->m_strClipboardHistoryPath;

	CString strMsg;

	strMsg.Format(_T("剪切板数据文件在如下位置: \n %s \n 如需清除剪切板数据，直接删除就行，请不要对其做任何修改和保存，否则会出现格式混乱，如已修改，请删除它即可 \n另:如果出现粘贴和复制无法使用的情况，请关闭本软件重试"), strPath);

	MessageBox(strMsg);
}
