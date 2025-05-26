#pragma once
#include "afxwin.h"
#define WM_MY_CLIPBOARD (WM_USER + 101) 
#include "MFCApplication1.h"
class ClipboardHistoryManager : public CWnd
{

public:

	ClipboardHistoryManager();

	virtual ~ClipboardHistoryManager();

	BOOL StartListening();

	void StopListening();

	BOOL SaveToExternalFile(const CString& strData, LPCTSTR lpszFilePath = _T("clipboard_history.dat"));

	CString NormalizeNewlines(const CString& strInput);





protected:

	afx_msg LRESULT OnClipboardUpdate(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

private:

	CString GetClipboardText();


};

