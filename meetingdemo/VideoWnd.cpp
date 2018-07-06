/*##############################################################################
 * �ļ���VideoWnd.cpp
 * ��������Ƶ����ʵ��
 * ���ߣ�Teck
 * ʱ�䣺2018��5��24��
 * ��Ȩ��Copyright(C) 2018 Fsmeeting.com. All rights reserved.
 ##############################################################################*/
#include "stdafx.h"
#include "VideoWnd.h"
#include "util.h"


/*------------------------------------------------------------------------------
 * ��  �������캯��
 * ��  ������
 * ����ֵ����
 ------------------------------------------------------------------------------*/
CVideoWnd::CVideoWnd(LPCTSTR szLayoutXml, LPCTSTR szWndClassName)
	: CFloatWnd(szLayoutXml, szWndClassName)
	, m_pEventCallback(nullptr)
{
}

/*------------------------------------------------------------------------------
 * ��  ������������
 * ��  ������
 * ����ֵ����
 ------------------------------------------------------------------------------*/
CVideoWnd::~CVideoWnd()
{
}

/*------------------------------------------------------------------------------
 * ��  �������ô�����ʾ�ľ�������
 * ��  ����[in] rectWnd ��������
 * ����ֵ����
------------------------------------------------------------------------------*/
void CVideoWnd::SetWndRect(const RECT& rectWnd)
{
	DWORD dwWidth = rectWnd.right - rectWnd.left;
	DWORD dwHeight = rectWnd.bottom - rectWnd.top;

	RECT rectPadding;
	rectPadding.left = (dwWidth - 38) / 2;
	rectPadding.right = 0;
	rectPadding.bottom = 0;
	rectPadding.top = (dwHeight - 38) / 2;

	CPaintManagerUI& paintMgr = GetPaintManager();	
	((CHorizontalLayoutUI*)paintMgr.FindControl(L"video_bk"))->SetPadding(rectPadding);

	__super::SetWndRect(rectWnd);
}

/*------------------------------------------------------------------------------
 * ��  ����SDK֪ͨӦ�ò����û��㲥����Ƶ�Ĵ���
 * ��  ����[in] uMsg		��Ϣ����
 *         [in] wParam		�Զ���
 *         [in] lParam		�Զ���
 *         [out] bHandled	��Ϣ�Ƿ��Ѿ�������
 * ����ֵ��������
 ------------------------------------------------------------------------------*/
LRESULT CVideoWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (uMsg == WM_LBUTTONDBLCLK && m_pEventCallback)
		m_pEventCallback->OnEvent(uMsg, wParam, lParam);

	return S_OK;
}

/*------------------------------------------------------------------------------
 * ��  ���������¼�����ص�����
 * ��  ����[in] pCallback �ص�����
 * ����ֵ����
------------------------------------------------------------------------------*/
void CVideoWnd::SetEventCallback(IEventCallback* pCallback)
{
	m_pEventCallback = pCallback;
}