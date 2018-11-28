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
	DUI_ON_MSGTYPE(DUI_MSGTYPE_SETFOCUS, OnSetWndFocus)
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

/*------------------------------------------------------------------------------
 * ��  �������⺯�����������ٴ���
 * ��  ������
 * ����ֵ��LRESULT
 ------------------------------------------------------------------------------*/
LRESULT CDuiSettingWnd::OnDestroy(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	//PostQuitMessage(0);
	return 0;
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
		_snwprintf(szFrameRate, 16, L"%d ֡/��", dwFrameRate);
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

	if (msg.pSender->GetName() == L"setting_ok")
		Close();
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
			_snwprintf(szImgName, 32, L"img\\setting\\menu_wave%d.png", dwEnergy / 10);
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
			_snwprintf(szImgName, 32, L"img\\setting\\menu_wave%d.png", dwEnergy / 10);
			CLabelUI* pLabel = (CLabelUI*)m_PaintManager.FindControl(L"aud_energy");
			pLabel->SetBkImage(szImgName);
		}
	}
	else if (uMsg == WM_MOVE)
	{
		CLabelUI* pPreviewLabel = (CLabelUI*)m_PaintManager.FindControl(L"cam_previewer");
		RECT rect = pPreviewLabel->GetClientPos();
		RectClientToScreen(m_hWnd, rect);

		m_pVideoWnd->SetWndRect(rect);
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
	CLabelUI* pPreviewLabel = (CLabelUI*)m_PaintManager.FindControl(L"cam_previewer");
	RECT rect = pPreviewLabel->GetClientPos();
	RectClientToScreen(m_hWnd, rect);

	m_pVideoWnd->SetWndRect(rect);

	fsp::IFspEngine* pEngin = CSdkManager::GetInstance().GetFspEngin();
	if (m_dwCurRenderCamId != INVALID_CAM_INDEX)
		pEngin->RemoveVideoPreview(m_dwCurRenderCamId, m_pVideoWnd->GetHWND());

	CComboUI* pComboCam = (CComboUI*)msg.pSender;
	pEngin->AddVideoPreview(pComboCam->GetCurSel(), m_pVideoWnd->GetHWND());
	m_dwCurRenderCamId = pComboCam->GetCurSel();
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
	fsp::IFspEngine* pEngin = CSdkManager::GetInstance().GetFspEngin();

	// ��ʼ����˷������б��
	CComboUI* pComboMic = (CComboUI*)m_PaintManager.FindControl(L"combo_mic");
	fsp::Vector<AudioDeviceInfo> vecMic = pEngin->GetDeviceManager()->GetMicrophoneDevices();
	for (auto iter = vecMic.begin(); iter != vecMic.end(); ++iter)
	{
		WCHAR szMicName[128];
		demo::ConvertUtf8ToUnicode(iter->device_name.c_str(), szMicName, 128);

		CListLabelElementUI* pLabel = new CListLabelElementUI();
		pLabel->SetAttribute(L"text", szMicName);
		pComboMic->Add(pLabel);
	}

	// ����Ѿ�����˷类�㲥����ѡ����Ӧ��˷�
	if (CSdkManager::GetInstance().GetOpenMic() != INVALID_MIC_INDEX)
		pComboMic->SelectItem(CSdkManager::GetInstance().GetOpenMic());

	////////////////////////////////////////////////////////////////////////////

	// ��ʼ�������������б��
	CComboUI* pComboAud = (CComboUI*)m_PaintManager.FindControl(L"combo_aud");
	fsp::Vector<AudioDeviceInfo> vecAud = pEngin->GetDeviceManager()->GetSpeakerDevices();
	for (auto iter = vecAud.begin(); iter != vecAud.end(); ++iter)
	{
		WCHAR szAudName[128];
		demo::ConvertUtf8ToUnicode(iter->device_name.c_str(), szAudName, 128);

		CListLabelElementUI* pLabel = new CListLabelElementUI();
		pLabel->SetAttribute(L"text", szAudName);
		pComboAud->Add(pLabel);
	}

	// ��������������ã���ѡ����Ӧ������
	if (CSdkManager::GetInstance().GetOpenAud() != INVALID_AUD_INDEX)
		pComboAud->SelectItem(CSdkManager::GetInstance().GetOpenAud());

	////////////////////////////////////////////////////////////////////////////

	// ��ʼ���ֱ��������б�
	CComboUI* pComboResolution = (CComboUI*)m_PaintManager.FindControl(L"combo_resolution");
	for (int i = 0; i < sizeof(VideoResolutions) / sizeof(VideoResolution); i++)
	{
		WCHAR szResolution[128];
		_snwprintf(szResolution, 128, L"%d * %d", VideoResolutions[i].dwWidth, VideoResolutions[i].dwHeight);

		CListLabelElementUI* pLabel = new CListLabelElementUI();
		pLabel->SetAttribute(L"text", szResolution);
		pComboResolution->Add(pLabel);
	}
	pComboResolution->SelectItem(CSdkManager::GetInstance().GetResolution());

	// ��ʼ��֡��
	CSliderUI* pComboFrameRate = (CSliderUI*)m_PaintManager.FindControl(L"slider_frame_rate");
	pComboFrameRate->SetValue(CSdkManager::GetInstance().GetFrameRate());

	WCHAR szFrameRate[32];
	_snwprintf(szFrameRate, 32, L"%d ֡/��", CSdkManager::GetInstance().GetFrameRate());
	CLabelUI* pLabelFrameRate = (CLabelUI*)m_PaintManager.FindControl(L"label_frame_rate");
	pLabelFrameRate->SetText(szFrameRate);

	// �������ô��ڵ�����ʾ
	m_pVideoWnd = new CFloatWnd(L"video_preview.xml", L"PreviewVideoWnd");
	m_pVideoWnd->CreateWnd(m_hWnd, L"PreviewWnd", UI_WNDSTYLE_FRAME, false);

	// ��ʼ������ͷ�����б��
	CComboUI* pComboCam = (CComboUI*)m_PaintManager.FindControl(L"combo_cam");
	fsp::Vector<VideoDeviceInfo> vecCam = pEngin->GetDeviceManager()->GetCameraDevices();
	for (auto iter = vecCam.begin(); iter != vecCam.end(); ++iter)
	{
		WCHAR szCamName[128];
		demo::ConvertUtf8ToUnicode(iter->device_name.c_str(), szCamName, 128);

		CListLabelElementUI* pLabel = new CListLabelElementUI();
		pLabel->SetAttribute(L"text", szCamName);
		pComboCam->Add(pLabel);
	}

	// ����ˢ��������Ϣ�Ķ�ʱ��
	m_PaintManager.SetTimer(pComboMic, UPDATE_SETTING_INFO_TIMER_ID, UPDATE_SETTING_INFO_INTERVAL);
}

/*------------------------------------------------------------------------------
 * ��  ������InitWindow�д���ѡ������ͷ���ᵼ�»�ȡRectΪ0����
 * ��  ����[in] msg ֪ͨ��Ϣ
 * ����ֵ����
------------------------------------------------------------------------------*/
void CDuiSettingWnd::OnSetWndFocus(TNotifyUI& msg)
{
	fsp::IFspEngine* pEngin = CSdkManager::GetInstance().GetFspEngin();
	fsp::Vector<VideoDeviceInfo> vecCam = pEngin->GetDeviceManager()->GetCameraDevices();

	if (vecCam.size() > 0)
	{
		((CComboUI*)m_PaintManager.FindControl(L"combo_cam"))->SelectItem(0);
	}
}