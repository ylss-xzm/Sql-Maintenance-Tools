#include "pch.h"
#include "CreateTemphtml.h"



CString CreateTemphtml::CreateTempHtmlFromResource(UINT nIDResource)
{
	// 1. 查找HTML资源
	HRSRC hResource = FindResource(AfxGetResourceHandle(),
		MAKEINTRESOURCE(nIDResource),
		RT_HTML);
	if (!hResource)
	{
		DWORD dwError = GetLastError();
		CString strError;
		strError.Format(_T("查找HTML资源失败!\n错误代码: %d\n"), dwError);

		// 常见错误代码解释
		if (dwError == 1813) // RESOURCE TYPE NOT FOUND
			strError += _T("资源类型未找到，请确认资源类型是否为'HTML'");
		else if (dwError == 1814) // RESOURCE NAME NOT FOUND
			strError += _T("资源ID未找到，请确认IDR_HTML_HELP是否正确");

		AfxMessageBox(strError);

		return _T("");
	}

	// 2. 加载资源
	HGLOBAL hMemory = LoadResource(AfxGetResourceHandle(), hResource);
	if (!hMemory)
	{
		AfxMessageBox(_T("加载资源失败"));
		return _T("");
	}

	// 3. 获取资源数据和大小
	DWORD dwSize = SizeofResource(AfxGetResourceHandle(), hResource);
	LPVOID pData = LockResource(hMemory);

	// 4. 创建临时文件
	TCHAR szTempPath[MAX_PATH];
	TCHAR szTempFile[MAX_PATH];
	GetTempPath(MAX_PATH, szTempPath);
	GetTempFileName(szTempPath, _T("MFC"), 0, szTempFile);

	// 确保扩展名为.html
	CString strTempFile = szTempFile;
	strTempFile = strTempFile.Left(strTempFile.ReverseFind('.')) + _T(".html");

	// 5. 写入HTML内容到临时文件
	CFile file;
	if (file.Open(strTempFile, CFile::modeCreate | CFile::modeWrite))
	{
		file.Write(pData, dwSize);
		file.Close();
		return strTempFile;
	}

	return _T("");
}


// 使用数据URL打开HTML资源的主函数
void CreateTemphtml::OpenHtmlResourceViaDataUrl(LPCTSTR lpszFilePath)
{
	if (PathFileExists(lpszFilePath))
	{
		HINSTANCE hResult = ShellExecute(NULL, _T("open"), lpszFilePath, NULL, NULL, SW_SHOW);
		if ((INT_PTR)hResult <= 32)
		{
			CString strError;
			strError.Format(_T("无法打开HTML文件，错误代码: %d"), (INT_PTR)hResult);
			AfxMessageBox(strError);
		}
	}
	else
	{
		AfxMessageBox(_T("HTML文件不存在"));
	}

}