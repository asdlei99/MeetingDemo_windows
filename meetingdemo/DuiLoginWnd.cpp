/*##############################################################################
 * �ļ���DuiLoginWnd.cpp
 * ��������¼����ʵ��
 * ���ߣ�Teck
 * ʱ�䣺2018��5��24��
 * ��Ȩ��Copyright(C) 2018 Fsmeeting.com. All rights reserved.
 ##############################################################################*/
#include "stdafx.h"
#include "DuiLoginWnd.h"
#include "DuiLoginWaitWnd.h"
#include "SdkManager.h"
#include "DuiAppConfigWnd.h"
#include "util.h"
#include <atlconv.h>

 // ��Ϣӳ��
DUI_BEGIN_MESSAGE_MAP(CDuiLoginWnd, WindowImplBase)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()

/*------------------------------------------------------------------------------
 * ��  �������캯��
 * ��  ������
 * ����ֵ����
------------------------------------------------------------------------------*/
CDuiLoginWnd::CDuiLoginWnd()
{
}

/*------------------------------------------------------------------------------
 * ��  ������������
 * ��  ������
 * ����ֵ����
------------------------------------------------------------------------------*/
CDuiLoginWnd::~CDuiLoginWnd()
{
}

/*------------------------------------------------------------------------------
 * ��  �������⺯����DUI���û�ȡ����xml�����ļ�Ŀ¼
 * ��  ������
 * ����ֵ��xml�����ļ�Ŀ¼
 ------------------------------------------------------------------------------*/
CDuiString CDuiLoginWnd::GetSkinFolder()
{
	return CDuiString(L"skin");
}

/*------------------------------------------------------------------------------
 * ��  �������⺯����DUI���û�ȡ����xml�����ļ�����
 * ��  ������
 * ����ֵ��xml�����ļ�����
 ------------------------------------------------------------------------------*/
CDuiString CDuiLoginWnd::GetSkinFile()
{
	return CDuiString(L"login.xml");
}

/*------------------------------------------------------------------------------
 * ��  �������⺯����DUI���û�ȡ����������
 * ��  ������
 * ����ֵ������������
 ------------------------------------------------------------------------------*/
LPCTSTR CDuiLoginWnd::GetWindowClassName(void) const
{
	return L"CDuiLoginWnd";
}

/*------------------------------------------------------------------------------
 * ��  ����DUILIB�����Ϣ����
 * ��  ����[in] msg ֪ͨ��Ϣ
 * ����ֵ����
 ------------------------------------------------------------------------------*/
void CDuiLoginWnd::OnClick(TNotifyUI& msg)
{
	if (msg.pSender->GetName() == L"login_btn")
	{
		CEditUI* pGroupEdit = (CEditUI*)m_PaintManager.FindControl(L"group_name_edit");
		CEditUI* pUserEdit = (CEditUI*)m_PaintManager.FindControl(L"user_name_edit");

		assert(pGroupEdit && pUserEdit);

		LPCTSTR szGroup = pGroupEdit->GetText();
		LPCTSTR szUser = pUserEdit->GetText();

		if (wcscmp(szGroup, L"") == 0 || wcscmp(szUser, L"") == 0)
		{
			demo::ShowMessageBox(m_hWnd, L"���������ID���û�ID");
		}
		else if (wcslen(szGroup) >= 32 || wcslen(szUser) >= 32)
		{
			demo::ShowMessageBox(m_hWnd, L"����ID�����û�ID̫���ˣ�");
		}
		else
		{
			CSdkManager::GetInstance().JoinGroup(szGroup, szUser);
		}
	}
	else if (msg.pSender->GetName() == L"app_config_btn")
	{
		CDuiAppConfigWnd wndAppConfig;
		wndAppConfig.Create(m_hWnd, _T("CDuiAppConfigWnd"), UI_WNDSTYLE_DIALOG, WS_EX_WINDOWEDGE);
		wndAppConfig.CenterWindow();
		wndAppConfig.ShowModal();
	}
	else
	{
		WindowImplBase::OnClick(msg);
	}
}

/*------------------------------------------------------------------------------
 * ��  ������Ҫ��������Ӧ�ó���ͼ��
 * ��  ������
 * ����ֵ����
 ------------------------------------------------------------------------------*/
void CDuiLoginWnd::InitWindow()
{
	SetIcon(IDI_FSPCLIENT);

	CLabelUI* pLabel = (CLabelUI*)m_PaintManager.FindControl(L"login_sdkver");

	WCHAR wszVersion[128] = { 0 };
	demo::ConvertUtf8ToUnicode(CSdkManager::GetInstance().GetFspEngin()->GetVersion().c_str(),
		wszVersion, 128);
	CDuiString strVerInfo;
	strVerInfo.Format(L"SdkVersion:  %s", wszVersion);
	pLabel->SetText(strVerInfo);
}

/*------------------------------------------------------------------------------
 * ��  �����������ٴ���
 * ��  ������
 * ����ֵ��LRESULT
 ------------------------------------------------------------------------------*/
LRESULT CDuiLoginWnd::OnDestroy(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	PostQuitMessage(0);
	return 0;
}