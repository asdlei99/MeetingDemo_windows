/*##############################################################################
 * �ļ���MediaInfoWnd.cpp
 * ������ý����Ϣ��ʾ����ʵ��
 * ���ߣ�Teck
 * ʱ�䣺2018��5��24��
 * ��Ȩ��Copyright(C) 2018 Fsmeeting.com. All rights reserved.
 ##############################################################################*/
#include "stdafx.h"
#include "MediaInfoWnd.h"

/*------------------------------------------------------------------------------
 * ��  �������캯��
 * ��  ������
 * ����ֵ����
 ------------------------------------------------------------------------------*/
CMediaInfoWnd::CMediaInfoWnd(LPCTSTR szLayoutXml, LPCTSTR szWndClassName)
	: CFloatWnd(szLayoutXml, szWndClassName)
{
}

/*------------------------------------------------------------------------------
 * ��  ������������
 * ��  ������
 * ����ֵ����
 ------------------------------------------------------------------------------*/
CMediaInfoWnd::~CMediaInfoWnd()
{
}

/*------------------------------------------------------------------------------
 * ��  �������ô�����ʾ�ľ�������
 * ��  ����[in] rectWnd ��������
 * ����ֵ����
------------------------------------------------------------------------------*/
void CMediaInfoWnd::SetWndRect(const RECT& rectWnd)
{
	DWORD dwWidth = rectWnd.right - rectWnd.left;

	RECT rectPadding;
	ZeroMemory(&rectPadding, sizeof(rectPadding));

	// normal size�Ŀ����400
	if (dwWidth > 400) // ���
	{
		// 76(�û���) + 110(������С) + 120(��Ƶ��Ϣ) + 30(�˵���ť)		
		rectPadding.left = (dwWidth - 336) / 3;
	}
	else // �ָ���ʼ��С
	{
		rectPadding.left = 10;
	}

	// ��������Ԫ��֮��ļ��
	CPaintManagerUI& paintMgr = GetPaintManager();
	((CHorizontalLayoutUI*)paintMgr.FindControl(L"audio_info"))->SetPadding(rectPadding);
	((CHorizontalLayoutUI*)paintMgr.FindControl(L"video_info"))->SetPadding(rectPadding);

	__super::SetWndRect(rectWnd);
}