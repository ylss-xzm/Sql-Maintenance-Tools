#include "pch.h"
#include "ClipboardHistoryManager.h"
#include"winuser.h"



BEGIN_MESSAGE_MAP(ClipboardHistoryManager, CWnd)

	ON_MESSAGE(WM_CLIPBOARDUPDATE, OnClipboardUpdate)

END_MESSAGE_MAP()

ClipboardHistoryManager::ClipboardHistoryManager()
{
	CreateEx(0, AfxRegisterWndClass(0), _T("ClipboardListener"),
		0, 0, 0, 0, 0, HWND_MESSAGE, NULL);//������һ�����ɼ��ļ��а��ش���
}


ClipboardHistoryManager::~ClipboardHistoryManager()
{
	StopListening();
	DestroyWindow();//���ټ�ش���
	
	
}

BOOL ClipboardHistoryManager::StartListening()
{
	return AddClipboardFormatListener(GetSafeHwnd());//ʹ��Windows API�������а�
}

void ClipboardHistoryManager::StopListening()
{
	RemoveClipboardFormatListener(GetSafeHwnd());//�Ƴ���������ֹ������ռ�ü��а�
}

LRESULT ClipboardHistoryManager::OnClipboardUpdate(WPARAM wParam, LPARAM lParam)
{
	CString text = GetClipboardText();//��ȡ���а�����


	CMFCApplication1App* pApp = (CMFCApplication1App*)AfxGetApp();

	CString strPath = pApp->m_strClipboardHistoryPath;

	if (!text.IsEmpty())
	{
		if (SaveToExternalFile(text, strPath))
		{
			TRACE(_T("���а����ݱ���ɹ�\n"));
		}
		else
		{
			TRACE(_T("���а����ݱ���ʧ��\n"));
		}
		//AfxGetMainWnd()->SendMessage(WM_MY_CLIPBOARD,
		//	(WPARAM)new CString(text),
		//	0);//���ｫ��ȡ�������ݴ��͸�������
	}
	return 0;

}

//������������а��ı�����
CString ClipboardHistoryManager::GetClipboardText()
{
	CString result = _T("");

	if (!::OpenClipboard(GetSafeHwnd()))//���Դ򿪼��а壬ʧ����ֱ�ӷ��ؿ�����
	{
		return result;
	}

	HANDLE hData = GetClipboardData(CF_UNICODETEXT);//������а��Unicode�ı�

	if (hData != NULL)
	{
		LPCWSTR pszText = (LPCWSTR)GlobalLock(hData);//��ָ�������ڴ�
		if (pszText != NULL)
		{
			result = pszText;

			GlobalUnlock(hData); //����ָ�����ڴ�
		}
	}
	else if ((hData = GetClipboardData(CF_TEXT)) != NULL)//CF_TEXTΪANSI�ı�
	{
		LPCWSTR pszText = (LPCWSTR)GlobalLock(hData);//��ָ�������ڴ�
		if (pszText != NULL)
		{
			result = CString(pszText);//��Ҫת�룬�����������

			GlobalUnlock(hData); //����ָ�����ڴ�
		}
	}

	CloseClipboard();//��Ҫ�رռ��а�

	return result;

}

BOOL ClipboardHistoryManager::SaveToExternalFile(const CString& strData, LPCTSTR lpszFilePath)
{
	// �����ļ���С(�������1MB)
	const DWORD dwMaxFileSize = 1024 * 1024 * 3;

	CFile file;
	CFileStatus status;

	// ��������ļ���С
	if (CFile::GetStatus(lpszFilePath, status) && status.m_size > dwMaxFileSize)
	{
		// �ļ����󣬴������ݲ����¿�ʼ
		CString strBackup = lpszFilePath;
		strBackup += _T(".bak");

		// ɾ���ɱ���(�������)
		if (CFile::GetStatus(strBackup, status))
		{
			if (!DeleteFile(strBackup))
			{
				DWORD dwError = GetLastError();
				CString strError;
				strError.Format(_T("�޷�ɾ���ɱ����ļ����������: %d"), dwError);
				AfxMessageBox(strError);
				return FALSE;
			}
		}
		// ��������ǰ�ļ�Ϊ����
		if (!MoveFile(lpszFilePath, strBackup))
		{
			DWORD dwError = GetLastError();
			CString strError;
			strError.Format(_T("�޷��������ļ�Ϊ���ݣ��������: %d"), dwError);
			AfxMessageBox(strError);
			return FALSE;
		}
	}


	try
	{
		CStdioFile file;

		char* old_locale = _strdup(setlocale(LC_CTYPE, NULL));
		setlocale(LC_CTYPE, "chs");

		if (!file.Open(lpszFilePath,
			CFile::modeWrite | CFile::modeCreate | CFile::modeNoTruncate | CFile::typeText))
		{
			return false;
		}

		file.SeekToEnd();

		static CString s_strLastClipboardContent;
		if (strData == s_strLastClipboardContent)
		{
			return false; // ������ͬ����
		}
		else
		{
			s_strLastClipboardContent = strData;

			CString strRecord;
			strRecord.Format(_T("%llu | %s\r\n"),
				CTime::GetCurrentTime().GetTime(),
				s_strLastClipboardContent);//��ȡ��д��ʱ���

			NormalizeNewlines(strRecord);

			strRecord.TrimRight();

			strRecord = strRecord + _T("###\r\n");

			file.WriteString(strRecord);

			file.Close();

			setlocale(LC_CTYPE, old_locale);
			free(old_locale);

			return true;
		}


	}
	catch (CFileException*e)
	{
		e->Delete();
		return false;
	}
}

CString ClipboardHistoryManager::NormalizeNewlines(const CString& strInput)
{
	CString strResult;
	int newlineCount = 0;
	const int maxNewlines = 2; // ��ౣ��2������(һ������)

	for (int i = 0; i < strInput.GetLength(); i++)
	{
		TCHAR ch = strInput[i];

		if (ch == _T('\r') || ch == _T('\n'))
		{
			newlineCount++;
			if (newlineCount <= maxNewlines)
			{
				// ͳһת��Ϊ\r\n
				if (ch == _T('\n') && (i == 0 || strInput[i - 1] != _T('\r')))
				{
					strResult += _T("\r\n");
				}
			}
		}
		else
		{
			if (newlineCount > 0)
			{
				// ��ӱ����Ļ��з�
				if (newlineCount > maxNewlines)
				{
					strResult += CString(_T("\r\n"), maxNewlines);
				}
				else
				{
					strResult += CString(_T("\r\n"), newlineCount);
				}
				newlineCount = 0;
			}
			strResult += ch;
		}
	}

	return strResult;
}
