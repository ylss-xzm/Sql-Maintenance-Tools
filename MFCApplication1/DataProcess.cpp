#include "pch.h"
#include "DataProcess.h"



CString DataProcess::DataProcessLogic(int i, int nLineCount, CString strLine, CString str)
{
	// TODO: 在此处添加实现代码.
	CString strdate = str;
	CString strLine1 = strLine;


	if (i == 0) {

		if (strLine1.IsEmpty() == false)
			strdate = _T("'") + strLine1 + _T("'");

	}
	else if (i == nLineCount - 1)
	{
		if (strLine1.IsEmpty() == false)
			strdate = strdate + _T(",'") + strLine1 + _T("'");
	}
	else
	{
		if (strLine1.IsEmpty() == false)
			strdate = strdate + _T(",'") + strLine1 + _T("'");
	}

	return strdate;
}



CString DataProcess::DataProcessLogic(int i, int nLineCount, CString strLine, CString str, CString CLNAME1)
{
	// TODO: 在此处添加实现代码.
	CString strdate = str;
	CString strLine1 = strLine;
	CString CLNAME2 = CLNAME1;

	if (i == 0) {

		if (strLine1.IsEmpty() == false)
			strdate = CLNAME2 + _T(" IN (") + _T("'") + strLine1 + _T("'");

	}
	else if (i == nLineCount - 1)
	{
		if (strLine1.IsEmpty() == false)
		{
			strdate = strdate + _T(",'") + strLine1 + _T("' )");
		}
		else
		{
			strdate = strdate + _T(")");
		}

	}
	else if (i % 980 == 0)
	{
		if (strLine1.IsEmpty() == false)
			strdate = strdate + _T(") OR ") + CLNAME2 + _T(" IN (") + _T("'") + strLine1 + _T("'");
	}
	else
	{

		if (strLine1.IsEmpty() == false)
			strdate = strdate + _T(",'") + strLine1 + _T("'");

	}

	return strdate;
}