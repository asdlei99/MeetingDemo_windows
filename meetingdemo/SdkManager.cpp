/*##############################################################################
 * �ļ���SdkManager.cpp
 * ������SDK������ʵ�֣���Ҫʵ�ֵ�¼�߼�������SDK
 * ���ߣ�Teck
 * ʱ�䣺2018��5��24��
 * ��Ȩ��Copyright(C) 2018 Fsmeeting.com. All rights reserved.
 ##############################################################################*/
#include "stdafx.h"
#include "SdkManager.h"
#include "define.h"
#include "util.h"
#include "ConfigParser.h"
#include "fsp_token.h"
#include <fstream>
#include <sstream>

/*------------------------------------------------------------------------------
 * ��  �������캯��
 * ��  ������
 * ����ֵ����
------------------------------------------------------------------------------*/
CSdkManager::CSdkManager()
	: m_pFspEngin(nullptr)
	, m_dwAudOpenIndex(INVALID_AUD_INDEX)
	, m_dwMicOpenIndex(INVALID_MIC_INDEX)
	, m_dwAudSetVoluem(50)
	, m_dwMicSetVolume(50)
	, m_dwResolutionIndex(0)	// Ĭ�Ϸֱ��ʣ�320*240
	, m_dwFrameRate(15)			// Ĭ��֡�ʣ�15֡/��
	, m_pDuiLoginWnd(nullptr)
	, m_pDuiLoginWaitWnd(nullptr)
	, m_pDuiLoginErrorWnd(nullptr)
	, m_pDuiFrameWnd(nullptr)
	, m_bRestart(false)
{
	Create(NULL, _T("CSdkManagerMsgWnd"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	ShowWindow(false);
}

/*------------------------------------------------------------------------------
 * ��  ������������
 * ��  ������
 * ����ֵ����
------------------------------------------------------------------------------*/
CSdkManager::~CSdkManager()
{
	Destroy();
}

/*------------------------------------------------------------------------------
 * ��  ������̬��������ȡ��̬ʵ��
 * ��  ������
 * ����ֵ����̬ʵ��
------------------------------------------------------------------------------*/
CSdkManager& CSdkManager::GetInstance()
{
	static CSdkManager mgr;
	return mgr;
}

/*------------------------------------------------------------------------------
 * ��  ������ʼ��
 * ��  ������
 * ����ֵ���ɹ�/ʧ��
------------------------------------------------------------------------------*/
bool CSdkManager::Init()
{
	m_pFspEngin = FspGetEngine();
	if (m_pFspEngin == nullptr)
		return false;

	demo::ClientConfig& config = demo::CConfigParser::GetInstance().GetClientConfig();

	m_FspEnginContext.app_id = config.bUserDefine ? config.strUserAppId.c_str() : config.strAppId.c_str();
	m_FspEnginContext.log_path = "./";
	m_FspEnginContext.event_handler = this;
	m_FspEnginContext.server_addr = config.strServerAddr.c_str();
	
	fsp::ErrCode result = m_pFspEngin->Init(m_FspEnginContext);
	if (result != ERR_OK)
		return false;

	fsp::IAudioEngine *pAudioEngin = m_pFspEngin->GetAudioEngine();

	// ��ȡĬ����˷������������������
	int nMicIndex = pAudioEngin->GetMicrophoneDevice();
	if (nMicIndex != -1 && nMicIndex >= 0)
	{
		m_dwMicOpenIndex = nMicIndex;
		pAudioEngin->SetAudioParam(AUDIOPARAM_MICROPHONE_VOLUME, m_dwMicSetVolume);
	}

	int nAudIndex = pAudioEngin->GetSpeakerDevice();
	if (nAudIndex != -1 && nAudIndex >= 0)
	{
		m_dwAudOpenIndex = nAudIndex;
		pAudioEngin->SetAudioParam(AUDIOPARAM_SPEAKER_VOLUME, m_dwAudSetVoluem);
	}

	return true;
}

void CSdkManager::Destroy()
{
	Close();
	if (m_pDuiLoginErrorWnd) {
		if (IsWindow(m_pDuiLoginErrorWnd->GetHWND()))
			m_pDuiLoginErrorWnd->Close();
		delete m_pDuiLoginErrorWnd;
		m_pDuiLoginErrorWnd = nullptr;
	}

	if (m_pDuiLoginWnd) {
		if (IsWindow(m_pDuiLoginWnd->GetHWND()))
			m_pDuiLoginWnd->Close();
		delete m_pDuiLoginWnd;
		m_pDuiLoginWnd = nullptr;
	}

	if (m_pDuiFrameWnd) {
		if (IsWindow(m_pDuiFrameWnd->GetHWND()))
			m_pDuiFrameWnd->Close();
		delete m_pDuiFrameWnd;
		m_pDuiFrameWnd = nullptr;
	}

	if (m_pDuiLoginWaitWnd) {
		if (IsWindow(m_pDuiLoginWaitWnd->GetHWND()))
			m_pDuiLoginWaitWnd->Close();
		delete m_pDuiLoginWaitWnd;
		m_pDuiLoginWaitWnd = nullptr;
	}

	FspReleaseEngine();
}

/*------------------------------------------------------------------------------
 * ��  ��������
 * ��  ������
 * ����ֵ����
------------------------------------------------------------------------------*/
void CSdkManager::OpenLoginWnd()
{
	if (m_pDuiLoginErrorWnd == nullptr) {
		m_pDuiLoginErrorWnd = new CDuiLoginErrorWnd();
		m_pDuiLoginErrorWnd->Create(NULL, _T("LoginError"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);

	}

	if (m_pDuiLoginWnd == nullptr) {
		m_pDuiLoginWnd = new CDuiLoginWnd;
		m_pDuiLoginWnd->Create(NULL, _T("Login"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	}

	if (m_pDuiFrameWnd == nullptr) {
		m_pDuiFrameWnd = new CDuiFrameWnd;
		m_pDuiFrameWnd->Create(NULL, _T("Main"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	}

	if (m_pDuiLoginWaitWnd == nullptr) {
		m_pDuiLoginWaitWnd = new CDuiLoginWaitWnd();
		m_pDuiLoginWaitWnd->Create(NULL, _T("Logining"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	}

	m_pDuiLoginErrorWnd->ShowWindow(false);
	m_pDuiFrameWnd->ShowWindow(false);
	m_pDuiLoginWaitWnd->ShowWindow(false);

	m_pDuiLoginWnd->CenterWindow();
	m_pDuiLoginWnd->ShowWindow(true);
}

/*------------------------------------------------------------------------------
 * ��  ��������ʹ�õ���˷��豸
 * ��  ����[in] dwMicIndex ��˷�����
 * ����ֵ����
------------------------------------------------------------------------------*/
void CSdkManager::SetOpenMic(DWORD dwMicIndex)
{
	if (dwMicIndex != INVALID_MIC_INDEX)
	{
		m_dwMicOpenIndex = dwMicIndex;
		m_pFspEngin->GetAudioEngine()->SetMicrophoneDevice(dwMicIndex);
	}
}

/*------------------------------------------------------------------------------
 * ��  ������ȡʹ�õ���˷��豸����
 * ��  ������
 * ����ֵ����˷�����
------------------------------------------------------------------------------*/
DWORD CSdkManager::GetOpenMic()
{
	return m_dwMicOpenIndex;
}

/*------------------------------------------------------------------------------
 * ��  ��������ʹ�õ��������豸
 * ��  ����[in] dwAudIndex ����������
 * ����ֵ����
------------------------------------------------------------------------------*/
void CSdkManager::SetOpenAud(DWORD dwAudIndex)
{
	if (dwAudIndex != INVALID_AUD_INDEX)
	{
		m_dwAudOpenIndex = dwAudIndex;
		m_pFspEngin->GetAudioEngine()->SetSpeakerDevice(dwAudIndex);
	}
}

/*------------------------------------------------------------------------------
 * ��  ������ȡ�������豸����
 * ��  ������
 * ����ֵ������������
------------------------------------------------------------------------------*/
DWORD CSdkManager::GetOpenAud()
{
	return m_dwAudOpenIndex;
}

/*------------------------------------------------------------------------------
 * ��  ����������˷�����
 * ��  ����[in] dwMicVol ����
 * ����ֵ����
------------------------------------------------------------------------------*/
void CSdkManager::SetMicVol(DWORD dwMicVol)
{
	assert(dwMicVol >= 0 && dwMicVol <= 100);

	m_pFspEngin->GetAudioEngine()->SetAudioParam(AUDIOPARAM_MICROPHONE_VOLUME, dwMicVol);

	m_dwMicSetVolume = dwMicVol;
}

/*------------------------------------------------------------------------------
 * ��  ������ȡ��˷�����
 * ��  ������
 * ����ֵ������
------------------------------------------------------------------------------*/
DWORD CSdkManager::GetMicVol()
{
	return m_dwMicSetVolume;
}

/*------------------------------------------------------------------------------
 * ��  ������������������
 * ��  ����[in] dwAudVol ����
 * ����ֵ����
------------------------------------------------------------------------------*/
void CSdkManager::SetAudVol(DWORD dwAudVol)
{
	assert(dwAudVol >= 0 && dwAudVol <= 100);

	m_pFspEngin->GetAudioEngine()->SetAudioParam(AUDIOPARAM_SPEAKER_VOLUME, dwAudVol);

	m_dwAudSetVoluem = dwAudVol;
}

/*------------------------------------------------------------------------------
 * ��  ������ȡ����������
 * ��  ������
 * ����ֵ������
------------------------------------------------------------------------------*/
DWORD CSdkManager::GetAudVol()
{
	return m_dwAudSetVoluem;
}

/*------------------------------------------------------------------------------
 * ��  �������ñ���ȫ������ͷ֡��
 * ��  ����[in] dwFrameRate ֡��
 * ����ֵ����
------------------------------------------------------------------------------*/
void CSdkManager::SetFrameRate(DWORD dwFrameRate)
{
	if (dwFrameRate != m_dwFrameRate)
	{
		m_dwFrameRate = dwFrameRate;
		m_pDuiFrameWnd->PostMessageW(DUILIB_MSG_VIDEO_PARAM_CHANGED, 0, 0);
	}
}

void CSdkManager::SetScreenShareConfig(const ScreenShareConfig& config)
{
	m_screenShareConfig = config;
}

ScreenShareConfig CSdkManager::GetScreenShareConfig() const
{
	return m_screenShareConfig;
}

/*------------------------------------------------------------------------------
 * ��  ������ȡ����ȫ������ͷ֡��
 * ��  ������
 * ����ֵ��֡��
------------------------------------------------------------------------------*/
DWORD CSdkManager::GetFrameRate()
{
	return m_dwFrameRate;
}

/*------------------------------------------------------------------------------
 * ��  �������ñ���ȫ������ͷ�ֱ���
 * ��  ����[in] dwResolutionIndex �ֱ�������������ֱ��ʼ�VideoResolution
 * ����ֵ����
------------------------------------------------------------------------------*/
void CSdkManager::SetResolution(DWORD dwResolutionIndex)
{
	if (dwResolutionIndex != m_dwResolutionIndex)
	{
		m_dwResolutionIndex = dwResolutionIndex;
		m_pDuiFrameWnd->PostMessageW(DUILIB_MSG_VIDEO_PARAM_CHANGED, 0, 0);
	}
}

/*------------------------------------------------------------------------------
 * ��  ������ȡ����ȫ������ͷ�ֱ�������������ֱ��ʼ�VideoResolution
 * ��  ������
 * ����ֵ������
------------------------------------------------------------------------------*/
DWORD CSdkManager::GetResolution()
{
	return m_dwResolutionIndex;
}

/*------------------------------------------------------------------------------
 * ��  �����豸����¼�����δ����
 * ��  ����[in] device_event �¼�����
 * ����ֵ����
------------------------------------------------------------------------------*/
void CSdkManager::OnDeviceChange(DeviceEventType device_event)
{
	m_pDuiFrameWnd->PostMessageW(DUILIB_MSG_DEVICECHANGE, 0, 0);
}

/*------------------------------------------------------------------------------
 * ��  ����SDK�¼��ص�����
 * ��  ����[in] event_type	�¼�����
 *         [in] err_code	������
 * ����ֵ����
------------------------------------------------------------------------------*/
void CSdkManager::OnEvent(EventType event_type, ErrCode err_code)
{
	PostMessage(DUILIB_MSG_FSP_EVENT, event_type, err_code);
}

LRESULT CSdkManager::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (uMsg == DUILIB_MSG_FSP_EVENT) {
		EventType event_type = (EventType)wParam;
		ErrCode err_code = (ErrCode)lParam;

		OnFspEvent(event_type, err_code);

		bHandled = TRUE;
	}

	return S_OK;
}

void CSdkManager::OnFspEvent(fsp::EventType eventType, fsp::ErrCode result)
{
	if (eventType == fsp::EVENT_JOINGROUP_RESULT) {
		assert(m_pDuiLoginWnd);

		// ���ص�¼�ȴ�����
		m_pDuiLoginWaitWnd->ShowWindow(false);

		if (result == ErrCode::ERR_OK) // ��¼�ɹ�����ʾ������
		{
			m_pDuiLoginWnd->ShowWindow(false);

			m_pDuiFrameWnd->CenterWindow();
			m_pDuiFrameWnd->ShowWindow(true);
		}
		else // ��¼ʧ�ܣ���ת��ʧ��ҳ��
		{
			CDuiString strErrInfo = L"δ֪����:";
			if (result == ErrCode::ERR_TOKEN_INVALID)
				strErrInfo = L"��֤ʧ�ܣ�";
			else if (result == ErrCode::ERR_CONNECT_FAIL)
				strErrInfo = L"���ӷ�����ʧ�ܣ�";
			else if (result == ErrCode::ERR_APP_NOT_EXIST)
				strErrInfo = L"Ӧ�ò����ڣ�";
			else if (result == ErrCode::ERR_USERID_CONFLICT)
				strErrInfo = L"�û��ѵ�¼��";
			else if (result == ErrCode::ERR_NO_BALANCE)
				strErrInfo = L"�˻����㣡";
			else
			{
				WCHAR szTmp[8];
				_snwprintf_s(szTmp, 8, L"%d", result);
				strErrInfo.Append(szTmp);
			}

			ShowErrorWnd(strErrInfo);
		}
	}
	else if (eventType == fsp::EVENT_CONNECT_LOST){
		m_pDuiFrameWnd->ResetWindowStatus();
		//���ӶϿ�����ʾʧ�ܴ���
		ShowErrorWnd(L"���ӶϿ�");
	}
}

/*------------------------------------------------------------------------------
 * ��  ����SDK�ص�Զ����Ƶ����¼���Ӧ�÷���Ϣ��UI�̴߳���
 * ��  ����[in] user_id �û�ID
 *         [in] video_id ��ƵID
 *         [in] remote_video_event �¼�����
 * ����ֵ����
------------------------------------------------------------------------------*/
void CSdkManager::OnRemoteVideoEvent(const String& user_id, const String& video_id, RemoteVideoEventType remote_video_event)
{
	RemoteVideoInfo* pInfo = new RemoteVideoInfo;
	pInfo->strUserId = user_id;
	pInfo->strVideoId = video_id;

	if (REMOTE_VIDEO_EVENT_PUBLISHE_STARTED == remote_video_event)
	{
		m_pDuiFrameWnd->PostMessageW(DUILIB_MSG_ADD_REMOTE_VIDEO, (WPARAM)pInfo, 0);
	}
	else if (REMOTE_VIDEO_EVENT_PUBLISHE_STOPED == remote_video_event)
	{
		m_pDuiFrameWnd->PostMessageW(DUILIB_MSG_DEL_REMOTE_VIDEO, (WPARAM)pInfo, 0);
	}
}

/*------------------------------------------------------------------------------
 * ��  ����SDK�ص�Զ����Ƶ����¼���Ӧ�÷���Ϣ��UI�̴߳���
 * ��  ����[in] user_id �û�ID
 *         [in] remote_video_event �¼�����
 * ����ֵ����
------------------------------------------------------------------------------*/
void CSdkManager::OnRemoteAudioEvent(const String& user_id, RemoteAudioEventType remote_audio_event)
{
	if (m_pDuiFrameWnd && IsWindow(m_pDuiFrameWnd->GetHWND())) {
		RemoteAudioInfo* pInfo = new RemoteAudioInfo;
		pInfo->strUserId = user_id;

		if (REMOTE_AUDIO_EVENT_PUBLISHE_STARTED == remote_audio_event)
		{
			m_pDuiFrameWnd->PostMessageW(DUILIB_MSG_ADD_REMOTE_AUDIO, (WPARAM)pInfo, 0);
		}
		else if (REMTOE_AUDIO_EVENT_PUBLISHE_STOPED == remote_audio_event)
		{
			m_pDuiFrameWnd->PostMessageW(DUILIB_MSG_DEL_REMOTE_AUDIO, (WPARAM)pInfo, 0);
		}
	}
}

/*------------------------------------------------------------------------------
 * ��  ����Զ�̿����¼�
 * ��  ����[in] user_id  	    �û�ID
 *         [in] operation_type	����
 * ����ֵ��Token
------------------------------------------------------------------------------*/
void CSdkManager::OnRemoteControlOperationEvent(const String& user_id,
	fsp::RemoteControlOperationType operation_type)
{
	RemoteControlInfo* pInfo = new RemoteControlInfo;
	pInfo->strUserId = user_id;
	pInfo->operationType = operation_type;
	m_pDuiFrameWnd->PostMessage(DUILIB_MSG_REMOTECONTROL_EVENT, (WPARAM)pInfo, 0);
}

/*------------------------------------------------------------------------------
 * ��  ��������Token�����ǵ���ȫ�ԣ����齫Token���ɴ��뼯�ɵ�������
 * ��  ����[in] szGroupId	����ID
 *         [in] szUserId	�û�ID
 * ����ֵ��Token
------------------------------------------------------------------------------*/
std::string CSdkManager::BuildToken(char* szGroupId, char* szUserId)
{
	demo::ClientConfig& config = demo::CConfigParser::GetInstance().GetClientConfig();
	
	fsp::tools::AccessToken token(config.bUserDefine ? config.strUserAppSecret : config.strAppSecret);
	token.app_id		= config.bUserDefine ? config.strUserAppId : config.strAppId;
	token.group_id		= szGroupId;
	token.user_id		= szUserId;
	token.expire_time	= 0;

	return token.Build();
}

/*------------------------------------------------------------------------------
 * ��  ����������鴦��
 * ��  ����[in] szGroup	����ID
 *         [in] szUser	�û�ID
 * ����ֵ����
------------------------------------------------------------------------------*/
void CSdkManager::JoinGroup(LPCTSTR szGroup, LPCTSTR szUser)
{
	// ���������������ڱ�������Ҫ��ʾ����Ϣ
	m_strGroup	= szGroup;
	m_strUser	= szUser;

	char szGroupId[32];
	char szUserId[32];
	demo::ConvertUnicodeToUtf8(szGroup, szGroupId, 32);
	demo::ConvertUnicodeToUtf8(szUser, szUserId, 32);

	// ����Token
	fsp::String strToken = BuildToken(szGroupId, szUserId).c_str();

	// �������
	if (fsp::ERR_OK != m_pFspEngin->JoinGroup(strToken, szGroupId, szUserId))
		return;

	// ���ص�¼����
	m_pDuiLoginWnd->ShowWindow(false);

	// ��ʾ��¼�ȴ�����
	m_pDuiLoginWaitWnd->CenterWindow();
	m_pDuiLoginWaitWnd->ShowWindow(true);
}

/*------------------------------------------------------------------------------
 * ��  �������÷���ID
 * ��  ����[in] szGroup
 * ����ֵ����
------------------------------------------------------------------------------*/
void CSdkManager::SetLoginGroup(LPCTSTR szGroup)
{
	m_strGroup = szGroup;
}

/*------------------------------------------------------------------------------
 * ��  ������ȡ����ID
 * ��  ������
 * ����ֵ������ID
------------------------------------------------------------------------------*/
const CDuiString& CSdkManager::GetLoginGroup()
{
	return m_strGroup;
}

/*------------------------------------------------------------------------------
 * ��  ���������û�ID
 * ��  ����[in] szUser �û�ID
 * ����ֵ����
------------------------------------------------------------------------------*/
void CSdkManager::SetLoginUser(LPCTSTR szUser)
{
	m_strUser = szUser; 
}

/*------------------------------------------------------------------------------
 * ��  ������ȡ�û�ID
 * ��  ������
 * ����ֵ���û�ID
------------------------------------------------------------------------------*/
const CDuiString& CSdkManager::GetLoginUser()
{
	return m_strUser;
}

void CSdkManager::ShowErrorWnd(const CDuiString& strErrInfo)
{
	//������������
	if (m_pDuiFrameWnd) {
		m_pDuiFrameWnd->ShowWindow(false);
	}

	m_pDuiLoginErrorWnd->UpdateErrInfo(strErrInfo);
	m_pDuiLoginErrorWnd->CenterWindow();
	m_pDuiLoginErrorWnd->ShowWindow(true);
}

CDuiString CSdkManager::GetSkinFolder()
{
	return CDuiString(L"skin");
}

CDuiString CSdkManager::GetSkinFile()
{
	return CDuiString(L"emptywnd.xml");
}

LPCTSTR CSdkManager::GetWindowClassName(void) const
{
	return L"CSdkManagerMsgWnd";
}