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
	, m_bMainFrameCreated(false)
	, m_pDuiLoginWnd(nullptr)
	, m_pDuiLoginWaitWnd(nullptr)
	, m_pDuiLoginErrorWnd(nullptr)
	, m_pDuiFrameWnd(nullptr)
	, m_bRestart(false)
{
	m_bSemaphore = CreateSemaphore(NULL, 0, 1, NULL);
}

/*------------------------------------------------------------------------------
 * ��  ������������
 * ��  ������
 * ����ֵ����
------------------------------------------------------------------------------*/
CSdkManager::~CSdkManager()
{
	FspReleaseEngine();
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
	m_FspEnginContext.data_handler = this;
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

/*------------------------------------------------------------------------------
 * ��  ��������
 * ��  ������
 * ����ֵ����
------------------------------------------------------------------------------*/
void CSdkManager::Start()
{
	m_pDuiLoginWnd = new CDuiLoginWnd;
	m_pDuiLoginWnd->Create(NULL, _T("CDuiLoginWnd"), UI_WNDSTYLE_DIALOG, WS_EX_WINDOWEDGE);
	m_pDuiLoginWnd->CenterWindow();
	m_pDuiLoginWnd->ShowModal();

	if (m_bRestart)
	{
		m_bRestart = false;
		Start();
	}
}

void CSdkManager::SetRestart()
{
	m_bRestart = true;
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
	//MessageBox(NULL, L"OnDeviceChange", L"Info", 0);
}

/*------------------------------------------------------------------------------
 * ��  ����SDK�¼��ص�����
 * ��  ����[in] event_type	�¼�����
 *         [in] err_code	������
 * ����ֵ����
------------------------------------------------------------------------------*/
void CSdkManager::OnEvent(EventType event_type, ErrCode err_code)
{
	if (event_type == EVENT_JOINGROUP_RESULT)
	{
		PostMessage(m_pDuiLoginWaitWnd->GetHWND(), DUILIB_MSG_LOGIN_RESULT, err_code, 0);
	}
}

/*------------------------------------------------------------------------------
 * ��  ������¼���������LoginWaitWnd�̻߳ص�����
 * ��  ����[in] result ��¼���
 * ����ֵ����
------------------------------------------------------------------------------*/
void CSdkManager::OnLoginResult(fsp::ErrCode result)
{
	assert(m_pDuiLoginWnd);
	
	// ���ص�¼�ȴ�����
	m_pDuiLoginWaitWnd->ShowWindow(false);

	if (result == ErrCode::ERR_OK) // ��¼�ɹ�����ʾ������
	{
		if (!m_bMainFrameCreated)
		{
			m_pDuiFrameWnd = new CDuiFrameWnd;
			m_pDuiFrameWnd->Create(NULL, _T("CDuiFrameWnd"), UI_WNDSTYLE_DIALOG, WS_EX_WINDOWEDGE);
			m_pDuiFrameWnd->CenterWindow();

			m_bMainFrameCreated = true;
			ReleaseSemaphore(m_bSemaphore, 1, 0);

			m_pDuiFrameWnd->ShowModal();
		}
	}
	else // ��¼ʧ�ܣ���ת��ʧ��ҳ��
	{		
		m_pDuiLoginErrorWnd = new CDuiLoginErrorWnd(result);
		m_pDuiLoginErrorWnd->Create(NULL, _T("CDuiLoginErrorWnd"), UI_WNDSTYLE_DIALOG, WS_EX_WINDOWEDGE);
		m_pDuiLoginErrorWnd->CenterWindow();
		m_pDuiLoginErrorWnd->ShowModal();
	}

	m_pDuiLoginWaitWnd->Close();
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
	if (!m_bMainFrameCreated)
		WaitForSingleObject(m_bSemaphore, INFINITE);

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
	if (!m_bMainFrameCreated)
		WaitForSingleObject(m_bSemaphore, INFINITE);

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

/*------------------------------------------------------------------------------
 * ��  ��������Token
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
	m_pDuiLoginWaitWnd = new CDuiLoginWaitWnd();
	m_pDuiLoginWaitWnd->Create(NULL, _T("CDuiLoginWaitWnd"), UI_WNDSTYLE_DIALOG, WS_EX_WINDOWEDGE);
	m_pDuiLoginWaitWnd->CenterWindow();
	m_pDuiLoginWaitWnd->ShowModal();

	m_pDuiLoginWnd->Close();
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

/*------------------------------------------------------------------------------
 * ��  ����������Ƶ���ݻص������˴�ֱ��д�ļ�
 * ��  ����[in] data ����
 *         [in] data_len ���ݳ���
 * ����ֵ����
------------------------------------------------------------------------------*/
void CSdkManager::OnLocalAudioStreamRawData(const char* data, int data_len)
{
	string strRecorFile = "LocalAudio.data";
	std::ofstream ofs(strRecorFile, std::ios_base::binary | std::ios_base::app);
	ofs.write(data, data_len);
}

/*------------------------------------------------------------------------------
 * ��  ����Զ����Ƶ���ݻص������˴�ֱ��д�ļ�
 * ��  ����[in] user_id Զ���û���ʶ
 *         [in] data ����
 *         [in] data_len ���ݳ���
 * ����ֵ����
------------------------------------------------------------------------------*/
void CSdkManager::OnRemoteAudioStreamRawData(const String& user_id, const char* data, int data_len)
{
	string strRecorFile = "RemoteAudio_" + string(user_id.c_str()) + ".data";
	std::ofstream ofs(strRecorFile, std::ios_base::binary | std::ios_base::app);
	ofs.write(data, data_len);
}

/*------------------------------------------------------------------------------
 * ��  ����������˷�/Զ���û���Ƶ���ݻص������˴�ֱ��д�ļ�
 * ��  ����[in] data ����
 *         [in] data_len ���ݳ���
 * ����ֵ����
------------------------------------------------------------------------------*/
void CSdkManager::OnMixAudioStreamRawData(const char* data, int data_len)
{
	MessageBox(NULL, L"OnAudioMixedStreamRawData", L"Callback", 0);
}

/*------------------------------------------------------------------------------
 * ��  ����������Ƶ���ݻص������˴�ֱ��д�ļ�
 * ��  ����[in] camera_id ��������ͷ����
 *         [in] header ��Ƶͷ��Ϣ�������BitmapInfoHeader��ע��
 *         [in] data ����
 *         [in] data_len ���ݳ���
 * ����ֵ����
------------------------------------------------------------------------------*/
void CSdkManager::OnLocalVideoStreamRawData(int camera_id, BitmapInfoHeader* header, const char* data, int data_len)
{
	std::ostringstream convert;
	convert << camera_id;
	string strRecorFile = "LocalVideo" + convert.str() + ".data";
	std::ofstream ofs(strRecorFile, std::ios_base::binary | std::ios_base::app);
	ofs.write(data, data_len);
}

/*------------------------------------------------------------------------------
 * ��  ����Զ����Ƶ���ݻص������˴�ֱ��д�ļ�
 * ��  ����[in] user_id Զ���û���ʶ
 *         [in] video_id Զ����Ƶ��ʶ
 *         [in] header ��Ƶͷ��Ϣ�������BitmapInfoHeader��ע��
 *         [in] data ����
 *         [in] data_len ���ݳ���
 * ����ֵ����
------------------------------------------------------------------------------*/
void CSdkManager::OnRemoteVideoStreamRawData(const String& user_id, const String& video_id, BitmapInfoHeader* header, const char* data, int data_len)
{
	string strRecorFile = "RemoteVideo_" + string(user_id.c_str()) + "_" + string(video_id.c_str()) +".data";
	std::ofstream ofs(strRecorFile, std::ios_base::binary | std::ios_base::app);
	ofs.write(data, data_len);
}

/*------------------------------------------------------------------------------
 * ��  ��������������Ƶ¼��
 * ��  ����[in] nDevId ��������ͷ����
 * ����ֵ����
------------------------------------------------------------------------------*/
void CSdkManager::StartRecordLocalVideo(int nDevId)
{
	fsp::CallbackDataDesc data_desc;
	data_desc.data_type = CALLBACK_DATA_LOCAL_VIDEO_RAW;
	data_desc.camera_id = nDevId;
	m_pFspEngin->SetCallbackDataState(data_desc, true);
}

/*------------------------------------------------------------------------------
 * ��  ����ֹͣ������Ƶ¼��
 * ��  ����[in] nDevId ��������ͷ����
 * ����ֵ����
------------------------------------------------------------------------------*/
void CSdkManager::StopRecordLocalVideo(int nDevId)
{
	fsp::CallbackDataDesc data_desc;
	data_desc.data_type = CALLBACK_DATA_LOCAL_VIDEO_RAW;
	data_desc.camera_id = nDevId;
	m_pFspEngin->SetCallbackDataState(data_desc, false);
}

/*------------------------------------------------------------------------------
 * ��  ��������������Ƶ¼��
 * ��  ������
 * ����ֵ����
------------------------------------------------------------------------------*/
void CSdkManager::StartRecordLocalAudio()
{
	fsp::CallbackDataDesc data_desc;
	data_desc.data_type = CALLBACK_DATA_LOCAL_AUDIO_RAW;
	m_pFspEngin->SetCallbackDataState(data_desc, true);
}

/*------------------------------------------------------------------------------
 * ��  ����ֹͣ������Ƶ¼��
 * ��  ������
 * ����ֵ����
------------------------------------------------------------------------------*/
void CSdkManager::StopRecordLocalAudio()
{
	fsp::CallbackDataDesc data_desc;
	data_desc.data_type = CALLBACK_DATA_LOCAL_AUDIO_RAW;
	m_pFspEngin->SetCallbackDataState(data_desc, false);
}

/*------------------------------------------------------------------------------
 * ��  ��������Զ����Ƶ¼��
 * ��  ����[in] user_id Զ���û���ʶ
 *         [in] video_id Զ����Ƶ��ʶ
 * ����ֵ����
------------------------------------------------------------------------------*/
void CSdkManager::StartRecordRemoteVideo(const std::string& user_id, const std::string& video_id)
{
	fsp::CallbackDataDesc data_desc;
	data_desc.data_type = CALLBACK_DATA_REMOTE_VIDEO_RAW;
	data_desc.user_id = user_id.c_str();
	data_desc.video_id = video_id.c_str();
	
	m_pFspEngin->SetCallbackDataState(data_desc, true);
}

/*------------------------------------------------------------------------------
 * ��  ����ֹͣԶ����Ƶ¼��
 * ��  ����[in] user_id Զ���û���ʶ
 *         [in] video_id Զ����Ƶ��ʶ
 * ����ֵ����
------------------------------------------------------------------------------*/
void CSdkManager::StopRecordRemoteVideo(const std::string& user_id, const std::string& video_id)
{
	fsp::CallbackDataDesc data_desc;
	data_desc.data_type = CALLBACK_DATA_REMOTE_VIDEO_RAW;
	data_desc.user_id = user_id.c_str();
	data_desc.video_id = video_id.c_str();

	m_pFspEngin->SetCallbackDataState(data_desc, false);
}

/*------------------------------------------------------------------------------
 * ��  ��������Զ����Ƶ¼��
 * ��  ����[in] user_id Զ���û���ʶ
 * ����ֵ����
------------------------------------------------------------------------------*/
void CSdkManager::StartRecordRemoteAudio(const std::string& user_id)
{
	fsp::CallbackDataDesc data_desc;
	data_desc.data_type = CALLBACK_DATA_REMOTE_AUDIO_RAW;
	data_desc.user_id = user_id.c_str();

	m_pFspEngin->SetCallbackDataState(data_desc, true);
}

/*------------------------------------------------------------------------------
 * ��  ����ֹͣԶ����Ƶ¼��
 * ��  ����[in] user_id Զ���û���ʶ
 * ����ֵ����
------------------------------------------------------------------------------*/
void CSdkManager::StopRecordRemoteAudio(const std::string& user_id)
{
	fsp::CallbackDataDesc data_desc;
	data_desc.data_type = CALLBACK_DATA_REMOTE_AUDIO_RAW;
	data_desc.user_id = user_id.c_str();

	m_pFspEngin->SetCallbackDataState(data_desc, false);
}
