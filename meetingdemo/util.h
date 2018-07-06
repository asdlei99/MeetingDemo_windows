/*##############################################################################
 * �ļ���util.h
 * ���������ߺ����������Ͷ���
 * ���ߣ�Teck
 * ʱ�䣺2018��5��24��
 * ��Ȩ��Copyright(C) 2018 Fsmeeting.com. All rights reserved.
 ##############################################################################*/
#pragma once

#include "StdAfx.h"
#include "MessageBox.h"

namespace demo
{
/*------------------------------------------------------------------------------
 * ��  ������Unicode��UTF-8���ַ���ת��
 * ��  ����[in] wszIn	Unicode�ַ���
 *         [out] szOut	UTF-8�ַ���
 *         [in] nSize	��������С
 * ����ֵ��ת�����ֽ���
------------------------------------------------------------------------------*/
inline DWORD ConvertUnicodeToUtf8(const WCHAR * wszIn, CHAR *szOut, DWORD nSize)
{
	DWORD nLen = 0;

	nLen = WideCharToMultiByte(CP_UTF8, 0, wszIn, -1, NULL, 0, NULL, NULL);
	if (nSize <= nLen)
		return 0;
	nLen = WideCharToMultiByte(CP_UTF8, 0, wszIn, -1, szOut, nSize, NULL, NULL);
	szOut[nLen] = 0;

	return nLen;
}

/*------------------------------------------------------------------------------
 * ��  ������UTF-8��Unicode���ַ���ת��
 * ��  ����[in] wszIn	UTF-8�ַ���
 *         [out] szOut	Unicode�ַ���
 *         [in] nSize	��������С
 * ����ֵ��ת�����ֽ���
------------------------------------------------------------------------------*/
inline DWORD ConvertUtf8ToUnicode(const CHAR * szIn, WCHAR *wszOut, DWORD nSize)
{
	DWORD nLen = 0;
	nLen = MultiByteToWideChar(CP_UTF8, 0, szIn, -1, NULL, 0);
	if (nSize <= nLen)
		return 0;
	nLen = MultiByteToWideChar(CP_UTF8, 0, szIn, -1, wszOut, nSize);
	wszOut[nLen] = 0;
	return nLen;
}

/*------------------------------------------------------------------------------
 * ��  ������ʾ��ʾ�Ի���
 * ��  ����[in] hParent	������
 *         [in] szText	��ʾ���ı�
 * ����ֵ����
------------------------------------------------------------------------------*/
inline void ShowMessageBox(HWND hParent, LPCTSTR szText)
{
	CMessageBox* pMB = new CMessageBox;

	pMB->Create(hParent, _T("CDuiLoginWnd"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	pMB->SetText(szText);
		
	pMB->CenterWindow();
	pMB->ShowModal();
}
}