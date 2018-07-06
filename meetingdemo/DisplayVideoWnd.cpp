/*##############################################################################
 * �ļ���DisplayVideoWnd.cpp
 * ��������ƵС����ʵ��
 * ���ߣ�Teck
 * ʱ�䣺2018��5��24��
 * ��Ȩ��Copyright(C) 2018 Fsmeeting.com. All rights reserved.
 ##############################################################################*/
#include "stdafx.h"
#include "DisplayVideoWnd.h"
#include "UIMenu.h"
#include "SdkManager.h"
#include "util.h"
#include "define.h"
#include "MediaInfoWnd.h"
#include "VideoWnd.h"

/*------------------------------------------------------------------------------
 * ��  �������캯��
 * ��  ������
 * ����ֵ����
------------------------------------------------------------------------------*/
CDisplayVideoWnd::CDisplayVideoWnd()
	: m_eRenderMode(fsp::RENDERMODE_SCALE_FILL)
	, m_pInfoWnd(nullptr)
	, m_pVideoWnd(nullptr)
	, m_bHasAudio(false)
	, m_bHasVideo(false)
	, m_dwCamIndex(0xFFFFFFFF)
	, m_dwMicIndex(0xFFFFFFFF)
	, m_bIsLocal(false)
	, m_bIsVideoMaximized(false)
	, m_pEventCallback(nullptr)
{
	ZeroMemory(&m_rectDisplay, sizeof(m_rectDisplay));
}

/*------------------------------------------------------------------------------
 * ��  ������������
 * ��  ������
 * ����ֵ����
------------------------------------------------------------------------------*/
CDisplayVideoWnd::~CDisplayVideoWnd()
{
	fsp::IFspEngine* pEngin = CSdkManager::GetInstance().GetFspEngin();

	if (m_bHasAudio && m_bIsLocal)
		pEngin->StopPublishAudio();

	if (m_bHasVideo && m_bIsLocal)
		pEngin->StopPublishVideo(m_strVideoId);

	if (m_pVideoWnd != nullptr)
		delete m_pVideoWnd;

	if (m_pInfoWnd != nullptr)
		delete m_pInfoWnd;
}

/*------------------------------------------------------------------------------
 * ��  ���������¼�����ص�����
 * ��  ����[in] pCallback �ص�����
 * ����ֵ����
------------------------------------------------------------------------------*/
void CDisplayVideoWnd::SetEventCallback(IEventCallback* pCallback)
{
	m_pEventCallback = pCallback;
}

/*------------------------------------------------------------------------------
 * ��  �����¼�����
 * ��  ����[in] uMsg	��Ϣ����
 *         [in] wParam	����
 *         [in] lParam	����
 * ����ֵ����
------------------------------------------------------------------------------*/
void CDisplayVideoWnd::OnEvent(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_LBUTTONDBLCLK)
		m_bIsVideoMaximized = !m_bIsVideoMaximized;

	if (m_pEventCallback)
		m_pEventCallback->OnEvent(uMsg, wParam, lParam);
}

/*------------------------------------------------------------------------------
 * ��  ���������Ƿ�˫���Ŵ���
 * ��  ������
 * ����ֵ����/��
------------------------------------------------------------------------------*/
bool CDisplayVideoWnd::IsVideoMaximized()
{
	return m_bIsVideoMaximized;
}

/*------------------------------------------------------------------------------
 * ��  ������ʾ�����ش���
 * ��  ����[in] bShow �Ƿ���ʾ����
 * ����ֵ����
------------------------------------------------------------------------------*/
void CDisplayVideoWnd::ShowWindow(bool bShow)
{
	m_pInfoWnd->ShowWindow(bShow);
	m_pVideoWnd->ShowWindow(bShow);
}

/*------------------------------------------------------------------------------
 * ��  ������ʼ��
 * ��  ����[in] hParentWnd	�����ھ��
 * ����ֵ����
------------------------------------------------------------------------------*/
void CDisplayVideoWnd::Init(HWND hParentWnd)
{
	m_hParentWnd = hParentWnd;

	m_pVideoWnd = new CVideoWnd(L"video_default.xml", L"VideoDefault");
	m_pVideoWnd->CreateWnd(hParentWnd, L"VideoWnd", UI_WNDSTYLE_CHILD, true);
	((CVideoWnd*)m_pVideoWnd)->SetEventCallback(this);

	m_pInfoWnd = new CMediaInfoWnd(L"video_info.xml", L"VideoInfo");
	m_pInfoWnd->CreateWnd(hParentWnd, L"InfoWnd", UI_WNDSTYLE_CHILD, true);
	m_pInfoWnd->GetPaintManager().AddNotifier(this);
}

/*------------------------------------------------------------------------------
 * ��  �������ô��ڵľ�������
 * ��  ����[in] rect ��������
 *         [in] bShow �ı����������Ƿ�������ʾ
 * ����ֵ����
------------------------------------------------------------------------------*/
void CDisplayVideoWnd::SetWndRect(const RECT& rect, bool bShow)
{
	m_rectDisplay = rect;

	RECT videoRect;
	videoRect.left	= rect.left;
	videoRect.top	= rect.top;
	videoRect.right	= rect.right;
	videoRect.bottom= rect.bottom - UI_VIDEO_INFO_HEIGHT;

	m_pVideoWnd->SetWndRect(videoRect);

	RECT infoRect;
	infoRect.left	= rect.left;
	infoRect.top	= rect.bottom - UI_VIDEO_INFO_HEIGHT;
	infoRect.right	= rect.right;
	infoRect.bottom = rect.bottom;

	m_pInfoWnd->SetWndRect(infoRect);
}

/*------------------------------------------------------------------------------
 * ��  �����㲥��˷�󣬸�����˷�ͼ��
 * ��  ������
 * ����ֵ����
------------------------------------------------------------------------------*/
void CDisplayVideoWnd::OpenMic()
{
	CLabelUI* pMicLabel = (CLabelUI*)m_pInfoWnd->GetPaintManager().FindControl(L"label_mic");
	pMicLabel->SetBkImage(L"img\\video\\video_stat_audio_open.png");
}

/*------------------------------------------------------------------------------
 * ��  ����ȡ���㲥��˷�󣬸�����˷�ͼ��
 * ��  ������
 * ����ֵ����
------------------------------------------------------------------------------*/
void CDisplayVideoWnd::CloseMic()
{
	CLabelUI* pMicLabel = (CLabelUI*)m_pInfoWnd->GetPaintManager().FindControl(L"label_mic");
	pMicLabel->SetBkImage(L"img\\video\\video_stat_audio.png");

	CLabelUI* pVolLabel = (CLabelUI*)m_pInfoWnd->GetPaintManager().FindControl(L"label_volume");
	pVolLabel->SetBkImage(L"img\\video\\video_stats_volume_bg.png");
}

/*------------------------------------------------------------------------------
 * ��  ����������˷���������ֵ��С���ı�����ֵͼƬ
 * ��  ����[in] dwEnergy ��˷���������ֵ��С
 * ����ֵ����
------------------------------------------------------------------------------*/
void CDisplayVideoWnd::OnMicEnergyChange(DWORD dwEnergy)
{
	WCHAR szBkImage[128];
	if (dwEnergy / 10 == 0)
		_snwprintf(szBkImage, 128, L"img\\video\\video_stats_volume_bg.png");
	else
		_snwprintf(szBkImage, 128, L"img\\video\\video_stats_volume%d.png", dwEnergy / 10);

	CLabelUI* pMicLabel = (CLabelUI*)m_pInfoWnd->GetPaintManager().FindControl(L"label_volume");
	pMicLabel->SetBkImage(szBkImage);
}

/*------------------------------------------------------------------------------
 * ��  ����ˢ������/֡��/�ֱ�����ʾ
 * ��  ������
 * ����ֵ����
------------------------------------------------------------------------------*/
void CDisplayVideoWnd::OnVideoStatsChange(LPCTSTR szVideoStats)
{
	CLabelUI* pVideoInfoLabel = (CLabelUI*)m_pInfoWnd->GetPaintManager().FindControl(L"label_video_info");
	pVideoInfoLabel->SetText(szVideoStats);
}

/*------------------------------------------------------------------------------
 * ��  ���������˵���
 * ��  ����[in] szName		�˵�������
 *         [in] szText		��ʾ���ı�
 *         [in] bRadio		�Ƿ��ǵ�ѡ��ť
 *         [in] bSelected	�Ƿ�Ĭ��ѡ��
 *         [in] bHasSubMenu	�Ƿ����Ӳ˵���
 * ����ֵ���˵���ָ��
------------------------------------------------------------------------------*/
CMenuElementUI* CDisplayVideoWnd::ConstructMenuItem(LPCTSTR szName, LPCTSTR szText, bool bRadio, bool bSelected, bool bHasSubMenu)
{
	CMenuElementUI* pElement = new CMenuElementUI;
	pElement->SetAttribute(L"width", L"180");
	pElement->SetAttribute(L"height", L"36");

	CHorizontalLayoutUI* pHLayout = new CHorizontalLayoutUI;
	pHLayout->SetAttribute(L"width", L"180");
	pHLayout->SetAttribute(L"height", L"36");

	if (bRadio)
	{
		COptionUI* pSelectOpt = new COptionUI;
		pSelectOpt->SetAttribute(L"name", szName);
		pSelectOpt->SetAttribute(L"height", L"16");
		pSelectOpt->SetAttribute(L"width", L"16");
		pSelectOpt->SetAttribute(L"padding", L"10, 10, 8, 10");
		pSelectOpt->SetAttribute(L"group", L"group");
		pSelectOpt->SetAttribute(L"normalimage", L"img\\video\\radio.png");
		pSelectOpt->SetAttribute(L"hotimage", L"img\\video\\radio_hot.png");
		pSelectOpt->SetAttribute(L"pushedimage", L"img\\video\\radio_pressed.png");
		pSelectOpt->SetAttribute(L"selectedimage", L"img\\video\\radio_sel.png");
		pSelectOpt->SetAttribute(L"selected", bSelected ? L"true" : L"false");

		pHLayout->Add(pSelectOpt);
	}
	else
	{
		CLabelUI* pPlaceholder = new CLabelUI;
		pPlaceholder->SetAttribute(L"width", L"34");

		pHLayout->Add(pPlaceholder);
	}

	CLabelUI* pTextLabel = new CLabelUI;
	pTextLabel->SetAttribute(L"height", L"36");
	pTextLabel->SetAttribute(L"width", L"116");
	pTextLabel->SetText(szText);
	pHLayout->Add(pTextLabel);

	if (bHasSubMenu)
	{
		CLabelUI* pSubmenuLabel = new CLabelUI;
		pSubmenuLabel->SetAttribute(L"height", L"8");
		pSubmenuLabel->SetAttribute(L"width", L"8");
		pSubmenuLabel->SetAttribute(L"padding", L"8,14,14,14");
		pSubmenuLabel->SetText(L"");
		pSubmenuLabel->SetAttribute(L"bkimage", L"img\\video\\more_menu_arrow.png");
		pHLayout->Add(pSubmenuLabel);
	}
	
	pElement->Add(pHLayout);

	return pElement;
}

/*------------------------------------------------------------------------------
 * ��  ���������˵�
 * ��  ����[in] pMenu �˵�����
 * ����ֵ����
------------------------------------------------------------------------------*/
void CDisplayVideoWnd::ConstructMenu(CMenuWnd* pMenu)
{
	// FIXME: xml�����ļ��ж���������һ���յ�MenuElement��Ϊռλ��������̬��ӵ�
	// �˵����޷���ʾ�������Ǹ�bug����ʱû��ʱ��ȥ�������������Ҫ�Ƴ�ռλ��
	((CMenuUI*)pMenu->m_pm.GetRoot())->Remove(pMenu->m_pm.FindControl(L"placeholder"));

	// �������Ƶ��ʾģʽ�˵���
	CMenuElementUI* pSfppElement = ConstructMenuItem(L"video_disp_sfpp", 
		L"��Ƶ����ƽ����ʾ", 
		true, 
		m_eRenderMode == fsp::RENDERMODE_SCALE_FILL, 
		false);
	CMenuElementUI* pDbcjElement = ConstructMenuItem(L"video_disp_dbcj", 
		L"��Ƶ�ȱȲü���ʾ", 
		true, 
		m_eRenderMode == fsp::RENDERMODE_CROP_FILL, 
		false);
	CMenuElementUI* pDbwzElement = ConstructMenuItem(L"video_disp_dbwz", 
		L"��Ƶ�ȱ�������ʾ", 
		true, 
		m_eRenderMode == fsp::RENDERMODE_FIT_CENTER, 
		false);

	((CMenuUI*)pMenu->m_pm.GetRoot())->Add(pSfppElement);
	((CMenuUI*)pMenu->m_pm.GetRoot())->Add(pDbcjElement);
	((CMenuUI*)pMenu->m_pm.GetRoot())->Add(pDbwzElement);

	// ������л�����ͷ�˵���
	if (m_bHasVideo && m_bIsLocal)
	{
		CMenuElementUI* pSwitchElement = ConstructMenuItem(L"cam_switch", L"�л�����ͷ", false, false, true);

		fsp::IFspEngine* pEngin = CSdkManager::GetInstance().GetFspEngin();
		fsp::Vector<fsp::VideoDeviceInfo> info = pEngin->GetDeviceManager()->GetCameraDevices();
		DWORD dwCamIndex = 0;
		for (auto iter : info)
		{
			WCHAR szOptionName[16];
			_snwprintf(szOptionName, 16, L"menu_cam_%d", dwCamIndex);

			WCHAR szCamName[32];
			demo::ConvertUtf8ToUnicode(iter.device_name.c_str(), szCamName, 32);

			CMenuElementUI* pElement = ConstructMenuItem(szOptionName, 
				szCamName, 
				true, 
				dwCamIndex == m_dwCamIndex, 
				false);

			pSwitchElement->Add(pElement);

			dwCamIndex++;
		}
		((CMenuUI*)pMenu->m_pm.GetRoot())->Add(pSwitchElement);
	}

	pMenu->ResizeMenu();
}

/*------------------------------------------------------------------------------
 * ��  ����������Ƶ��ʾģʽ����Ļ/�ȱȲü�/�ȱ�����
 * ��  ����[in] mode ��ʾģʽ
 * ����ֵ����
------------------------------------------------------------------------------*/
void CDisplayVideoWnd::SetVideoRenderMode(fsp::RenderMode mode)
{
	if (m_bHasVideo && m_eRenderMode != mode)
		CSdkManager::GetInstance().GetFspEngin()->SetRenderMode(GetVideoWnd(), mode);

	m_eRenderMode = mode;
}

/*------------------------------------------------------------------------------
 * ��  �����л�����ͷ����
 * ��  ����[in] dwCamIndex �µ�����ͷ����
 * ����ֵ����
------------------------------------------------------------------------------*/
void CDisplayVideoWnd::ChangeCamIndex(DWORD dwCamIndex)
{
	if (m_bHasVideo && m_bIsLocal)
	{
		// VideoId���䣬CamIndex�仯��������ϣ����ǹ㲥������ͷ�Ѿ��ı�
		fsp::IFspEngine* pEngin = CSdkManager::GetInstance().GetFspEngin();
		if (fsp::ERR_OK != pEngin->StartPublishVideo(m_strVideoId, dwCamIndex))
			return;

		// ���Ƴ��ɵ�preview����ͷ
		pEngin->RemoveVideoPreview(m_dwCamIndex, GetVideoWnd());

		// ����preview�µ�����ͷ
		pEngin->AddVideoPreview(dwCamIndex, GetVideoWnd());
		pEngin->SetRenderMode(GetVideoWnd(), m_eRenderMode);

		// ֪ͨ������㲥������ͷ�仯��
		CSdkManager::GetInstance().GetFrameWnd()->SendMessageW(
			DUILIB_MSG_BROADCAST_CAM_CHANGED, m_dwCamIndex, dwCamIndex);

		m_dwCamIndex = dwCamIndex;
	}
}

/*------------------------------------------------------------------------------
 * ��  ����DUILIB��֪ͨ��Ϣ����
 * ��  ����[in] msg ֪ͨ��Ϣ
 * ����ֵ����
------------------------------------------------------------------------------*/
void CDisplayVideoWnd::Notify(TNotifyUI& msg)
{
	if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		if (msg.pSender->GetName() == L"btn_menu")
		{
			CMenuWnd* pMenu = new CMenuWnd(m_pInfoWnd->GetHWND());

			CDuiPoint point = msg.ptMouse;
			ClientToScreen(m_pInfoWnd->GetHWND(), &point);

			pMenu->Init(NULL,
				this,
				L"video_menu.xml",
				_T("xml"),
				point,
				eMenuAlignment_Right | eMenuAlignment_Bottom);

			ConstructMenu(pMenu);
		}
	}
	else if (msg.sType == DUI_MSGTYPE_SELECTCHANGED)
	{
		if (wcsncmp(msg.pSender->GetName(), L"menu_cam_", 9) == 0)
		{
			DWORD dwCamIndex = _wtoi(msg.pSender->GetName().GetData() + 9);
			ChangeCamIndex(dwCamIndex);
		}
		else if (msg.pSender->GetName() == L"video_disp_sfpp")
		{
			SetVideoRenderMode(RENDERMODE_SCALE_FILL);
		}
		else if (msg.pSender->GetName() == L"video_disp_dbcj")
		{
			SetVideoRenderMode(RENDERMODE_CROP_FILL);
		}
		else if (msg.pSender->GetName() == L"video_disp_dbwz")
		{
			SetVideoRenderMode(RENDERMODE_FIT_CENTER);
		}
	}
}

/*------------------------------------------------------------------------------
 * ��  ������ʱ������
 * ��  ������
 * ����ֵ����
------------------------------------------------------------------------------*/
void CDisplayVideoWnd::OnTimer()
{
	fsp::IFspEngine* pEngin = CSdkManager::GetInstance().GetFspEngin();

	if (m_bHasAudio)
	{
		int nAudioEnergy = 0;
		if (m_bIsLocal)
			nAudioEnergy = pEngin->GetAudioEngine()->GetAudioParam(AUDIOPARAM_MICROPHONE_ENERGY);
		else
			nAudioEnergy = pEngin->GetRemoteAudioEnergy(m_strUserId);

		OnMicEnergyChange(nAudioEnergy);
	}

	if (m_bHasVideo)
	{
		fsp::VideoStatsInfo statsInfo;
		if (fsp::ERR_OK == pEngin->GetVideoStats(m_strUserId, m_strVideoId, &statsInfo))
		{
			WCHAR szVideoStats[32];
			_snwprintf(szVideoStats, 32,
				L"%dK %dF %d*%d",
				statsInfo.bitrate / 1024,
				statsInfo.framerate,
				statsInfo.width,
				statsInfo.height);
			OnVideoStatsChange(szVideoStats);
		}
	}
}

/*------------------------------------------------------------------------------
 * ��  ���������Ƿ���У���û����ƵҲû����Ƶ
 * ��  ������
 * ����ֵ����/��
------------------------------------------------------------------------------*/
bool CDisplayVideoWnd::IsIdle()
{
	return !m_bHasAudio && !m_bHasVideo;
}

/*------------------------------------------------------------------------------
 * ��  ���������Ƿ�������ʾԶ���û���ĳһ·��Ƶ
 * ��  ����[in] strUserId	�û�ID
 *         [in] strVideoId	��ƵID
 * ����ֵ����/��
------------------------------------------------------------------------------*/
bool CDisplayVideoWnd::IsHoldRemoteVideo(const fsp::String& strUserId, const fsp::String& strVideoId)
{
	return m_bHasVideo 
		&& (strcmp(m_strUserId.c_str(), strUserId.c_str()) == 0) 
		&& (strcmp(m_strVideoId.c_str(), strVideoId.c_str()) == 0);
}

/*------------------------------------------------------------------------------
 * ��  ���������Ƿ�������ʾԶ���û�����Ƶ
 * ��  ����[in] strUserId	�û�ID
 * ����ֵ����/��
------------------------------------------------------------------------------*/
bool CDisplayVideoWnd::IsHoldRemoteVideo(const fsp::String& strUserId)
{
	return m_bHasVideo && (strcmp(m_strUserId.c_str(), strUserId.c_str()) == 0);
}

/*------------------------------------------------------------------------------
 * ��  ���������Ƿ�������ʾԶ���û�����Ƶ
 * ��  ����[in] strUserId	�û�ID
 * ����ֵ����/��
------------------------------------------------------------------------------*/
bool CDisplayVideoWnd::IsHoldRemoteAudio(const fsp::String& strUserId)
{
	return m_bHasAudio && (strcmp(m_strUserId.c_str(), strUserId.c_str()) == 0);
}

/*------------------------------------------------------------------------------
 * ��  ���������Ƿ�������ʾ����ĳһ·��Ƶ
 * ��  ����[in] dwCamIndex ����ͷ����
 * ����ֵ����/��
------------------------------------------------------------------------------*/
bool CDisplayVideoWnd::IsHoldLocalVideo(DWORD dwCamIndex)
{
	return m_bHasVideo && m_bIsLocal && m_dwCamIndex == dwCamIndex;
}

/*------------------------------------------------------------------------------
 * ��  ���������Ƿ�������ʾ������Ƶ
 * ��  ������
 * ����ֵ����/��
------------------------------------------------------------------------------*/
bool CDisplayVideoWnd::IsHoldLocalVideo()
{
	return m_bHasVideo && m_bIsLocal;
}

/*------------------------------------------------------------------------------
 * ��  ���������Ƿ�������ʾ����ĳһ·��Ƶ
 * ��  ����[in] dwMicIndex ��˷�����
 * ����ֵ����/��
------------------------------------------------------------------------------*/
bool CDisplayVideoWnd::IsHoldLocalAudio(DWORD dwMicIndex)
{
	return m_bHasAudio && m_bIsLocal && m_dwMicIndex == dwMicIndex;
}

/*------------------------------------------------------------------------------
 * ��  ���������Ƿ�������ʾ������Ƶ
 * ��  ������
 * ����ֵ����/��
------------------------------------------------------------------------------*/
bool CDisplayVideoWnd::IsHoldLocalAudio()
{
	return m_bHasAudio && m_bIsLocal;
}

/*------------------------------------------------------------------------------
 * ��  ����ˢ���û�ͼ����û�����
 * ��  ������
 * ����ֵ����
------------------------------------------------------------------------------*/
void CDisplayVideoWnd::RefreshUserName()
{
	CLabelUI* pUserLabel = (CLabelUI*)m_pInfoWnd->GetPaintManager().FindControl(L"label_user");
	CLabelUI* pIconLabel = (CLabelUI*)m_pInfoWnd->GetPaintManager().FindControl(L"label_user_icon");

	if (IsIdle())
	{
		pIconLabel->SetBkImage(L"img\\video\\video_user_disable.png");
		pUserLabel->SetText(L"Nobody");
	}
	else
	{
		pIconLabel->SetBkImage(L"img\\video\\video_user_enable.png");
		if (m_bIsLocal)
		{
			pUserLabel->SetText(L"Local");
		}
		else
		{
			WCHAR szUserName[128];
			demo::ConvertUtf8ToUnicode(m_strUserId.c_str(), szUserName, 128);
			pUserLabel->SetText(szUserName);
		}
	}

	pUserLabel->Invalidate();
	pIconLabel->Invalidate();
}

/*------------------------------------------------------------------------------
 * ��  �����㲥����ͷ����
 * ��  ����[in] dwCamIndex ����ͷ����
 * ����ֵ���ɹ�/ʧ��
------------------------------------------------------------------------------*/
bool CDisplayVideoWnd::StartPublishCam(DWORD dwCamIndex)
{
	if (m_bHasVideo)
		return false;

	fsp::IFspEngine* pEngin = CSdkManager::GetInstance().GetFspEngin();

	// �㲥������Ƶ
	char szVideoId[32];
	sprintf(szVideoId, "LocalCam_%d", dwCamIndex);
	if (fsp::ERR_OK != pEngin->StartPublishVideo(szVideoId, dwCamIndex))
	{
		PostMessageW(CSdkManager::GetInstance().GetFrameWnd()->GetHWND(), 
			DUILIB_MSG_BROADCAST_CAM_FAILED, 0, 0);
		return false;
	}

	// ��ʾ������Ƶ
	pEngin->AddVideoPreview(dwCamIndex, GetVideoWnd());
	pEngin->SetRenderMode(GetVideoWnd(), m_eRenderMode);
	
	char szUserId[128];
	demo::ConvertUnicodeToUtf8(CSdkManager::GetInstance().GetLoginUser(), szUserId, 128);

	// ����״̬��Ϣ
	m_dwCamIndex	= dwCamIndex;
	m_bHasVideo		= true;
	m_bIsLocal		= true;
	m_strVideoId	= szVideoId;
	m_strUserId		= szUserId;

	SetVideoParam();
	RefreshUserName();

	return true;
}

/*------------------------------------------------------------------------------
 * ��  ����ֹͣ�㲥����ͷ����
 * ��  ����[in] dwCamIndex ����ͷ����
 * ����ֵ���ɹ�/ʧ��
------------------------------------------------------------------------------*/
bool CDisplayVideoWnd::StopPublishCam(DWORD dwCamIndex)
{
	bool bResult = false;

	fsp::IFspEngine* pEngin = CSdkManager::GetInstance().GetFspEngin();

	if (m_bHasVideo && m_bIsLocal && m_dwCamIndex == dwCamIndex)
	{
		pEngin->StopPublishVideo(m_strVideoId);

		// ȡ���㲥��Ƶ
		pEngin->RemoveVideoPreview(dwCamIndex, GetVideoWnd());

		// ������Ƶ����
		::InvalidateRect(GetVideoWnd(), NULL, TRUE);

		m_bHasVideo = false;

		bResult = true;
	}

	RefreshUserName();

	return bResult;
}

/*------------------------------------------------------------------------------
 * ��  �������Զ��ĳ�û���ĳһ·��Ƶ
 * ��  ����[in] strUserId	�û�ID
 *         [in] strVideoId	��ƵID
 * ����ֵ����
------------------------------------------------------------------------------*/
void CDisplayVideoWnd::AddRemoteVideo(const fsp::String& strUserId, const fsp::String& strVideoId)
{
	fsp::IFspEngine* pEngin = CSdkManager::GetInstance().GetFspEngin();

	pEngin->SetRemoteVideoRender(strUserId, strVideoId, GetVideoWnd());
	pEngin->SetRenderMode(GetVideoWnd(), m_eRenderMode);

	m_dwCamIndex = 0;
	m_bHasVideo = true;
	m_bIsLocal = false;
	m_strVideoId = strVideoId;
	m_strUserId = strUserId;

	RefreshUserName();
}

/*------------------------------------------------------------------------------
 * ��  ����ɾ��Զ��ĳ�û���ĳһ·��Ƶ
 * ��  ����[in] strUserId	�û�ID
 *         [in] strVideoId	��ƵID
 * ����ֵ����
------------------------------------------------------------------------------*/
void CDisplayVideoWnd::DelRemoteVideo(const fsp::String& strUserId, const fsp::String& strVideoId)
{
	if (m_bHasVideo && !m_bIsLocal
		&& strcmp(m_strVideoId.c_str(), strVideoId.c_str()) == 0
		&& strcmp(m_strUserId.c_str(), strUserId.c_str()) == 0)
	{
		// ˢ������Ƶ���ڣ�������ܻ��в�����Ƶͼ��
		::InvalidateRect(GetVideoWnd(), NULL, TRUE);

		m_bHasVideo = false;
	}

	RefreshUserName();
}

/*------------------------------------------------------------------------------
 * ��  �������Զ��ĳ�û�����Ƶ
 * ��  ����[in] strUserId	�û�ID
 * ����ֵ����
------------------------------------------------------------------------------*/
void CDisplayVideoWnd::AddRemoteAudio(const fsp::String& strUserId)
{
	if (m_bHasVideo && strcmp(m_strUserId.c_str(), strUserId.c_str()) != 0)
		return;

	m_bIsLocal = false;
	m_bHasAudio = true;
	m_strUserId = strUserId;

	OpenMic();
	RefreshUserName();
}

/*------------------------------------------------------------------------------
 * ��  ����ɾ��Զ��ĳ�û�����Ƶ
 * ��  ����[in] strUserId	�û�ID
 * ����ֵ����
------------------------------------------------------------------------------*/
void CDisplayVideoWnd::DelRemoteAudio(const fsp::String& strUserId)
{
	if (m_bHasAudio && strcmp(m_strUserId.c_str(), strUserId.c_str()) == 0)
		m_bHasAudio = false;

	CloseMic();
	RefreshUserName();
}

/*------------------------------------------------------------------------------
 * ��  �����㲥������˷�
 * ��  ����[in] dwMicIndex ��˷�����
 * ����ֵ����
------------------------------------------------------------------------------*/
void CDisplayVideoWnd::StartPublishMic(DWORD dwMicIndex)
{
	if (m_bHasAudio)
		return;

	fsp::IFspEngine* pEngin = CSdkManager::GetInstance().GetFspEngin();
	if (fsp::ErrCode::ERR_OK != pEngin->GetAudioEngine()->SetMicrophoneDevice(dwMicIndex))
		return;
	
	if (fsp::ErrCode::ERR_OK != pEngin->StartPublishAudio())
		return;
	
	char szUserId[128];
	demo::ConvertUnicodeToUtf8(CSdkManager::GetInstance().GetLoginUser(), szUserId, 128);

	m_bHasAudio		= true;
	m_dwMicIndex	= dwMicIndex;
	m_bIsLocal		= true;
	m_strUserId		= szUserId;

	OpenMic();
	RefreshUserName();
}

/*------------------------------------------------------------------------------
 * ��  ����ȡ���㲥������˷�
 * ��  ����[in] dwMicIndex ��˷�����
 * ����ֵ���ɹ�/ʧ��
------------------------------------------------------------------------------*/
bool CDisplayVideoWnd::StopPublishMic(DWORD dwMicIndex)
{
	if (!m_bHasAudio || m_dwMicIndex != dwMicIndex)
		return false;

	CSdkManager::GetInstance().GetFspEngin()->StopPublishAudio();

	m_bHasAudio = false;

	CloseMic();
	RefreshUserName();

	return true;
}

void CDisplayVideoWnd::SetVideoParam()
{
	CSdkManager& mgr = CSdkManager::GetInstance();

	fsp::VideoProfile profile;
	profile.framerate = mgr.GetFrameRate();
	profile.width = VideoResolutions[mgr.GetResolution()].dwWidth;
	profile.height = VideoResolutions[mgr.GetResolution()].dwHeight;

	mgr.GetFspEngin()->SetVideoProfile(m_strVideoId, profile);
}

/*------------------------------------------------------------------------------
 * ��  ������Ƶ�����ı䣬Ҫ��������
 * ��  ������
 * ����ֵ����
------------------------------------------------------------------------------*/
void CDisplayVideoWnd::OnVideoParamChanged()
{
	if (m_bHasVideo && m_bIsLocal)
		SetVideoParam();
}