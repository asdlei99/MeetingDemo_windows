/*##############################################################################
 * �ļ���DuiFrameWnd.cpp
 * ������������ʵ��
 * ���ߣ�Teck
 * ʱ�䣺2018��5��24��
 * ��Ȩ��Copyright(C) 2018 Fsmeeting.com. All rights reserved.
 ##############################################################################*/

#include "stdafx.h"
#include "DuiFrameWnd.h"
#include "core/UIDefine.h"
#include "UIMenu.h"
#include "ToolbarMenu.h"
#include "fsp_engine.h"
#include "SdkManager.h"
#include "DuiSettingWnd.h"
#include "util.h"
#include "define.h"

#define MENU_ITEM_HEIGHT	36
#define MENU_ITEM_WIDTH		220

// ��Ϣӳ�䶨��
DUI_BEGIN_MESSAGE_MAP(CDuiFrameWnd, WindowImplBase)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_SELECTCHANGED, OnSelectChanged)
DUI_END_MESSAGE_MAP()

/*------------------------------------------------------------------------------
 * ��  �������캯��
 * ��  ������
 * ����ֵ����
 ------------------------------------------------------------------------------*/
CDuiFrameWnd::CDuiFrameWnd()
	: m_bVideoWndInitFlag(false)
	, m_bBroadcastMic(false)
	, m_bOpenSpeaker(false)
	, m_bPlayAudio(false)
	, m_bRecord(false)
{
}

/*------------------------------------------------------------------------------
 * ��  ������������
 * ��  ������
 * ����ֵ����
 ------------------------------------------------------------------------------*/
CDuiFrameWnd::~CDuiFrameWnd()
{
	if (m_bRecord)
		StopRecord();
}

/*------------------------------------------------------------------------------
 * ��  �������⺯����DUI���û�ȡ����xml�����ļ�Ŀ¼
 * ��  ������
 * ����ֵ��xml�����ļ�Ŀ¼
 ------------------------------------------------------------------------------*/
CDuiString CDuiFrameWnd::GetSkinFolder()
{
	return CDuiString(L"skin");
}

/*------------------------------------------------------------------------------
 * ��  �������⺯����DUI���û�ȡ����xml�����ļ�����
 * ��  ������
 * ����ֵ��xml�����ļ�����
 ------------------------------------------------------------------------------*/
CDuiString CDuiFrameWnd::GetSkinFile()
{
	return CDuiString(L"main.xml");
}

/*------------------------------------------------------------------------------
 * ��  �������⺯����DUI���û�ȡ����������
 * ��  ������
 * ����ֵ������������
 ------------------------------------------------------------------------------*/
LPCTSTR CDuiFrameWnd::GetWindowClassName(void) const
{
	return L"CDuiFrameWnd";
}

/*------------------------------------------------------------------------------
 * ��  �������⺯����DUI������ɴ��ڳ�ʼ��
 * ��  ������
 * ����ֵ����
 ------------------------------------------------------------------------------*/
void CDuiFrameWnd::InitWindow()
{
	if (!m_bVideoWndInitFlag)
	{
		m_VideoWndMgr.Init(GetHWND());
		m_bVideoWndInitFlag = true;
	}

	WCHAR szCaption[128];
	_snwprintf(szCaption, 64, L"Group ID��%s    User ID��%s", 
		CSdkManager::GetInstance().GetLoginGroup().GetData(), 
		CSdkManager::GetInstance().GetLoginUser().GetData());

	CLabelUI* pCapLabel = (CLabelUI*)m_PaintManager.FindControl(L"group_user");
	pCapLabel->SetText(szCaption);

	m_PaintManager.SetTimer(m_PaintManager.FindControl(L"btn_mic"), 
		UPDATE_VIDEO_INFO_TIMER_ID, UPDATE_VIDEO_INFO_INTERVAL);

	SetIcon(IDI_FSPCLIENT);
}

/*------------------------------------------------------------------------------
 * ��  ����ˢ�������ڹ�������˷米��ͼƬ���㲥״̬��δ�㲥״̬�ı���ͼƬ��һ��
 * ��  ������
 * ����ֵ����
------------------------------------------------------------------------------*/
void CDuiFrameWnd::RefreshMicBtnBkImg()
{
	CControlUI* pBtnMic = m_PaintManager.FindControl(L"btn_mic");
	if (m_bBroadcastMic)
	{
		pBtnMic->SetAttribute(L"normalimage", L"img\\video\\toolbar_mic_open.png");
		pBtnMic->SetAttribute(L"hotimage", L"img\\video\\toolbar_mic_open_hot.png");
		pBtnMic->SetAttribute(L"pushedimage", L"img\\video\\toolbar_mic_open_pressed.png");
	}
	else
	{
		pBtnMic->SetAttribute(L"normalimage", L"img\\video\\toolbar_mic.png");
		pBtnMic->SetAttribute(L"hotimage", L"img\\video\\toolbar_mic_hot.png");
		pBtnMic->SetAttribute(L"pushedimage", L"img\\video\\toolbar_mic_pressed.png");
	}
}

/*------------------------------------------------------------------------------
 * ��  ����ˢ�������ڹ�����¼�Ʊ���ͼƬ
 * ��  ������
 * ����ֵ����
------------------------------------------------------------------------------*/
void CDuiFrameWnd::RefreshRecordBtnBkImg()
{
	CControlUI* pBtnRecord = m_PaintManager.FindControl(L"btn_record");
	if (m_bRecord)
	{
		pBtnRecord->SetAttribute(L"normalimage", L"img\\video\\toolbar_recorder_open.png");
		pBtnRecord->SetAttribute(L"hotimage", L"img\\video\\toolbar_recorder_open_hot.png");
		pBtnRecord->SetAttribute(L"pushedimage", L"img\\video\\toolbar_recorder_open_pressed.png");
	}
	else
	{
		pBtnRecord->SetAttribute(L"normalimage", L"img\\video\\toolbar_recorder.png");
		pBtnRecord->SetAttribute(L"hotimage", L"img\\video\\toolbar_recorder_hot.png");
		pBtnRecord->SetAttribute(L"pushedimage", L"img\\video\\toolbar_recorder_pressed.png");
	}
}

/*------------------------------------------------------------------------------
 * ��  ����ˢ�������ڹ���������ͷ����ͼƬ���㲥״̬��δ�㲥״̬�ı���ͼƬ��һ��
 * ��  ������
 * ����ֵ����
 ------------------------------------------------------------------------------*/
void CDuiFrameWnd::RefreshCamBtnBkImg()
{
	CControlUI* pBtnAud = m_PaintManager.FindControl(L"btn_cam");
	if (!m_setCamOpenIndexes.empty())
	{
		pBtnAud->SetAttribute(L"normalimage", L"img\\video\\toolbar_cam_open.png");
		pBtnAud->SetAttribute(L"hotimage", L"img\\video\\toolbar_cam_open_hot.png");
		pBtnAud->SetAttribute(L"pushedimage", L"img\\video\\toolbar_cam_open_pressed.png");
	}
	else
	{
		pBtnAud->SetAttribute(L"normalimage", L"img\\video\\toolbar_cam.png");
		pBtnAud->SetAttribute(L"hotimage", L"img\\video\\toolbar_cam_hot.png");
		pBtnAud->SetAttribute(L"pushedimage", L"img\\video\\toolbar_cam_pressed.png");
	}
}

/*------------------------------------------------------------------------------
 * ��  ����ˢ�������ڹ�����������Ƶ����ͼƬ
 * ��  ������
 * ����ֵ����
 ------------------------------------------------------------------------------*/
void CDuiFrameWnd::RefreshAudBtnBkImg()
{
	CControlUI* pBtnAud = m_PaintManager.FindControl(L"btn_aud");
	if (m_bPlayAudio)
	{
		pBtnAud->SetAttribute(L"normalimage", L"img\\video\\toolbar_audio_open.png");
		pBtnAud->SetAttribute(L"hotimage", L"img\\video\\toolbar_audio_open_hot.png");
		pBtnAud->SetAttribute(L"pushedimage", L"img\\video\\toolbar_audio_open_pressed.png");
	}
	else
	{
		pBtnAud->SetAttribute(L"normalimage", L"img\\video\\toolbar_audio.png");
		pBtnAud->SetAttribute(L"hotimage", L"img\\video\\toolbar_audio_hot.png");
		pBtnAud->SetAttribute(L"pushedimage", L"img\\video\\toolbar_audio_pressed.png");
	}
}

/*------------------------------------------------------------------------------
 * ��  ��������ͷѡ��仯����ͨ���˵���
 * ��  ����[in] msg ֪ͨ��Ϣ
 * ����ֵ����
 ------------------------------------------------------------------------------*/
void CDuiFrameWnd::OnCamSelectChanged(TNotifyUI& msg)
{
	DWORD dwCamIndex = _wtoi(msg.pSender->GetName().GetData() + 4);

	COptionUI* pCamOpt = (COptionUI*)msg.pSender;

	if (pCamOpt->IsSelected())
	{
		if (m_setCamOpenIndexes.end() != m_setCamOpenIndexes.find(dwCamIndex))
			return; // �쳣���
		
		//�������㲥��·��Ƶ!
		if (m_setCamOpenIndexes.size() < 2)
		{
			m_setCamOpenIndexes.insert(dwCamIndex);
			if (!m_VideoWndMgr.AddBroadcastCam(dwCamIndex))
				pCamOpt->Selected(false);
		}
		else
		{
			pCamOpt->SetAttribute(L"selected", L"false");

			// �˵���Ϣ��������ֱ�ӵ���MessageBox������������Ϊ�˵�ʧȥ����
			// ���ٺ��ֽ�����Ϣ��������±������������﷢��Ϣ�����̵߳�����ʾ
			this->PostMessageW(DUILIB_MSG_TOO_MANY_CAM, 0, 0);
		}
	}
	else // δѡ��״̬ҲҪ����
	{
		if (m_setCamOpenIndexes.end() != m_setCamOpenIndexes.find(dwCamIndex))
		{
			m_setCamOpenIndexes.erase(dwCamIndex);
			m_VideoWndMgr.DelBroadcastCam(dwCamIndex);
		}
	}

	RefreshCamBtnBkImg();
}

/*------------------------------------------------------------------------------
 * ��  ����Option�ؼ�ѡ��仯֪ͨ������Ϣӳ�䴦����
 * ��  ����[in] msg ֪ͨ��Ϣ
 * ����ֵ����
 ------------------------------------------------------------------------------*/
void CDuiFrameWnd::OnSelectChanged(TNotifyUI& msg)
{
	if (_tcsncmp(msg.pSender->GetName(), L"cam_", 4) == 0)
	{
		OnCamSelectChanged(msg);
	}
}

/*------------------------------------------------------------------------------
 * ��  ������������ڹ�������˷簴ť������
 * ��  ����[in] msg ֪ͨ��Ϣ
 * ����ֵ����
 ------------------------------------------------------------------------------*/
void CDuiFrameWnd::OnClickMicBtn(TNotifyUI& msg)
{
	DWORD dwMicIndex = CSdkManager::GetInstance().GetOpenMic();

	if (dwMicIndex == INVALID_MIC_INDEX)
	{
		m_bBroadcastMic = false;
		return;
	}
		
	if (m_bBroadcastMic)
	{
		m_VideoWndMgr.DelBroadcastMic(dwMicIndex);
		m_bBroadcastMic = false;
	}
	else
	{
		m_VideoWndMgr.AddBroadcastMic(dwMicIndex);
		m_bBroadcastMic = true;
	}

	RefreshMicBtnBkImg();
}

/*------------------------------------------------------------------------------
 * ��  ������������ڹ�����������Ƶ��ť������
 * ��  ����[in] msg ֪ͨ��Ϣ
 * ����ֵ����
 ------------------------------------------------------------------------------*/
void CDuiFrameWnd::OnClickAudBtn(TNotifyUI& msg)
{
	if (m_bPlayAudio)
	{
		CSdkManager::GetInstance().GetFspEngin()->StopPlayAudio();
		m_bPlayAudio = false;
	}
	else
	{
		CSdkManager::GetInstance().GetFspEngin()->StartPlayAudio();
		m_bPlayAudio = true;
	}

	RefreshAudBtnBkImg();
}

/*------------------------------------------------------------------------------
 * ��  ��������ͷ�Ƿ��Ѿ����㲥��
 * ��  ����[in] dwCamIndex ����ͷ����
 * ����ֵ����/��
 ------------------------------------------------------------------------------*/
bool CDuiFrameWnd::IsCamOpened(DWORD dwCamIndex)
{
	return m_setCamOpenIndexes.end() != m_setCamOpenIndexes.find(dwCamIndex);
}

/*------------------------------------------------------------------------------
 * ��  ������������湤��������ͷ��ť����
 * ��  ����[in] msg ֪ͨ��Ϣ
 * ����ֵ����
 ------------------------------------------------------------------------------*/
void CDuiFrameWnd::OnClickCamBtn(TNotifyUI& msg)
{
	// �����˵�
	CToolbarMenu* pMenu = new CToolbarMenu(GetHWND(), &m_PaintManager, this);
	if (!pMenu || !pMenu->Init(MENU_ITEM_HEIGHT, MENU_ITEM_WIDTH, 20))
		return;

	// ��SDK��ȡ���п�������ͷ
	fsp::IFspEngine* pEngin = CSdkManager::GetInstance().GetFspEngin();
	fsp::Vector<VideoDeviceInfo> vecCam = pEngin->GetDeviceManager()->GetCameraDevices();

	// ������ͷ��ӵ��˵���
	DWORD dwMenuItemIndex = 0;
	for (auto iter = vecCam.begin(); iter != vecCam.end(); ++iter)
	{
		WCHAR szMenuItemName[64];
		swprintf(szMenuItemName, L"cam_menu_%d", dwMenuItemIndex);

		WCHAR szOptionName[64];
		swprintf(szOptionName, L"cam_%d", dwMenuItemIndex);

		WCHAR szCamName[128];
		demo::ConvertUtf8ToUnicode(iter->device_name.c_str(), szCamName, 128);

		pMenu->AddMenuItem(szMenuItemName,
			CToolbarMenu::MENU_ITEM_TYPE_CHECKBOX,
			szOptionName,
			szCamName,
			L"",
			IsCamOpened(dwMenuItemIndex));

		dwMenuItemIndex++;
	}

	// ����˵���ʾλ�ã�������ʾ��
	RECT rcBtnPos = msg.pSender->GetClientPos();
	CDuiPoint point;
	point.x = (rcBtnPos.right + rcBtnPos.left) / 2 - MENU_ITEM_WIDTH / 2;
	point.y = rcBtnPos.top - 5 - MENU_ITEM_HEIGHT * dwMenuItemIndex;
	ClientToScreen(m_hWnd, &point);

	// ��ʾ�˵�
	pMenu->ShowMenu(point);
}

/*------------------------------------------------------------------------------
 * ��  ������������湤�������ð�ť����
 * ��  ����[in] msg ֪ͨ��Ϣ
 * ����ֵ����
 ------------------------------------------------------------------------------*/
void CDuiFrameWnd::OnClickSettingBtn(TNotifyUI& msg)
{
	CDuiSettingWnd wndSetting;
	wndSetting.Create(m_hWnd, _T("CDuiSettingWnd"), UI_WNDSTYLE_DIALOG, WS_EX_WINDOWEDGE);
	wndSetting.CenterWindow();
	wndSetting.ShowModal();
}

/*------------------------------------------------------------------------------
 * ��  ����ֹͣ¼��
 * ��  ������
 * ����ֵ����
 ------------------------------------------------------------------------------*/
void CDuiFrameWnd::StopRecord()
{
	// ֹͣ¼�Ʊ�����Ƶ
	fsp::IFspEngine* pEngin = CSdkManager::GetInstance().GetFspEngin();
	auto vecCam = pEngin->GetDeviceManager()->GetCameraDevices();
	for (auto iter = vecCam.begin(); iter != vecCam.end(); ++iter)
	{
		CSdkManager::GetInstance().StopRecordLocalVideo(iter->camera_id);
	}

	// ֹͣ¼��Զ����Ƶ
	for each (RemoteVideoInfo info in m_vecRemoteVideoInfo)
	{
		CSdkManager::GetInstance().StopRecordRemoteVideo(
			info.strUserId.c_str(), info.strVideoId.c_str());
	}

	// ֹͣ¼�Ʊ�����Ƶ
	CSdkManager::GetInstance().StopRecordLocalAudio();

	// ֹͣ¼��Զ����Ƶ
	for each (RemoteAudioInfo info in m_vecRemoteAudioInfo)
	{
		CSdkManager::GetInstance().StopRecordRemoteAudio(info.strUserId.c_str());
	}
}

/*------------------------------------------------------------------------------
 * ��  ������������湤����¼�ư�ť����
 * ��  ����[in] msg ֪ͨ��Ϣ
 * ����ֵ����
 ------------------------------------------------------------------------------*/
void CDuiFrameWnd::OnClickRecordBtn(TNotifyUI& msg)
{
	if (m_bRecord)
	{
		StopRecord();

		m_bRecord = false;
	}
	else
	{
		// ¼�Ʊ�����Ƶ
		fsp::IFspEngine* pEngin = CSdkManager::GetInstance().GetFspEngin();
		auto vecCam = pEngin->GetDeviceManager()->GetCameraDevices();
		for (auto iter = vecCam.begin(); iter != vecCam.end(); ++iter)
		{
			CSdkManager::GetInstance().StartRecordLocalVideo(iter->camera_id);
		}

		// ¼��Զ����Ƶ
		for each (RemoteVideoInfo info in m_vecRemoteVideoInfo)
		{
			CSdkManager::GetInstance().StartRecordRemoteVideo(
				info.strUserId.c_str(), info.strVideoId.c_str());
		}

		// ¼�Ʊ�����Ƶ
		CSdkManager::GetInstance().StartRecordLocalAudio();

		// ¼��Զ����Ƶ
		for each (RemoteAudioInfo info in m_vecRemoteAudioInfo)
		{
			CSdkManager::GetInstance().StartRecordRemoteAudio(info.strUserId.c_str());
		}

		m_bRecord = true;
	}

	RefreshRecordBtnBkImg();
}

/*------------------------------------------------------------------------------
 * ��  ������Ϣӳ�䴦����
 * ��  ����[in] msg ֪ͨ��Ϣ
 * ����ֵ����
 ------------------------------------------------------------------------------*/
void CDuiFrameWnd::OnClick(TNotifyUI& msg)
{
	if (msg.pSender->GetName() == L"btn_mic")
	{
		OnClickMicBtn(msg);
	}
	else if (msg.pSender->GetName() == L"btn_cam")
	{
		OnClickCamBtn(msg);
	}
	else if (msg.pSender->GetName() == L"btn_aud")
	{
		OnClickAudBtn(msg);
	}
	else if (msg.pSender->GetName() == L"btn_setting")
	{
		OnClickSettingBtn(msg);
	}
	else if (msg.pSender->GetName() == L"btn_record")
	{
		OnClickRecordBtn(msg);
	}
	else
	{
		WindowImplBase::OnClick(msg);
	}
}

/*------------------------------------------------------------------------------
 * ��  �������⺯�����������˳�ʱ��������Ҫ�˳�
 * ��  ����[in] UINT
 *         [in] WPARAM
 *         [in] LPARAM
 *         [out] bHandled ��Ϣ�Ƿ��Ѿ�������
 * ����ֵ��������(LRESULT)
 ------------------------------------------------------------------------------*/
LRESULT CDuiFrameWnd::OnDestroy(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	::SendMessage(GetHWND(), WM_QUIT, 0, 0);
	return S_OK;
}

/*------------------------------------------------------------------------------
 * ��  �������⺯���������˳�֮ǰ�Ļص�
 * ��  ����[in] hWnd ���ھ��
 * ����ֵ����
 ------------------------------------------------------------------------------*/
void CDuiFrameWnd::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}

/*------------------------------------------------------------------------------
 * ��  ����SDK֪ͨӦ�ò����û��㲥����Ƶ�Ĵ���
 * ��  ����[in] wParam Я��RemoteAudioInfo�ṹ��ָ��
 *         [in] lParam û��ʹ��
 * ����ֵ����
 ------------------------------------------------------------------------------*/
void CDuiFrameWnd::OnAddRemoteAudio(WPARAM wParam, LPARAM lParam)
{
	RemoteAudioInfo* pInfo = (RemoteAudioInfo*)wParam;

	m_VideoWndMgr.AddRemoteAudio(pInfo->strUserId);

	m_vecRemoteAudioInfo.push_back(*pInfo);

	if (m_bRecord) // ���¼��
	{
		CSdkManager::GetInstance().StartRecordRemoteAudio(pInfo->strUserId.c_str());
	}

	delete pInfo;
}

/*------------------------------------------------------------------------------
 * ��  ����SDK֪ͨӦ�ò����û�ȡ���㲥����Ƶ�Ĵ���
 * ��  ����[in] wParam Я��RemoteAudioInfo�ṹ��ָ��
 *         [in] lParam û��ʹ��
 * ����ֵ����
 ------------------------------------------------------------------------------*/
void CDuiFrameWnd::OnDelRemoteAudio(WPARAM wParam, LPARAM lParam)
{
	RemoteAudioInfo* pInfo = (RemoteAudioInfo*)wParam;

	m_VideoWndMgr.DelRemoteAudio(pInfo->strUserId);

	for (auto iter = m_vecRemoteAudioInfo.begin(); iter != m_vecRemoteAudioInfo.end(); ++iter)
	{
		if (iter->strUserId == pInfo->strUserId)
		{
			m_vecRemoteAudioInfo.erase(iter);
			break;
		}
	}

	if (m_bRecord) // ֹͣ¼��
	{
		CSdkManager::GetInstance().StopRecordRemoteAudio(pInfo->strUserId.c_str());
	}

	delete pInfo;
}

/*------------------------------------------------------------------------------
 * ��  ����SDK֪ͨӦ�ò����û��㲥����Ƶ�Ĵ���
 * ��  ����[in] wParam Я��RemoteVideoInfo�ṹ��ָ��
 *         [in] lParam û��ʹ��
 * ����ֵ����
 ------------------------------------------------------------------------------*/
void CDuiFrameWnd::OnAddRemoteVideo(WPARAM wParam, LPARAM lParam)
{
	RemoteVideoInfo* pInfo = (RemoteVideoInfo*)wParam;

	m_VideoWndMgr.AddRemoteVideo(pInfo->strUserId, pInfo->strVideoId);

	m_vecRemoteVideoInfo.push_back(*pInfo);

	if (m_bRecord) // ���¼��
	{
		CSdkManager::GetInstance().StartRecordRemoteVideo(
			pInfo->strUserId.c_str(), pInfo->strVideoId.c_str());
	}

	delete pInfo;
}

/*------------------------------------------------------------------------------
 * ��  ����SDK֪ͨӦ�ò����û�ȡ���㲥����Ƶ�Ĵ���
 * ��  ����[in] wParam Я��RemoteVideoInfo�ṹ��ָ��
 *         [in] lParam û��ʹ��
 * ����ֵ����
 ------------------------------------------------------------------------------*/
void CDuiFrameWnd::OnDelRemoteVideo(WPARAM wParam, LPARAM lParam)
{
	RemoteVideoInfo* pInfo = (RemoteVideoInfo*)wParam;

	m_VideoWndMgr.DelRemoteVideo(pInfo->strUserId, pInfo->strVideoId);

	for (auto iter = m_vecRemoteVideoInfo.begin(); iter != m_vecRemoteVideoInfo.end(); ++iter)
	{
		if (iter->strUserId == pInfo->strUserId && iter->strVideoId == pInfo->strVideoId)
		{
			m_vecRemoteVideoInfo.erase(iter);
			break;
		}
	}

	if (m_bRecord) // ֹͣ¼��
	{
		CSdkManager::GetInstance().StopRecordRemoteVideo(
			pInfo->strUserId.c_str(), pInfo->strVideoId.c_str());
	}

	delete pInfo;
}

/*------------------------------------------------------------------------------
 * ��  �����л��㲥����ͷ����
 * ��  ����[in] wParam �л�֮ǰ������ͷ����
 *         [in] lParam �л�֮�������ͷ����
 * ����ֵ����
 ------------------------------------------------------------------------------*/
void CDuiFrameWnd::OnBroadcastCamChanged(WPARAM wParam, LPARAM lParam)
{
	DWORD dwOldCamIndex = (DWORD)wParam;
	DWORD dwNewCamIndex = (DWORD)lParam;

	if (m_setCamOpenIndexes.end() != m_setCamOpenIndexes.find(dwOldCamIndex))
	{
		m_setCamOpenIndexes.erase(dwOldCamIndex);
	}

	if (m_setCamOpenIndexes.end() == m_setCamOpenIndexes.find(dwNewCamIndex))
	{
		m_setCamOpenIndexes.insert(dwNewCamIndex);
	}
}

/*------------------------------------------------------------------------------
 * ��  ������ȡ�������м䲿����ʾ����
 * ��  ������
 * ����ֵ����������
 ------------------------------------------------------------------------------*/
RECT CDuiFrameWnd::GetDisplayRect()
{
	RECT rect;
	::GetClientRect(GetHWND(), &rect);
	rect.top += UI_MAIN_FRAME_TITLE_HEIGHT;	
	rect.bottom -= UI_MAIN_FRAME_TOOLBAR_HEIGHT;

	return rect;
}

/*------------------------------------------------------------------------------
 * ��  ���������ڴ�С�仯������Group ID��User ID��ʾλ��
 * ��  ������
 * ����ֵ����
 ------------------------------------------------------------------------------*/
void CDuiFrameWnd::AdjustTitleGroupUser()
{
	RECT rcClient = GetDisplayRect();

	RECT rectPadding;
	ZeroMemory(&rectPadding, sizeof(rectPadding));

	// ħ�����ֲο�duilib�Ĳ����ļ���main.xml��
	rectPadding.left = (rcClient.right - rcClient.left) / 2 - 400 - 130;

	(m_PaintManager.FindControl(L"group_user"))->SetPadding(rectPadding);
}

/*------------------------------------------------------------------------------
 * ��  ���������ڴ�С�仯������Toolbar��ť����ʾλ��
 * ��  ������
 * ����ֵ����
 ------------------------------------------------------------------------------*/
void CDuiFrameWnd::AdjustToolbarBtn()
{
	RECT rcClient = GetDisplayRect();

	RECT rectPadding;
	ZeroMemory(&rectPadding, sizeof(rectPadding));

	// ħ�����ֲο�duilib�Ĳ����ļ���main.xml��
	rectPadding.left	= (rcClient.right - rcClient.left - 460) / 2;
	rectPadding.top		= 15;
	rectPadding.right	= 20;
	rectPadding.bottom	= 15;

	(m_PaintManager.FindControl(L"btn_mic"))->SetPadding(rectPadding);
}

/*------------------------------------------------------------------------------
 * ��  ������ʱ���ص���������ˢ�±��������ϴ���������
 * ��  ������
 * ����ֵ����
 ------------------------------------------------------------------------------*/
void CDuiFrameWnd::OnTimer()
{
	static int count = 0; // ������ʱ1��

	if (++count >= 5) // Timer�����200ms����������ÿ��ˢ���ϴ���������
	{
		StreamStats stats = CSdkManager::GetInstance().GetFspEngin()->GetStreamStats(true);

		WCHAR szCaption[256];
		ZeroMemory(szCaption, sizeof(szCaption));

		_snwprintf(szCaption, 256, L"Group ID��%s    User ID��%s    Up��%d kbps    Down��%d kbps",
			CSdkManager::GetInstance().GetLoginGroup().GetData(),
			CSdkManager::GetInstance().GetLoginUser().GetData(),
			stats.send_data_size * 8 / 1024, stats.recv_data_size * 8 / 1024);

		CLabelUI* pCapLabel = (CLabelUI*)m_PaintManager.FindControl(L"group_user");
		pCapLabel->SetText(szCaption);

		count = 0;
	}
	
	m_VideoWndMgr.OnTimer(); // �����߳�ˢ������Ƶ״̬��Ϣ
}

/*------------------------------------------------------------------------------
 * ��  ����SDK֪ͨӦ�ò����û��㲥����Ƶ�Ĵ���
 * ��  ����[in] uMsg		��Ϣ����
 *         [in] wParam		�Զ���
 *         [in] lParam		�Զ���
 *         [out] bHandled	��Ϣ�Ƿ��Ѿ�������
 * ����ֵ��������
 ------------------------------------------------------------------------------*/
LRESULT CDuiFrameWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (!m_bVideoWndInitFlag)
		return S_OK;

	switch (uMsg)
	{
	case WM_TIMER:
		OnTimer();
		break;

	case WM_SIZE:
		m_VideoWndMgr.SetWndRect(GetDisplayRect());
		AdjustToolbarBtn();
		AdjustTitleGroupUser();
		break;

	case WM_GETMINMAXINFO:
	{
		RECT rt;
		SystemParametersInfo(SPI_GETWORKAREA, 0, &rt, 0);
		((MINMAXINFO*)lParam)->ptMaxSize.y = (rt.bottom - rt.top);
		break;
	}

	case DUILIB_MSG_ADD_REMOTE_VIDEO:
		OnAddRemoteVideo(wParam, lParam);
		break;

	case DUILIB_MSG_DEL_REMOTE_VIDEO:
		OnDelRemoteVideo(wParam, lParam);
		break;

	case DUILIB_MSG_ADD_REMOTE_AUDIO:
		OnAddRemoteAudio(wParam, lParam);
		break;

	case DUILIB_MSG_DEL_REMOTE_AUDIO:
		OnDelRemoteAudio(wParam, lParam);
		break;

	case DUILIB_MSG_TOO_MANY_CAM:
		demo::ShowMessageBox(m_hWnd, STRING_TOO_MANY_CAM);
		break;

	case DUILIB_MSG_BROADCAST_CAM_CHANGED:
		OnBroadcastCamChanged(wParam, lParam);
		break;

	case DUILIB_MSG_CHANGE_CAM_FAILED:
		demo::ShowMessageBox(m_hWnd, STRING_CHANGE_CAM_FAILED);
		break;

	case DUILIB_MSG_BROADCAST_CAM_FAILED:
		demo::ShowMessageBox(m_hWnd, STRING_BROADCAST_CAM_FAILED);
		break;

	case DUILIB_MSG_VIDEO_PARAM_CHANGED:
		m_VideoWndMgr.OnVideoParamChanged();
		break;

	default:
		break;
	}

	return S_OK;
}