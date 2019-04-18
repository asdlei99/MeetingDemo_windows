/*##############################################################################
 * �ļ���VideoWndMgr.h
 * ��������ƵС���ڹ�������������붨��
 * ���ߣ�Teck
 * ʱ�䣺2018��5��24��
 * ��Ȩ��Copyright(C) 2018 Fsmeeting.com. All rights reserved.
 ##############################################################################*/
#pragma once

#include "DisplayVideoWnd.h"
#include "fsp_engine.h"

#define VIDEO_WND_COUNT 6

class CSdkManager;

class CVideoWndMgr : public IEventCallback
{
public:
	CVideoWndMgr();
	~CVideoWndMgr();

	void Init(HWND hwndParent);
	void SetWndRect(const RECT& rect);

	bool AddBroadcastCam(DWORD dwCamIndex);
	void DelBroadcastCam(DWORD dwCamIndex);

	void AddBroadcastMic(DWORD dwMicIndex);
	void DelBroadcastMic(DWORD dwMicIndex);

	void AddRemoteVideo(const fsp::String& user_id, const fsp::String& video_id);
	void DelRemoteVideo(const fsp::String& user_id, const fsp::String& video_id);

	void AddRemoteAudio(const fsp::String& user_id);
	void DelRemoteAudio(const fsp::String& user_id);

	void OnVideoParamChanged();
	void OnTimer();

	void OnRemoteControlOperation(const fsp::String& user_id, fsp::RemoteControlOperationType operationType);

	void DelAllAV();
private:
	void CalCFloatWndRect(bool bShow);
	bool IsMediaWndUsed(int nWndIndex);
	bool IsRemoteVideoAlreadyOn(const fsp::String& strUserId, const fsp::String& strVideoId);
	bool HasMaximizedWnd();
	void MaximizeDisplayWnd();

	virtual void OnEvent(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

private:
	CDisplayVideoWnd* m_aVideoWnd[VIDEO_WND_COUNT];

	RECT m_rectCanvas;

	HWND m_hParenWnd;
};

