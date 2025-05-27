#include "pch.h"
#include "ClipboardHistoryManager.h"
#include"winuser.h"



BEGIN_MESSAGE_MAP(ClipboardHistoryManager, CWnd)

	ON_MESSAGE(WM_CLIPBOARDUPDATE, OnClipboardUpdate)

END_MESSAGE_MAP()

ClipboardHistoryManager::ClipboardHistoryManager()
{
	CreateEx(0, AfxRegisterWndClass(0), _T("ClipboardListener"),
		0, 0, 0, 0, 0, HWND_MESSAGE, NULL);//创建了一个不可见的剪切板监控窗口
}


ClipboardHistoryManager::~ClipboardHistoryManager()
{
	StopListening();
	DestroyWindow();//销毁监控窗口
	
	
}

BOOL ClipboardHistoryManager::StartListening()
{
	return AddClipboardFormatListener(GetSafeHwnd());//使用Windows API监听剪切板
}

void ClipboardHistoryManager::StopListening()
{
	RemoveClipboardFormatListener(GetSafeHwnd());//移除监听，防止程序长期占用剪切板
}

LRESULT ClipboardHistoryManager::OnClipboardUpdate(WPARAM wParam, LPARAM lParam)
{
	CString text = GetClipboardText();//获取剪切板数据


	CMFCApplication1App* pApp = (CMFCApplication1App*)AfxGetApp();

	CString strPath = pApp->m_strClipboardHistoryPath;

	if (!text.IsEmpty())
	{
		if (SaveToExternalFile(text, strPath))
		{
			TRACE(_T("剪切板内容保存成功\n"));
		}
		else
		{
			TRACE(_T("剪切板内容保存失败\n"));
		}
		//AfxGetMainWnd()->SendMessage(WM_MY_CLIPBOARD,
		//	(WPARAM)new CString(text),
		//	0);//这里将获取到的数据传送给主窗口
	}
	return 0;

}

//以下来处理剪切板文本数据
CString ClipboardHistoryManager::GetClipboardText()
{
	CString result = _T("");

	if (!::OpenClipboard(GetSafeHwnd()))//尝试打开剪切板，失败则直接返回空数据
	{
		return result;
	}

	HANDLE hData = GetClipboardData(CF_UNICODETEXT);//处理剪切板的Unicode文本

	if (hData != NULL)
	{
		LPCWSTR pszText = (LPCWSTR)GlobalLock(hData);//用指针锁定内存
		if (pszText != NULL)
		{
			result = pszText;

			GlobalUnlock(hData); //解锁指定的内存
		}
	}
	else if ((hData = GetClipboardData(CF_TEXT)) != NULL)//CF_TEXT为ANSI文本
	{
		LPCWSTR pszText = (LPCWSTR)GlobalLock(hData);//用指针锁定内存
		if (pszText != NULL)
		{
			result = CString(pszText);//需要转码，否侧会出现乱码

			GlobalUnlock(hData); //解锁指定的内存
		}
	}

	CloseClipboard();//需要关闭剪切板

	return result;

}

BOOL ClipboardHistoryManager::SaveToExternalFile(const CString& strData, LPCTSTR lpszFilePath)
{
	// 限制文件大小(例如最大1MB)
	const DWORD dwMaxFileSize = 1024 * 1024 * 3;

	CFile file;
	CFileStatus status;

	// 检查现有文件大小
	if (CFile::GetStatus(lpszFilePath, status) && status.m_size > dwMaxFileSize)
	{
		// 文件过大，创建备份并重新开始
		CString strBackup = lpszFilePath;
		strBackup += _T(".bak");

		// 删除旧备份(如果存在)
		if (CFile::GetStatus(strBackup, status))
		{
			if (!DeleteFile(strBackup))
			{
				DWORD dwError = GetLastError();
				CString strError;
				strError.Format(_T("无法删除旧备份文件，错误代码: %d"), dwError);
				AfxMessageBox(strError);
				return FALSE;
			}
		}
		// 重命名当前文件为备份
		if (!MoveFile(lpszFilePath, strBackup))
		{
			DWORD dwError = GetLastError();
			CString strError;
			strError.Format(_T("无法重命名文件为备份，错误代码: %d"), dwError);
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
			return false; // 跳过相同内容
		}
		else
		{
			s_strLastClipboardContent = strData;

			CString strRecord;
			strRecord.Format(_T("%llu | %s\r\n"),
				CTime::GetCurrentTime().GetTime(),
				s_strLastClipboardContent);//获取并写入时间戳

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
	const int maxNewlines = 2; // 最多保留2个换行(一个空行)

	for (int i = 0; i < strInput.GetLength(); i++)
	{
		TCHAR ch = strInput[i];

		if (ch == _T('\r') || ch == _T('\n'))
		{
			newlineCount++;
			if (newlineCount <= maxNewlines)
			{
				// 统一转换为\r\n
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
				// 添加保留的换行符
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
