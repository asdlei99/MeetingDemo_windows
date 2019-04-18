/*##############################################################################
 * �ļ���FloatWnd.cpp
 * ��������������ʵ�֣�������һ�㴰��ʵ�֣������������
 * ���ߣ�Teck
 * ʱ�䣺2018��5��24��
 * ��Ȩ��Copyright(C) 2018 Fsmeeting.com. All rights reserved.
 ##############################################################################*/
#include "stdafx.h"
#include "FloatWnd.h"
#include "UIMenu.h"

/*------------------------------------------------------------------------------
 * ��  �������캯��
 * ��  ������
 * ����ֵ����
------------------------------------------------------------------------------*/
CFloatWnd::CFloatWnd(LPCTSTR szLayoutXml, LPCTSTR szWndClassName)
	: m_szLayoutXml(szLayoutXml)
	, m_szWndClassName(szWndClassName)
	, m_dwThreadId(0)
{
}

/*------------------------------------------------------------------------------
 * ��  ������������
 * ��  ������
 * ����ֵ����
------------------------------------------------------------------------------*/
CFloatWnd::~CFloatWnd()
{
	if (m_dwThreadId != 0)
		::PostThreadMessage(m_dwThreadId, WM_QUIT, 0, 0);
}

/*------------------------------------------------------------------------------
 * ��  �������⺯����DUI���û�ȡ����xml�����ļ�Ŀ¼
 * ��  ������
 * ����ֵ��xml�����ļ�Ŀ¼
 ------------------------------------------------------------------------------*/
CDuiString CFloatWnd::GetSkinFolder()
{
	return CDuiString(L"skin");
}

/*------------------------------------------------------------------------------
 * ��  �������⺯����DUI���û�ȡ����xml�����ļ�����
 * ��  ������
 * ����ֵ��xml�����ļ�����
 ------------------------------------------------------------------------------*/
CDuiString CFloatWnd::GetSkinFile()
{
	return m_szLayoutXml;
}

/*------------------------------------------------------------------------------
 * ��  �������⺯����DUI���û�ȡ����������
 * ��  ������
 * ����ֵ������������
 ------------------------------------------------------------------------------*/
LPCTSTR CFloatWnd::GetWindowClassName(void) const
{
	return m_szWndClassName;
}

/*------------------------------------------------------------------------------
 * ��  ��������ӵ���Լ����߳�
 * ��  ����[in] lpParam �Զ������
 * ����ֵ��DWORD
 ------------------------------------------------------------------------------*/
DWORD CFloatWnd::WndThreadProc(LPVOID lpParam)
{
	CFloatWnd* pThis = (CFloatWnd*)lpParam;

	if (pThis->m_bShowAfterCreate)
		::ShowWindow(pThis->m_hWnd, SW_SHOW);
	else
		::ShowWindow(pThis->m_hWnd, SW_HIDE);

	pThis->m_PaintManager.MessageLoop();

	return 0;
}

/*------------------------------------------------------------------------------
 * ��  ������������
 * ��  ����[in] hParentWnd	������
 *         [in] szWndName	��������
 *         [in] dwWndStyle	���ڷ��
 *         [in] bShowAfterCreate �������Ƿ�������ʾ
 * ����ֵ���ɹ�/ʧ��
 ------------------------------------------------------------------------------*/
bool CFloatWnd::CreateWnd(HWND hParentWnd, LPCTSTR szWndName, DWORD dwWndStyle, bool bShowAfterCreate)
{
	m_hParentWnd = hParentWnd;
	m_bShowAfterCreate = bShowAfterCreate;

	WindowImplBase::Create(hParentWnd, szWndName, dwWndStyle, WS_EX_WINDOWEDGE);

	if (NULL == ::CreateThread(NULL, 0, WndThreadProc, this, 0, &m_dwThreadId))
		return false;

	return true;
}

/*------------------------------------------------------------------------------
 * ��  �������ô��ھ�������
 * ��  ����[in] rectWnd ���ھ�������
 * ����ֵ����
 ------------------------------------------------------------------------------*/
void CFloatWnd::SetWndRect(const RECT& rectWnd)
{
	::MoveWindow(m_hWnd, 
		rectWnd.left,
		rectWnd.top,
		rectWnd.right - rectWnd.left,
		rectWnd.bottom - rectWnd.top,
		TRUE);

	::ShowWindow(m_hWnd, SW_SHOW);
}

