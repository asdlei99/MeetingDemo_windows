/*##############################################################################
 * �ļ���DuiFrameWnd.h
 * �����������������붨��
 * ���ߣ�Teck
 * ʱ�䣺2018��5��24��
 * ��Ȩ��Copyright(C) 2018 Fsmeeting.com. All rights reserved.
 ##############################################################################*/
#pragma once

#include <set>
#include "UIlib.h"
#include "VideoWndMgr.h"

using namespace DuiLib;

class CSdkManager;

class CDuiFrameWnd : public WindowImplBase 
{
public:
	CDuiFrameWnd();
	~CDuiFrameWnd();

	DUI_DECLARE_MESSAGE_MAP()

private:
	virtual CDuiString GetSkinFolder() override;
	virtual CDuiString GetSkinFile() override;
	virtual LPCTSTR GetWindowClassName(void) const override;
	virtual void InitWindow() override;
	virtual LRESULT OnDestroy(UINT, WPARAM, LPARAM, BOOL& bHandled) override;
	virtual void OnFinalMessage(HWND hWnd) override;

	void OnClick(TNotifyUI& msg);
	void OnSelectChanged(TNotifyUI& msg);

	void OnClickMicBtn(TNotifyUI& msg);
	void OnClickCamBtn(TNotifyUI& msg);
	void OnClickAudBtn(TNotifyUI& msg);
	void OnClickSettingBtn(TNotifyUI& msg);
	void OnClickRecordBtn(TNotifyUI& msg);

	bool IsCamOpened(DWORD dwCamIndex);

	void OnMicSelectChanged(TNotifyUI& msg);
	void OnCamSelectChanged(TNotifyUI& msg);

	void RefreshMicBtnBkImg();
	void RefreshCamBtnBkImg();
	void RefreshAudBtnBkImg();
	void RefreshRecordBtnBkImg();

	void OnAddRemoteAudio(WPARAM wParam, LPARAM lParam);
	void OnDelRemoteAudio(WPARAM wParam, LPARAM lParam);
	void OnAddRemoteVideo(WPARAM wParam, LPARAM lParam);
	void OnDelRemoteVideo(WPARAM wParam, LPARAM lParam);

	void OnBroadcastCamChanged(WPARAM wParam, LPARAM lParam);

	RECT GetDisplayRect();
	void AdjustTitleGroupUser();
	void AdjustToolbarBtn();

	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) override;

	void StopRecord();

	void OnTimer();

private:
	// 6����Ƶ���ڹ�����
	CVideoWndMgr m_VideoWndMgr;

	// TODO:��Ƶ��󻯴���
	//CDisplayVideoWnd* m_pMaxVideoWnd;

	bool m_bVideoWndInitFlag;

	// ͬʱ���Կ���2������ͷ
	std::set<DWORD> m_setCamOpenIndexes;

	bool m_bBroadcastMic;
	bool m_bOpenSpeaker;
	bool m_bPlayAudio;
	bool m_bRecord;

	std::vector<RemoteAudioInfo> m_vecRemoteAudioInfo;
	std::vector<RemoteVideoInfo> m_vecRemoteVideoInfo;
};

