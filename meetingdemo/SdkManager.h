/*##############################################################################
 * �ļ���SdkManager.h
 * ������SDK�������������붨��
 * ���ߣ�Teck
 * ʱ�䣺2018��5��24��
 * ��Ȩ��Copyright(C) 2018 Fsmeeting.com. All rights reserved.
 ##############################################################################*/
#pragma once

#include "fsp_engine.h"

#include "LoginWnd.h"
#include "MeetingMainWnd.h"
#include "UserStateWnd.h"

using namespace fsp;

//Ϊ�˴���fsp�¼�ת�����̵߳���Ϣ�����Դ�WindowImplBase�̳�
class CSdkManager : public IFspEngineEventHandler, public IFspSignalingEventHandler, public WindowImplBase
{
public:
	static CSdkManager& GetInstance();
	
	bool Init();
	void Destroy();

	void OpenLoginWnd();

	bool IsLogined();

	fsp::ErrCode Login(LPCTSTR szUser);
	fsp::ErrCode JoinGroup(LPCTSTR szGroup);
	fsp::ErrCode LeaveGroup();

	void SetOpenMic(DWORD dwMicIndex);
	DWORD GetOpenMic();

	void SetOpenAud(DWORD dwAudIndex);
	DWORD GetOpenAud();

	void SetMicVol(DWORD dwMicVol);
	DWORD GetMicVol();

	void SetAudVol(DWORD dwAudVol);
	DWORD GetAudVol();

	const std::string& GetLoginGroupId();
	const std::string GetLoginUserId();

	void SetResolution(DWORD dwResolutionIndex);
	DWORD GetResolution();

	void SetFrameRate(DWORD dwFrameRate);
	DWORD GetFrameRate();

	fsp::Vector<fsp::UserInfo> GetUserList() { return m_vecUsers; }

	void SetScreenShareConfig(const ScreenShareConfig& config);
	ScreenShareConfig GetScreenShareConfig() const;

	IFspEngine* GetFspEngin() { return m_pFspEngine; }

	CMeetingMainWnd* GetMeetingMainWnd() { return m_pMeetingMainWnd; }

	CUserStateWnd* GetUserStateWnd() { return m_pUserStateWnd; }

	CDuiString BuildErrorInfo(fsp::ErrCode errCode);

	void SetCurSelectChatUser(fsp::String userid) { m_CurSelectChatUser = userid; }
	const fsp::String GetCurSelectChatUser() { return m_CurSelectChatUser; }

	fsp::ErrCode StartPublishVideo(const char* szVideoId, int nCameraId);
	fsp::ErrCode StopPublishVideo(const char* szVideoId);
	
private:
	CSdkManager();
	~CSdkManager();

	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) override;

	virtual CDuiString GetSkinFolder() override;
	virtual CDuiString GetSkinFile() override;
	virtual LPCTSTR GetWindowClassName(void) const override;
	
	/////////  begin IFspEngineEventHandler
	void OnFspEvent(fsp::EventType eventType, fsp::ErrCode result);
	virtual void OnDeviceChange(DeviceEventType device_event) override;
	virtual void OnEvent(EventType event_type, ErrCode err_code) override;
	virtual void OnRemoteVideoEvent(const String& user_id, 
		const String& video_id,
		RemoteVideoEventType remote_video_event) override;
	virtual void OnRemoteAudioEvent(const String& user_id, 
		RemoteAudioEventType remote_audio_event) override;
	virtual void OnRemoteControlOperationEvent(const String& user_id,
		fsp::RemoteControlOperationType operation_type) override;
	virtual void OnGroupUsersRefreshed(const fsp::Vector<fsp::String>& user_ids) override;
	virtual void OnRemoteUserEvent(const char* szRemoteUserId, fsp::RemoteUserEventType remote_user_eventType) override;
	/////////   end IFspEngineEventHandler


	////////////// begin IFspSignalingEventHandler
	void OnUsersStateRefreshed(fsp::ErrCode errCode, unsigned int nRequestId, const Vector<fsp::UserInfo> users);
	void OnInviteCome(const char* szInviterUserId,
		unsigned int nInviteId, const char* szGroupId, const char* szMsg);
	void OnInviteAccepted(const char* szRemoteUserId, unsigned int nInviteId);
	void OnInviteRejected(const char* szRemoteUserId, unsigned int nInviteId);
	void OnUserMsgCome(const char* szSenderUserId, unsigned int nMsgId, const char* szMsg);
	void OnGroupMsgCome(const char* szSenderUserId, unsigned int nMsgId, const char* szMsg);
	/////////////  end IFspSignalingEventHandler

	std::string BuildToken(const std::string& struserId);
	void ShowErrorWnd(const CDuiString& strErrInfo);

	LRESULT OnTimerEx(UINT uMsg);
	
private:
	CLoginWnd*		    m_pLoginWnd = nullptr;
	CMeetingMainWnd*	m_pMeetingMainWnd = nullptr;
	CUserStateWnd*      m_pUserStateWnd = nullptr;

	bool m_isLogined = false;

	IFspEngine*			m_pFspEngine;
	fsp::IFspSignaling* m_pFspSignaling;

	FspEngineContext	m_FspEnginContext;

	std::string m_strMyGroupId;
	std::string m_strMyUserId;

	DWORD m_dwMicSetVolume;	// ��˷���������ֵ
	DWORD m_dwMicOpenIndex;	// ѡ��ʹ�õ���˷�

	DWORD m_dwAudSetVoluem; // ��������������ֵ
	DWORD m_dwAudOpenIndex; // ѡ��ʹ�õ�������

	DWORD m_dwResolutionIndex;	// �ֱ���
	DWORD m_dwFrameRate;		// ֡��

	std::set<std::string> m_setMyPublishedVideoIds;
	
	ScreenShareConfig m_screenShareConfig;

	fsp::Vector<fsp::UserInfo> m_vecUsers;

	fsp::String m_CurSelectChatUser="������";
};

