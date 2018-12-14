/*##############################################################################
 * �ļ���SdkManager.h
 * ������SDK�������������붨��
 * ���ߣ�Teck
 * ʱ�䣺2018��5��24��
 * ��Ȩ��Copyright(C) 2018 Fsmeeting.com. All rights reserved.
 ##############################################################################*/
#pragma once

#include "fsp_engine.h"
#include "DuiLoginWnd.h"
#include "DuiLoginWaitWnd.h"
#include "DuiLoginErrorWnd.h"
#include "DuiFrameWnd.h"

using namespace fsp;

class CSdkManager : public IFspEngineEventHandler, public IFspEngineDataHandler
{
public:
	static CSdkManager& GetInstance();

	bool Init();
	void Start();
	void SetRestart();

	void OnLoginResult(fsp::ErrCode result);
	void JoinGroup(LPCTSTR szGroup, LPCTSTR szUser);

	void SetOpenMic(DWORD dwMicIndex);
	DWORD GetOpenMic();

	void SetOpenAud(DWORD dwAudIndex);
	DWORD GetOpenAud();

	void SetMicVol(DWORD dwMicVol);
	DWORD GetMicVol();

	void SetAudVol(DWORD dwAudVol);
	DWORD GetAudVol();

	void SetLoginGroup(LPCTSTR szGroup);
	const CDuiString& GetLoginGroup();

	void SetLoginUser(LPCTSTR szUser);
	const CDuiString& GetLoginUser();

	void SetResolution(DWORD dwResolutionIndex);
	DWORD GetResolution();

	void SetFrameRate(DWORD dwFrameRate);
	DWORD GetFrameRate();

	IFspEngine* GetFspEngin() { return m_pFspEngin; }

	CDuiFrameWnd* GetFrameWnd() { return m_pDuiFrameWnd; }

	void StartRecordLocalVideo(int nDevId);
	void StopRecordLocalVideo(int nDevId);

	void StartRecordLocalAudio();
	void StopRecordLocalAudio();

	void StartRecordRemoteVideo(const std::string& user_id, const std::string& video_id);
	void StopRecordRemoteVideo(const std::string& user_id, const std::string& video_id);

	void StartRecordRemoteAudio(const std::string& user_id);
	void StopRecordRemoteAudio(const std::string& user_id);

private:
	CSdkManager();
	~CSdkManager();

	virtual void OnDeviceChange(DeviceEventType device_event) override;
	virtual void OnEvent(EventType event_type, ErrCode err_code) override;
	virtual void OnRemoteVideoEvent(const String& user_id, 
		const String& video_id,
		RemoteVideoEventType remote_video_event) override;
	virtual void OnRemoteAudioEvent(const String& user_id, 
		RemoteAudioEventType remote_audio_event) override;

	virtual void OnScreenShareEvent(const String &user_id, ScreenShareEventType screen_share_event) {}
	virtual void OnScreenShareRemoteControlEvent(const String& user_id, const String& src_user_name, ScreenShareRemoteControlEventType event_type) {}

	virtual void OnLocalAudioStreamRawData(const char* data, int data_len) override;
	virtual void OnRemoteAudioStreamRawData(const String& user_id, const char* data, int data_len) override;
	virtual void OnMixAudioStreamRawData(const char* data, int data_len) override;
	virtual void OnLocalVideoStreamRawData(int camera_id, BitmapInfoHeader* header, const char* data, int data_len) override;
	virtual void OnRemoteVideoStreamRawData(const String& user_id, const String& video_id, BitmapInfoHeader* header, const char* data, int data_len) override;

	std::string BuildToken(char* szGroupId, char* szUserId);

private:
	CDuiLoginWnd*		m_pDuiLoginWnd;
	CDuiLoginWaitWnd*	m_pDuiLoginWaitWnd;
	CDuiLoginErrorWnd*	m_pDuiLoginErrorWnd;
	CDuiFrameWnd*		m_pDuiFrameWnd;

	IFspEngine*			m_pFspEngin;
	FspEngineContext	m_FspEnginContext;

	CDuiString m_strGroup;
	CDuiString m_strUser;

	DWORD m_dwMicSetVolume;	// ��˷���������ֵ
	DWORD m_dwMicOpenIndex;	// ѡ��ʹ�õ���˷�

	DWORD m_dwAudSetVoluem; // ��������������ֵ
	DWORD m_dwAudOpenIndex; // ѡ��ʹ�õ�������

	DWORD m_dwResolutionIndex;	// �ֱ���
	DWORD m_dwFrameRate;		// ֡��

	// ȷ�������ڴ������ٴ���SDK�ص���Ϣ����������
	bool	m_bMainFrameCreated;
	HANDLE	m_bSemaphore;

	bool m_bRestart;
};

