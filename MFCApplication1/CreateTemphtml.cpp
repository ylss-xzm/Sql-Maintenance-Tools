#include "pch.h"
#include "CreateTemphtml.h"



CString CreateTemphtml::CreateTempHtmlFromResource(UINT nIDResource)
{
	// 1. ����HTML��Դ
	HRSRC hResource = FindResource(AfxGetResourceHandle(),
		MAKEINTRESOURCE(nIDResource),
		RT_HTML);
	if (!hResource)
	{
		DWORD dwError = GetLastError();
		CString strError;
		strError.Format(_T("����HTML��Դʧ��!\n�������: %d\n"), dwError);

		// ��������������
		if (dwError == 1813) // RESOURCE TYPE NOT FOUND
			strError += _T("��Դ����δ�ҵ�����ȷ����Դ�����Ƿ�Ϊ'HTML'");
		else if (dwError == 1814) // RESOURCE NAME NOT FOUND
			strError += _T("��ԴIDδ�ҵ�����ȷ��IDR_HTML_HELP�Ƿ���ȷ");

		AfxMessageBox(strError);

		return _T("");
	}

	// 2. ������Դ
	HGLOBAL hMemory = LoadResource(AfxGetResourceHandle(), hResource);
	if (!hMemory)
	{
		AfxMessageBox(_T("������Դʧ��"));
		return _T("");
	}

	// 3. ��ȡ��Դ���ݺʹ�С
	DWORD dwSize = SizeofResource(AfxGetResourceHandle(), hResource);
	LPVOID pData = LockResource(hMemory);

	// 4. ������ʱ�ļ�
	TCHAR szTempPath[MAX_PATH];
	TCHAR szTempFile[MAX_PATH];
	GetTempPath(MAX_PATH, szTempPath);
	GetTempFileName(szTempPath, _T("MFC"), 0, szTempFile);

	// ȷ����չ��Ϊ.html
	CString strTempFile = szTempFile;
	strTempFile = strTempFile.Left(strTempFile.ReverseFind('.')) + _T(".html");

	// 5. д��HTML���ݵ���ʱ�ļ�
	CFile file;
	if (file.Open(strTempFile, CFile::modeCreate | CFile::modeWrite))
	{
		file.Write(pData, dwSize);
		file.Close();
		return strTempFile;
	}

	return _T("");
}


// ʹ������URL��HTML��Դ��������
void CreateTemphtml::OpenHtmlResourceViaDataUrl(LPCTSTR lpszFilePath)
{
	if (PathFileExists(lpszFilePath))
	{
		HINSTANCE hResult = ShellExecute(NULL, _T("open"), lpszFilePath, NULL, NULL, SW_SHOW);
		if ((INT_PTR)hResult <= 32)
		{
			CString strError;
			strError.Format(_T("�޷���HTML�ļ����������: %d"), (INT_PTR)hResult);
			AfxMessageBox(strError);
		}
	}
	else
	{
		AfxMessageBox(_T("HTML�ļ�������"));
	}

}