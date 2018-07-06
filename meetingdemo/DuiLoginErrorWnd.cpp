/*##############################################################################
 * �ļ���DuiLoginErrorWnd.cpp
 * ��������¼ʧ�ܴ���ʵ��
 * ���ߣ�Teck
 * ʱ�䣺2018��5��24��
 * ��Ȩ��Copyright(C) 2018 Fsmeeting.com. All rights reserved.
 ##############################################################################*/
#include "stdafx.h"
#include "DuiLoginErrorWnd.h"
#include "SdkManager.h"

// ��Ϣӳ��
DUI_BEGIN_MESSAGE_MAP(CDuiLoginErrorWnd, WindowImplBase)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()

/*------------------------------------------------------------------------------
 * ��  �������캯��
 * ��  ����[in] result ��¼���
 * ����ֵ����
------------------------------------------------------------------------------*/
CDuiLoginErrorWnd::CDuiLoginErrorWnd(fsp::ErrCode result)
	: m_eLoginResult(result)
{
}

/*------------------------------------------------------------------------------
 * ��  ������������
 * ��  ������
 * ����ֵ����
------------------------------------------------------------------------------*/
CDuiLoginErrorWnd::~CDuiLoginErrorWnd()
{
}

/*------------------------------------------------------------------------------
 * ��  �������⺯����DUI���û�ȡ����xml�����ļ�Ŀ¼
 * ��  ������
 * ����ֵ��xml�����ļ�Ŀ¼
 ------------------------------------------------------------------------------*/
CDuiString CDuiLoginErrorWnd::GetSkinFolder()
{
	return CDuiString(L"skin");
}

/*------------------------------------------------------------------------------
 * ��  �������⺯����DUI���û�ȡ����xml�����ļ�����
 * ��  ������
 * ����ֵ��xml�����ļ�����
 ------------------------------------------------------------------------------*/
CDuiString CDuiLoginErrorWnd::GetSkinFile()
{
	return CDuiString(L"login_error.xml");
}

/*------------------------------------------------------------------------------
 * ��  �������⺯����DUI���û�ȡ����������
 * ��  ������
 * ����ֵ������������
 ------------------------------------------------------------------------------*/
LPCTSTR CDuiLoginErrorWnd::GetWindowClassName(void) const
{
	return L"CDuiLoginErrorWnd";
}

/*------------------------------------------------------------------------------
 * ��  ����DUILIB�����Ϣ����
 * ��  ����[in] msg ֪ͨ��Ϣ
 * ����ֵ����
 ------------------------------------------------------------------------------*/
void CDuiLoginErrorWnd::OnClick(TNotifyUI& msg)
{
	if (msg.pSender->GetName() == L"relogin_btn")
	{
		CSdkManager::GetInstance().SetRestart();
		Close();
	}
	else
	{
		WindowImplBase::OnClick(msg);
	}
}

/*------------------------------------------------------------------------------
 * ��  �����������ٴ���
 * ��  ������
 * ����ֵ��LRESULT
 ------------------------------------------------------------------------------*/
LRESULT CDuiLoginErrorWnd::OnDestroy(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	::SendMessage(GetHWND(), WM_QUIT, 0, 0);
	return 0;
}

/*------------------------------------------------------------------------------
 * ��  ������������ǰ��ɾ������
 * ��  ����[in] hWnd ���ھ��
 * ����ֵ����
 ------------------------------------------------------------------------------*/
void CDuiLoginErrorWnd::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}

/*------------------------------------------------------------------------------
 * ��  ������Ҫ��������Ӧ�ó���ͼ��
 * ��  ������
 * ����ֵ����
 ------------------------------------------------------------------------------*/
void CDuiLoginErrorWnd::InitWindow()
{
	SetIcon(IDI_FSPCLIENT);

	CDuiString strErrInfo = L"δ֪����";
	if (m_eLoginResult == ErrCode::ERR_TOKEN_INVALID)
		strErrInfo = L"��֤ʧ�ܣ�";
	else if (m_eLoginResult == ErrCode::ERR_CONNECT_FAIL)
		strErrInfo = L"���ӷ�����ʧ�ܣ�";
	else if (m_eLoginResult == ErrCode::ERR_APP_NOT_EXIST)
		strErrInfo = L"Ӧ�ò����ڣ�";
	else if (m_eLoginResult == ErrCode::ERR_USERID_CONFLICT)
		strErrInfo = L"�û��ѵ�¼��";
	else if (m_eLoginResult == ErrCode::ERR_NO_BALANCE)
		strErrInfo = L"�˻����㣡";
	else
	{
		WCHAR szTmp[8];
		_snwprintf(szTmp, 8, L"%d", m_eLoginResult);
		strErrInfo.Append(szTmp);
	}
		
	m_PaintManager.FindControl(L"error_info")->SetText(strErrInfo);
}