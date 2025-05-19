#include "pch.h"
#include "sqlscripts.h"

bool sqlscripts::readsqldata(UINT uID, int i)
{

	// 1. 查找资源
	HRSRC hRes = FindResource(AfxGetResourceHandle(),
		MAKEINTRESOURCE(uID),
		_T("text"));
	if (!hRes) return false;

	// 2. 加载资源
	DWORD dwSize = SizeofResource(AfxGetResourceHandle(), hRes);
	HGLOBAL hData = LoadResource(AfxGetResourceHandle(), hRes);
	if (!hData) return false;

	LPVOID pData = LockResource(hData);
	//DWORD dwSize = SizeofResource(NULL, hRes);

	CStringA strA((LPCSTR)pData, dwSize);


	CStringW strW = CA2W(strA, CP_UTF8);

	CString str = CString(strW);

	//// 3. 创建内存文件
	//CMemFile file((BYTE*)LockResource(hData), dwSize);
	//CArchive ar(&file, CArchive::load);

	//// 4. 读取内容
	//CString strText;
	//CString strLine;
	//while (ar.ReadString(strLine))
	//{
	//	
	//	strText = strText+ CString(strLine) + _T("\r\n");
	//}
	sqlmap.SetAt(i, str);

}
//此处用于处理中文乱码问题
void sqlscripts::ReadStringCharToUnicode(CString & str)
{
	char *szBuf = new char[str.GetLength() + 1];

	memset(szBuf, '\0', str.GetLength());

	int i;
	for (i = 0; i < str.GetLength(); i++)
	{
		szBuf[i] = (char)str.GetAt(i);
	}
	szBuf[i] = '\0';


	int nLen;
	WCHAR *ptch;

	if (szBuf == NULL)
	{
		return;
	}
	nLen = MultiByteToWideChar(CP_UTF8, 0, szBuf, -1, NULL, 0);
	ptch = new WCHAR[nLen];
	memset(ptch, '\0', nLen);
	//TRACE("size:%d\n", nLen);
	MultiByteToWideChar(CP_UTF8, 0, szBuf, -1, ptch, nLen);
	str.Format(_T("%s"), ptch);
	//TRACE("size:%s\n", ptch);

	if (NULL != ptch)
		delete[] ptch;
	ptch = NULL;

	if (NULL != szBuf)
		delete[] szBuf;
	szBuf = NULL;

	return;
}

