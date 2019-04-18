/*##############################################################################
 * �ļ���define.h
 * ���������������Ͷ���
 * ���ߣ�Teck
 * ʱ�䣺2018��5��24��
 * ��Ȩ��Copyright(C) 2018 Fsmeeting.com. All rights reserved.
 ##############################################################################*/
#pragma once

#include "fsp_engine.h"

// ��Чֵ����
#define INVALID_MIC_INDEX					0xFFFFFFFF
#define INVALID_AUD_INDEX					0xFFFFFFFF
#define INVALID_CAM_INDEX					0xFFFFFFFF
#define INVALID_VOLUME						0xFFFFFFFF

// �ڲ��Զ�����Ϣ
#define DUILIB_MSG_SET_MIC_DEV				WM_USER + 101
#define DUILIB_MSG_SET_AUD_DEV				WM_USER + 102
#define DUILIB_MSG_SET_MIC_VOL				WM_USER + 103
#define DUILIB_MSG_SET_AUD_VOL				WM_USER + 104
#define	DUILIB_MSG_ADD_REMOTE_VIDEO			WM_USER + 105
#define DUILIB_MSG_DEL_REMOTE_VIDEO			WM_USER + 106
#define DUILIB_MSG_ADD_REMOTE_AUDIO			WM_USER + 107
#define DUILIB_MSG_DEL_REMOTE_AUDIO			WM_USER + 108
#define DUILIB_MSG_TOO_MANY_CAM				WM_USER + 109
#define DUILIB_MSG_BROADCAST_CAM_CHANGED	WM_USER + 110
#define DUILIB_MSG_CHANGE_CAM_FAILED		WM_USER + 111
#define	DUILIB_MSG_BROADCAST_CAM_FAILED		WM_USER + 112
#define	DUILIB_MSG_VIDEO_PARAM_CHANGED		WM_USER + 113
#define DUILIB_MSG_CONNECT_LOST             WM_USER + 114
#define DUILIB_MSG_FSP_EVENT				WM_USER + 115
#define DUILIB_MSG_REMOTECONTROL_EVENT      WM_USER + 116
#define DUILIB_MSG_DEVICECHANGE             WM_USER + 117

// ��¼���
#define LOGIN_SUCCESS		1
#define LOGIN_FAILED		0

// ��ʱ��
#define UPDATE_VIDEO_INFO_TIMER_ID			100
#define UPDATE_VIDEO_INFO_INTERVAL			200	// ms
#define UPDATE_SETTING_INFO_TIMER_ID		101
#define UPDATE_SETTING_INFO_INTERVAL		200	// ms

// UI
#define UI_VIDEO_INFO_HEIGHT				30
#define UI_VIDEO_WND_HEIGHT					300
#define UI_VIDEO_WND_WIDTH					400
#define UI_MAIN_WND_WIDTH					1200
#define UI_VIDEO_WND_ROW_SIZE				2
#define UI_VIDEO_WND_COLUMN_SIZE			3
#define UI_MAIN_FRAME_TITLE_HEIGHT			30
#define UI_MAIN_FRAME_TOOLBAR_HEIGHT		90

// �ַ���
#define STRING_TOO_MANY_CAM					L"�������㲥2����Ƶ�豸��"
#define STRING_CHANGE_CAM_FAILED			L"�л�����ͷʧ�ܣ����ܷ�����ϵͳ������ߴ��л�������ͷ�Ѿ���ʹ���У�"
#define STRING_BROADCAST_CAM_FAILED			L"�㲥����ͷʧ�ܣ�"

struct VideoResolution
{
	DWORD dwWidth;
	DWORD dwHeight;
};

static VideoResolution VideoResolutions[] =
{
	{320, 240},
	{480, 360},
	{640, 480},
	{1280, 720},
	{1920, 1080}
};

// ��Ϣ�ṹ��
struct RemoteVideoInfo
{
	fsp::String strUserId;
	fsp::String strVideoId;
};

struct RemoteAudioInfo
{
	fsp::String strUserId;
};

struct RemoteControlInfo
{
	fsp::String strUserId;
	fsp::RemoteControlOperationType operationType;
};

struct ScreenShareConfig
{
	fsp::ScreenShareQualityBias qualityBias;
	int left;
	int top;
	int right;
	int bottom;

	ScreenShareConfig()
	{
		qualityBias = fsp::SCREEN_SHARE_BIAS_QUALITY;
		left = 0;
		top = 0;
		right = 0;
		bottom = 0;
	}
};

struct IEventCallback
{
	virtual void OnEvent(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;
};