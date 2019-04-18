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

//Ϊ�˴���fsp�¼�ת�����̵߳���Ϣ�����Դ�WindowImplBase�̳�
class CSdkManager : public IFspEngineEventHandler, public WindowImplBase
{
public:
	static CSdkManager& GetInstance();
	
	bool Init();
	void Destroy();

	void OpenLoginWnd();

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

	void SetScreenShareConfig(const ScreenShareConfig& config);
	ScreenShareConfig GetScreenShareConfig() const;

	IFspEngine* GetFspEngin() { return m_pFspEngin; }

	CDuiFrameWnd* GetFrameWnd() { return m_pDuiFrameWnd; }
private:
	CSdkManager();
	~CSdkManager();
	
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) override;

	virtual CDuiString GetSkinFolder() override;
	virtual CDuiString GetSkinFile() override;
	virtual LPCTSTR GetWindowClassName(void) const override;
	
	void OnFspEvent(fsp::EventType eventType, fsp::ErrCode result);
	void ShowErrorWnd(const CDuiString& strErrInfo);

	virtual void OnDeviceChange(DeviceEventType device_event) override;
	virtual void OnEvent(EventType event_type, ErrCode err_code) override;
	virtual void OnRemoteVideoEvent(const String& user_id, 
		const String& video_id,
		RemoteVideoEventType remote_video_event) override;
	virtual void OnRemoteAudioEvent(const String& user_id, 
		RemoteAudioEventType remote_audio_event) override;

	virtual void OnRemoteControlOperationEvent(const String& user_id,
		fsp::RemoteControlOperationType operation_type);

	std::string BuildToken(char* szGroupId, char* szUserId);

private:
	CDuiLoginWnd*		m_pDuiLoginWnd = nullptr;
	CDuiLoginWaitWnd*	m_pDuiLoginWaitWnd = nullptr;
	CDuiLoginErrorWnd*	m_pDuiLoginErrorWnd = nullptr;
	CDuiFrameWnd*		m_pDuiFrameWnd = nullptr;

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
	
	ScreenShareConfig m_screenShareConfig;

	bool m_bRestart;
};

