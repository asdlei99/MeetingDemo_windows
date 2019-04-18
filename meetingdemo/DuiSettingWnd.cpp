/*##############################################################################
 * �ļ���DuiSettingWnd.cpp
 * ���������ô��ڵĽ��漰�߼�ʵ��
 * ���ߣ�Teck
 * ʱ�䣺2018��5��24��
 * ��Ȩ��Copyright(C) 2018 Fsmeeting.com. All rights reserved.
 ##############################################################################*/
#include "stdafx.h"
#include "DuiSettingWnd.h"
#include "SdkManager.h"
#include "fsp_engine.h"
#include "util.h"
#include "define.h"

using namespace fsp;

namespace
{
void RectClientToScreen(HWND hWnd, RECT& rect)
{
	CPoint ptLeftTop;
	ptLeftTop.x = rect.left;
	ptLeftTop.y = rect.top;

	ClientToScreen(hWnd, &ptLeftTop);

	CPoint ptRightBottom;
	ptRightBottom.x = rect.right;
	ptRightBottom.y = rect.bottom;

	ClientToScreen(hWnd, &ptRightBottom);

	rect.left = ptLeftTop.x;
	rect.top = ptLeftTop.y;
	rect.right = ptRightBottom.x;
	rect.bottom = ptRightBottom.y;
}
}

// ��Ϣӳ��
DUI_BEGIN_MESSAGE_MAP(CDuiSettingWnd, WindowImplBase)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_VALUECHANGED, OnValueChanged)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMSELECT, OnComboItemSelected)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_SELECTCHANGED, OnSelectedChange)
DUI_END_MESSAGE_MAP()

/*------------------------------------------------------------------------------
 * ��  �������캯��
 * ��  ������
 * ����ֵ����
------------------------------------------------------------------------------*/
CDuiSettingWnd::CDuiSettingWnd()
	: m_dwCurRenderCamId(INVALID_CAM_INDEX)
	, m_pVideoWnd(nullptr)
{
}

/*------------------------------------------------------------------------------
 * ��  ������������
 * ��  ������
 * ����ֵ����
------------------------------------------------------------------------------*/
CDuiSettingWnd::~CDuiSettingWnd()
{
	delete m_pVideoWnd;
}

/*------------------------------------------------------------------------------
 * ��  �������⺯����DUI���û�ȡ����xml�����ļ�Ŀ¼
 * ��  ������
 * ����ֵ��xml�����ļ�Ŀ¼
 ------------------------------------------------------------------------------*/
CDuiString CDuiSettingWnd::GetSkinFolder()
{
	return CDuiString(L"skin");
}

/*------------------------------------------------------------------------------
 * ��  �������⺯����DUI���û�ȡ����xml�����ļ�����
 * ��  ������
 * ����ֵ��xml�����ļ�����
 ------------------------------------------------------------------------------*/
CDuiString CDuiSettingWnd::GetSkinFile()
{
	return CDuiString(L"setting.xml");
}

/*------------------------------------------------------------------------------
 * ��  �������⺯����DUI���û�ȡ����������
 * ��  ������
 * ����ֵ������������
 ------------------------------------------------------------------------------*/
LPCTSTR CDuiSettingWnd::GetWindowClassName(void) const
{
	return L"CDuiSettingWnd";
}

void CDuiSettingWnd::Notify(TNotifyUI& msg)
{
	if (msg.sType == DUI_MSGTYPE_WINDOWINIT) {
		MoveVideoWnd();
	}
	WindowImplBase::Notify(msg);
}

/*------------------------------------------------------------------------------
 * ��  ����DUILIB��������Ϣ����
 * ��  ����[in] msg ֪ͨ��Ϣ
 * ����ֵ����
 ------------------------------------------------------------------------------*/
void CDuiSettingWnd::OnValueChanged(TNotifyUI& msg)
{
	if (msg.pSender->GetName() == L"slider_mic")
	{
		CSliderUI* pSliderMic = (CSliderUI*)m_PaintManager.FindControl(L"slider_mic");
		CSdkManager::GetInstance().SetMicVol(pSliderMic->GetValue());
	}
	else if (msg.pSender->GetName() == L"slider_aud")
	{
		CSliderUI* pSliderAud = (CSliderUI*)m_PaintManager.FindControl(L"slider_aud");
		CSdkManager::GetInstance().SetAudVol(pSliderAud->GetValue());
	}
	else if (msg.pSender->GetName() == L"slider_frame_rate")
	{
		DWORD dwFrameRate = ((CSliderUI*)msg.pSender)->GetValue();

		WCHAR szFrameRate[16]; // ���»��������֡����ʾ
		CLabelUI* pLabelFrameRate = (CLabelUI*)m_PaintManager.FindControl(L"label_frame_rate");
		_snwprintf_s(szFrameRate, 16, L"%d ֡/��", dwFrameRate);
		pLabelFrameRate->SetText(szFrameRate);

		CSdkManager::GetInstance().SetFrameRate(dwFrameRate);
	}
}

/*------------------------------------------------------------------------------
 * ��  ����DUILIB�����Ϣ����
 * ��  ����[in] msg ֪ͨ��Ϣ
 * ����ֵ����
 ------------------------------------------------------------------------------*/
void CDuiSettingWnd::OnClick(TNotifyUI& msg)
{
	WindowImplBase::OnClick(msg);

	if (msg.pSender->GetName() == L"setting_ok") {
		ScreenShareConfig screenShareConfig;
		COptionUI* pOptionShareSpeed = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("sharemodeopt_speed")));
		COptionUI* pOptionShareQuality = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("sharemodeopt_quality")));
		CRichEditUI* pEditLeft = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("share_left_edit")));
		CRichEditUI* pEditTop = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("share_top_edit")));
		CRichEditUI* pEditRight = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("share_right_edit")));
		CRichEditUI* pEditBottom = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("share_bottom_edit")));
		if (pOptionShareSpeed->IsSelected()) {
			screenShareConfig.qualityBias = fsp::SCREEN_SHARE_BIAS_SPEED;
		}
		else {
			screenShareConfig.qualityBias = fsp::SCREEN_SHARE_BIAS_QUALITY;
		}

		screenShareConfig.left = _tstoi(pEditLeft->GetText());
		screenShareConfig.top = _tstoi(pEditTop->GetText());
		screenShareConfig.right = _tstoi(pEditRight->GetText());
		screenShareConfig.bottom = _tstoi(pEditBottom->GetText());

		CSdkManager::GetInstance().SetScreenShareConfig(screenShareConfig);

		Close();
	}
}

/*------------------------------------------------------------------------------
 * ��  �����麯�����Զ�����Ϣ����
 * ��  ������
 * ����ֵ��LRESULT
 ------------------------------------------------------------------------------*/
LRESULT CDuiSettingWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (uMsg == WM_TIMER)
	{
		if (CSdkManager::GetInstance().GetOpenMic() != INVALID_MIC_INDEX)
		{
			// ��ȡ��˷�����ֵ
			fsp::IFspEngine* pEngin = CSdkManager::GetInstance().GetFspEngin();
			DWORD dwEnergy = pEngin->GetAudioEngine()->GetAudioParam(AUDIOPARAM_MICROPHONE_ENERGY);

			// ��ʾ����ֵ
			WCHAR szImgName[32];
			_snwprintf_s(szImgName, 32, L"img\\setting\\menu_wave%d.png", dwEnergy / 10);
			CLabelUI* pLabel = (CLabelUI*)m_PaintManager.FindControl(L"mic_energy");
			pLabel->SetBkImage(szImgName);
		}

		if (CSdkManager::GetInstance().GetOpenAud() != INVALID_AUD_INDEX)
		{
			// ��ȡ����������ֵ
			fsp::IFspEngine* pEngin = CSdkManager::GetInstance().GetFspEngin();
			DWORD dwEnergy = pEngin->GetAudioEngine()->GetAudioParam(AUDIOPARAM_SPEAKER_ENERGY);

			// ��ʾ����ֵ
			WCHAR szImgName[32];
			_snwprintf_s(szImgName, 32, L"img\\setting\\menu_wave%d.png", dwEnergy / 10);
			CLabelUI* pLabel = (CLabelUI*)m_PaintManager.FindControl(L"aud_energy");
			pLabel->SetBkImage(szImgName);
		}
	}
	else if (uMsg == WM_MOVE)
	{
		if (m_pTabLayout && m_pTabLayout->GetCurSel() == 0) {
			MoveVideoWnd();
		}
	}
	else if (uMsg == WM_CLOSE) {
		if (m_pVideoWnd) {
			fsp::IFspEngine* pEngin = CSdkManager::GetInstance().GetFspEngin();
			if (m_dwCurRenderCamId != INVALID_CAM_INDEX)
				pEngin->RemoveVideoPreview(m_dwCurRenderCamId, m_pVideoWnd->GetHWND());
		}
	}
	return 0;
}

/*------------------------------------------------------------------------------
 * ��  ������˷����������б���Ϣ����
 * ��  ����[in] msg ֪ͨ��Ϣ
 * ����ֵ����
 ------------------------------------------------------------------------------*/
void CDuiSettingWnd::OnSelectMic(TNotifyUI& msg)
{
	CComboUI* pComboMic = (CComboUI*)msg.pSender;
	DWORD dwMicIndex = pComboMic->GetCurSel();

	CSdkManager::GetInstance().SetOpenMic(dwMicIndex);

	CSliderUI* pMicSlider = (CSliderUI*)m_PaintManager.FindControl(L"slider_mic");
	if (dwMicIndex == INVALID_MIC_INDEX)
	{
		pMicSlider->SetAttribute(L"bkimage", L"img\\setting\\menu_slider_bg.png");
		pMicSlider->SetAttribute(L"thumbimage", L"img\\setting\\menu_sliderbtn_disable.png");
		pMicSlider->SetValue(0);
	}
	else
	{
		pMicSlider->SetAttribute(L"bkimage", L"img\\setting\\menu_slider_stat.png");
		pMicSlider->SetAttribute(L"thumbimage", L"img\\setting\\menu_sliderbtn.png");
		pMicSlider->SetAttribute(L"thumbhotimage", L"img\\setting\\menu_sliderbtn_hot.png");
		pMicSlider->SetAttribute(L"thumbpushedimage", L"img\\setting\\menu_sliderbtn_press.png");
		pMicSlider->SetValue(CSdkManager::GetInstance().GetMicVol());
	}
}

/*------------------------------------------------------------------------------
 * ��  �������������������б���Ϣ����
 * ��  ����[in] msg ֪ͨ��Ϣ
 * ����ֵ����
 ------------------------------------------------------------------------------*/
void CDuiSettingWnd::OnSelectAud(TNotifyUI& msg)
{
	CComboUI* pComboAud = (CComboUI*)msg.pSender;
	DWORD dwAudIndex = pComboAud->GetCurSel();

	CSdkManager::GetInstance().SetOpenAud(dwAudIndex);

	CSliderUI* pAudSlider = (CSliderUI*)m_PaintManager.FindControl(L"slider_aud");
	if (dwAudIndex == INVALID_AUD_INDEX)
	{
		pAudSlider->SetAttribute(L"bkimage", L"img\\setting\\menu_slider_bg.png");
		pAudSlider->SetAttribute(L"thumbimage", L"img\\setting\\menu_sliderbtn_disable.png");
		pAudSlider->SetValue(0);
	}
	else
	{
		pAudSlider->SetAttribute(L"bkimage", L"img\\setting\\menu_slider_stat.png");
		pAudSlider->SetAttribute(L"thumbimage", L"img\\setting\\menu_sliderbtn.png");
		pAudSlider->SetAttribute(L"thumbhotimage", L"img\\setting\\menu_sliderbtn_hot.png");
		pAudSlider->SetAttribute(L"thumbpushedimage", L"img\\setting\\menu_sliderbtn_press.png");
		pAudSlider->SetValue(CSdkManager::GetInstance().GetAudVol());
	}
}

/*------------------------------------------------------------------------------
 * ��  ��������ͷԤ�������б���Ϣ����
 * ��  ����[in] msg ֪ͨ��Ϣ
 * ����ֵ����
 ------------------------------------------------------------------------------*/
void CDuiSettingWnd::OnSelectCam(TNotifyUI& msg)
{
	if (!m_pVideoWnd) {
		return;
	}

	MoveVideoWnd();

	fsp::IFspEngine* pEngin = CSdkManager::GetInstance().GetFspEngin();
	if (m_dwCurRenderCamId != INVALID_CAM_INDEX)
		pEngin->RemoveVideoPreview(m_dwCurRenderCamId, m_pVideoWnd->GetHWND());

	fsp::Vector<VideoDeviceInfo> vecCam = pEngin->GetDeviceManager()->GetCameraDevices();
	
	CComboUI* pComboCam = (CComboUI*)msg.pSender;
	pEngin->AddVideoPreview(vecCam[pComboCam->GetCurSel()].camera_id, m_pVideoWnd->GetHWND(), fsp::RENDERMODE_FIT_CENTER);
	m_dwCurRenderCamId = vecCam[pComboCam->GetCurSel()].camera_id;
}

/*------------------------------------------------------------------------------
 * ��  �����ֱ��������б���Ϣ����
 * ��  ����[in] msg ֪ͨ��Ϣ
 * ����ֵ����
 ------------------------------------------------------------------------------*/
void CDuiSettingWnd::OnSelectRes(TNotifyUI& msg)
{
	CComboUI* pComboRes = (CComboUI*)msg.pSender;
	DWORD dwResIndex = pComboRes->GetCurSel();

	CSdkManager::GetInstance().SetResolution(dwResIndex);
}

/*------------------------------------------------------------------------------
 * ��  ���������б���Ϣ����
 * ��  ����[in] msg ֪ͨ��Ϣ
 * ����ֵ����
 ------------------------------------------------------------------------------*/
void CDuiSettingWnd::OnComboItemSelected(TNotifyUI& msg)
{
	if (msg.pSender->GetName() == L"combo_cam")
	{
		OnSelectCam(msg);
	}
	else if (msg.pSender->GetName() == L"combo_mic")
	{
		OnSelectMic(msg);
	}
	else if (msg.pSender->GetName() == L"combo_aud")
	{
		OnSelectAud(msg);
	}
	else if (msg.pSender->GetName() == L"combo_resolution")
	{
		OnSelectRes(msg);
	}
}

/*------------------------------------------------------------------------------
 * ��  ����DUILIB��ܻص������ڳ�ʼ������
 * ��  ������
 * ����ֵ����
 ------------------------------------------------------------------------------*/
void CDuiSettingWnd::InitWindow()
{
	// ��ʼ���ֱ��������б�
	CComboUI* pComboResolution = (CComboUI*)m_PaintManager.FindControl(L"combo_resolution");
	for (int i = 0; i < sizeof(VideoResolutions) / sizeof(VideoResolution); i++)
	{
		WCHAR szResolution[128];
		_snwprintf_s(szResolution, 128, L"%d * %d", VideoResolutions[i].dwWidth, VideoResolutions[i].dwHeight);

		CListLabelElementUI* pLabel = new CListLabelElementUI();
		pLabel->SetAttribute(L"text", szResolution);
		pComboResolution->Add(pLabel);
	}
	pComboResolution->SelectItem(CSdkManager::GetInstance().GetResolution());

	// ��ʼ��֡��
	CSliderUI* pComboFrameRate = (CSliderUI*)m_PaintManager.FindControl(L"slider_frame_rate");
	pComboFrameRate->SetValue(CSdkManager::GetInstance().GetFrameRate());

	WCHAR szFrameRate[32];
	_snwprintf_s(szFrameRate, 32, L"%d ֡/��", CSdkManager::GetInstance().GetFrameRate());
	CLabelUI* pLabelFrameRate = (CLabelUI*)m_PaintManager.FindControl(L"label_frame_rate");
	pLabelFrameRate->SetText(szFrameRate);

	// �������ô��ڵ�����ʾ
	m_pVideoWnd = new CFloatWnd(L"video_preview.xml", L"PreviewVideoWnd");
	m_pVideoWnd->CreateWnd(m_hWnd, L"PreviewWnd", UI_WNDSTYLE_FRAME, false);

	// ����ˢ��������Ϣ�Ķ�ʱ��
	CComboUI* pComboMic = (CComboUI*)m_PaintManager.FindControl(L"combo_mic");
	m_PaintManager.SetTimer(pComboMic, UPDATE_SETTING_INFO_TIMER_ID, UPDATE_SETTING_INFO_INTERVAL);
	m_pTabLayout = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("setting_tab")));

	//screenshare
	ScreenShareConfig screenShareConfig = CSdkManager::GetInstance().GetScreenShareConfig();
	COptionUI* pOptionShareSpeed = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("sharemodeopt_speed")));
	COptionUI* pOptionShareQuality = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("sharemodeopt_quality")));
	CRichEditUI* pEditLeft = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("share_left_edit")));
	CRichEditUI* pEditTop = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("share_top_edit")));
	CRichEditUI* pEditRight = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("share_right_edit")));
	CRichEditUI* pEditBottom = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("share_bottom_edit")));
	if (screenShareConfig.qualityBias == fsp::SCREEN_SHARE_BIAS_SPEED) {
		pOptionShareSpeed->Selected(true);
	}
	else {
		pOptionShareQuality->Selected(true);
	}
	CDuiString strPosVal;
	strPosVal.Format(L"%d", screenShareConfig.left);
	pEditLeft->SetText(strPosVal);

	strPosVal.Format(L"%d", screenShareConfig.top);
	pEditTop->SetText(strPosVal);

	strPosVal.Format(L"%d", screenShareConfig.right);
	pEditRight->SetText(strPosVal);

	strPosVal.Format(L"%d", screenShareConfig.bottom);
	pEditBottom->SetText(strPosVal);

	UpdateDeviceList();
}

void CDuiSettingWnd::OnDeviceChanged()
{
	UpdateDeviceList();
}

/*------------------------------------------------------------------------------
 * ��  ����tab��ǩҳ�л�
 * ��  ������
 * ����ֵ����
 ------------------------------------------------------------------------------*/
void CDuiSettingWnd::OnSelectedChange(TNotifyUI& msg)
{
	CDuiString    strName = msg.pSender->GetName();
	
	//tab
	if (strName == _T("tabopt_video")) {
		m_pTabLayout->SelectItem(0);
		m_pVideoWnd->ShowWindow(true);
		MoveVideoWnd();
	}
	else if (strName == _T("tabopt_audio")) {
		m_pTabLayout->SelectItem(1);
		m_pVideoWnd->ShowWindow(false);
		UpdateAudioTab();
	}
	else if (strName == _T("tabopt_screenshare")) {
		m_pTabLayout->SelectItem(2);
		m_pVideoWnd->ShowWindow(false);
	}
}

void CDuiSettingWnd::UpdateAudioTab()
{
	CComboUI* pComboMic = (CComboUI*)m_PaintManager.FindControl(L"combo_mic");
	// ����Ѿ�����˷类�㲥����ѡ����Ӧ��˷�
	if (CSdkManager::GetInstance().GetOpenMic() != INVALID_MIC_INDEX)
		pComboMic->SelectItem(CSdkManager::GetInstance().GetOpenMic());

	CComboUI* pComboAud = (CComboUI*)m_PaintManager.FindControl(L"combo_aud");
	// ��������������ã���ѡ����Ӧ������
	if (CSdkManager::GetInstance().GetOpenAud() != INVALID_AUD_INDEX)
		pComboAud->SelectItem(CSdkManager::GetInstance().GetOpenAud());
}

void CDuiSettingWnd::MoveVideoWnd()
{
	CLabelUI* pPreviewLabel = (CLabelUI*)m_PaintManager.FindControl(L"cam_previewer");
	if (m_pVideoWnd && pPreviewLabel) {
		RECT rect = pPreviewLabel->GetClientPos();
		RectClientToScreen(m_hWnd, rect);
		m_pVideoWnd->SetWndRect(rect);
	}
}

void CDuiSettingWnd::UpdateDeviceList()
{
	fsp::IFspEngine* pEngin = CSdkManager::GetInstance().GetFspEngin();

	// ��ʼ����˷������б��
	CComboUI* pComboMic = (CComboUI*)m_PaintManager.FindControl(L"combo_mic");
	pComboMic->RemoveAll();
	fsp::Vector<AudioDeviceInfo> vecMic = pEngin->GetDeviceManager()->GetMicrophoneDevices();
	for (auto iter = vecMic.begin(); iter != vecMic.end(); ++iter)
	{
		WCHAR szMicName[128];
		demo::ConvertUtf8ToUnicode(iter->device_name.c_str(), szMicName, 128);

		CListLabelElementUI* pLabel = new CListLabelElementUI();
		pLabel->SetAttribute(L"text", szMicName);
		pComboMic->Add(pLabel);
	}
	pComboMic->SelectItem(0);
	////////////////////////////////////////////////////////////////////////////

	// ��ʼ�������������б��
	CComboUI* pComboAud = (CComboUI*)m_PaintManager.FindControl(L"combo_aud");
	pComboAud->RemoveAll();
	fsp::Vector<AudioDeviceInfo> vecAud = pEngin->GetDeviceManager()->GetSpeakerDevices();
	for (auto iter = vecAud.begin(); iter != vecAud.end(); ++iter)
	{
		WCHAR szAudName[128];
		demo::ConvertUtf8ToUnicode(iter->device_name.c_str(), szAudName, 128);

		CListLabelElementUI* pLabel = new CListLabelElementUI();
		pLabel->SetAttribute(L"text", szAudName);
		pComboAud->Add(pLabel);
	}
	pComboAud->SelectItem(0);
	////////////////////////////////////////////////////////////////////////////

	// ��ʼ������ͷ�����б��
	CComboUI* pComboCam = (CComboUI*)m_PaintManager.FindControl(L"combo_cam");
	pComboCam->RemoveAll();

	fsp::Vector<VideoDeviceInfo> vecCam = pEngin->GetDeviceManager()->GetCameraDevices();
	int nSelectedIdx = 0;
	for (int i = 0; i < vecCam.size(); i++)
	{
		WCHAR szCamName[128];
		demo::ConvertUtf8ToUnicode(vecCam[i].device_name.c_str(), szCamName, 128);

		CListLabelElementUI* pLabel = new CListLabelElementUI();
		pLabel->SetAttribute(L"text", szCamName);
		pComboCam->Add(pLabel);

		if (vecCam[i].camera_id == m_dwCurRenderCamId) {
			nSelectedIdx = i;
		}
	}

	pComboCam->SelectItem(nSelectedIdx);
}