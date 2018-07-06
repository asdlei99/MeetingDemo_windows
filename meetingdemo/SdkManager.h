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

class CSdkManager : public IFspEngineEventHandler
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

