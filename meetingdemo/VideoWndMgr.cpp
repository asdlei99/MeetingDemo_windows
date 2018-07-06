/*##############################################################################
 * �ļ���VideoWndMgr.cpp
 * �������������������ƵС����
 * ���ߣ�Teck
 * ʱ�䣺2018��5��24��
 * ��Ȩ��Copyright(C) 2018 Fsmeeting.com. All rights reserved.
 ##############################################################################*/
#include "stdafx.h"
#include "VideoWndMgr.h"
#include "DisplayVideoWnd.h"
#include "fsp_engine.h"
#include "SdkManager.h"
#include "define.h"

/*------------------------------------------------------------------------------
 * ��  �������캯��
 * ��  ������
 * ����ֵ����
------------------------------------------------------------------------------*/
CVideoWndMgr::CVideoWndMgr()
{
	ZeroMemory(&m_rectCanvas, sizeof(m_rectCanvas));
	ZeroMemory(m_aVideoWnd, sizeof(m_aVideoWnd));
}

/*------------------------------------------------------------------------------
 * ��  ������������
 * ��  ������
 * ����ֵ����
------------------------------------------------------------------------------*/
CVideoWndMgr::~CVideoWndMgr()
{
	for (int i = 0; i < VIDEO_WND_COUNT; i++)
	{
		if (m_aVideoWnd[i] != nullptr)
			delete m_aVideoWnd[i];
	}
}

/*------------------------------------------------------------------------------
 * ��  ������ӹ㲥����ͷ
 * ��  ����[in] dwCamIndex ����ͷ����
 * ����ֵ���ɹ�/ʧ��
------------------------------------------------------------------------------*/
bool CVideoWndMgr::AddBroadcastCam(DWORD dwCamIndex)
{
	// �Ƿ��ظ��㲥��Ƶ
	for (int i = 0; i < VIDEO_WND_COUNT; i++)
	{
		if (m_aVideoWnd[i]->IsHoldLocalVideo(dwCamIndex))
			return true;
	}

	// �����Ƿ������㲥����Ƶ
	for (int i = 0; i < VIDEO_WND_COUNT; i++)
	{
		if (m_aVideoWnd[i]->IsHoldLocalAudio()
			&& !m_aVideoWnd[i]->IsHoldLocalVideo())
		{
			return m_aVideoWnd[i]->StartPublishCam(dwCamIndex);
		}
	}

	// ����Ѱ��һ�����д���
	for (int i = 0; i < VIDEO_WND_COUNT; i++)
	{
		if (m_aVideoWnd[i]->IsIdle())
		{
			return m_aVideoWnd[i]->StartPublishCam(dwCamIndex);
		}
	}

	return false;
}

/*------------------------------------------------------------------------------
 * ��  ����ɾ���㲥������ͷ
 * ��  ����[in] dwCamIndex ����ͷ����
 * ����ֵ����
------------------------------------------------------------------------------*/
void CVideoWndMgr::DelBroadcastCam(DWORD dwCamIndex)
{
	for (int i = 0; i < VIDEO_WND_COUNT; i++)
	{
		if (m_aVideoWnd[i]->StopPublishCam(dwCamIndex))
			break;
	}
}

/*------------------------------------------------------------------------------
 * ��  ����������ƵС���ڵľ�������
 * ��  ����[in] bShow �Ƿ���ʾ
 * ����ֵ����
------------------------------------------------------------------------------*/
void CVideoWndMgr::CalCFloatWndRect(bool bShow)
{
	// ������Ƶ������ʾλ�úʹ�С���������У�
	DWORD dwWndWidth = (m_rectCanvas.right - m_rectCanvas.left) / UI_VIDEO_WND_COLUMN_SIZE;
	DWORD dwWndHeight = (m_rectCanvas.bottom - m_rectCanvas.top) / UI_VIDEO_WND_ROW_SIZE;

	RECT rect;
	for (int i = 0; i < VIDEO_WND_COUNT; i++)
	{
		rect.left	= m_rectCanvas.left + i % UI_VIDEO_WND_COLUMN_SIZE * dwWndWidth;
		rect.top	= m_rectCanvas.top + i / UI_VIDEO_WND_COLUMN_SIZE * dwWndHeight;
		rect.right	= rect.left + dwWndWidth;
		rect.bottom = rect.top + dwWndHeight;

		m_aVideoWnd[i]->SetWndRect(rect, bShow);
	}
}

/*------------------------------------------------------------------------------
 * ��  ������ʼ��
 * ��  ����[in] hParentWnd ������
 *         [in] rectCanvas ������Ƶ�Ӵ��ڵı������ھ�������
 * ����ֵ���ɹ�/ʧ��
------------------------------------------------------------------------------*/
void CVideoWndMgr::SetWndRect(const RECT& rect)
{
	m_rectCanvas = rect;
	CalCFloatWndRect(true);
}

/*------------------------------------------------------------------------------
 * ��  ������ʼ��
 * ��  ����[in] hParentWnd ������
 * ����ֵ���ɹ�/ʧ��
------------------------------------------------------------------------------*/
void CVideoWndMgr::Init(HWND hParentWnd)
{
	m_hParenWnd = hParentWnd;

	// ������Ƶ����
	for (int i = 0; i < VIDEO_WND_COUNT; i++)
	{
		m_aVideoWnd[i] = new CDisplayVideoWnd();
		m_aVideoWnd[i]->Init(m_hParenWnd);
		m_aVideoWnd[i]->SetEventCallback(this);
	}
}

/*------------------------------------------------------------------------------
 * ��  �����Ƿ��б�˫���Ŵ����Ƶ����
 * ��  ������
 * ����ֵ����/��
------------------------------------------------------------------------------*/
bool CVideoWndMgr::HasMaximizedWnd()
{
	bool bHasMaxWnd = false;
	for (int i = 0; i < VIDEO_WND_COUNT; i++)
	{
		if (m_aVideoWnd[i]->IsVideoMaximized())
		{
			bHasMaxWnd = true;
			break;
		}
	}

	return bHasMaxWnd;
}

/*------------------------------------------------------------------------------
 * ��  �������������
 * ��  ������
 * ����ֵ����
------------------------------------------------------------------------------*/
void CVideoWndMgr::MaximizeDisplayWnd()
{
	for (int i = 0; i < VIDEO_WND_COUNT; i++)
	{
		if (m_aVideoWnd[i]->IsVideoMaximized())
		{
			RECT rect;
			::GetClientRect(m_hParenWnd, &rect);
			rect.top += UI_MAIN_FRAME_TITLE_HEIGHT;
			rect.bottom -= UI_MAIN_FRAME_TOOLBAR_HEIGHT;

			m_aVideoWnd[i]->SetWndRect(rect, true);
		}
		else
		{
			m_aVideoWnd[i]->ShowWindow(false);
		}
	}
}

/*------------------------------------------------------------------------------
 * ��  �����������˫���¼�
 * ��  ����[in] uMsg	��Ϣ����
 *         [in] wParam	����
 *         [in] lParam	����
 * ����ֵ����
------------------------------------------------------------------------------*/
void CVideoWndMgr::OnEvent(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_LBUTTONDBLCLK)
	{
		if (HasMaximizedWnd())
			MaximizeDisplayWnd();
		else
			CalCFloatWndRect(true);
	}
}

/*------------------------------------------------------------------------------
 * ��  �������Զ���û���ĳһ·��Ƶ
 * ��  ����[in] strUserId	�û�ID
 *         [in] strVideoId	��ƵID
 * ����ֵ����
------------------------------------------------------------------------------*/
void CVideoWndMgr::AddRemoteVideo(const fsp::String& strUserId, const fsp::String& strVideoId)
{
	// �ظ��㲥��Ƶ
	for (int i = 0; i < VIDEO_WND_COUNT; i++)
	{
		// FIXME: ��׳�Կ��ǣ�����Զ�˵���
		if (m_aVideoWnd[i]->IsHoldRemoteVideo(strUserId, strVideoId))
		{
			m_aVideoWnd[i]->AddRemoteVideo(strUserId, strVideoId);
			return;
		}
	}

	// ���´��û���ǰ��û�й㲥��Ƶ
	for (int i = 0; i < VIDEO_WND_COUNT; i++)
	{
		if (m_aVideoWnd[i]->IsHoldRemoteAudio(strUserId)
			&& !m_aVideoWnd[i]->IsHoldRemoteVideo(strUserId))
		{
			m_aVideoWnd[i]->AddRemoteVideo(strUserId, strVideoId);
			return;
		}
	}

	// ������һ�����еĴ���
	for (int i = 0; i < VIDEO_WND_COUNT; i++)
	{
		if (m_aVideoWnd[i]->IsIdle())
		{
			m_aVideoWnd[i]->AddRemoteVideo(strUserId, strVideoId);
			return;
		}
	}
}

/*------------------------------------------------------------------------------
 * ��  ����ɾ��Զ���û���ĳһ·��Ƶ
 * ��  ����[in] strUserId	�û�ID
 *         [in] strVideoId	��ƵID
 * ����ֵ����
------------------------------------------------------------------------------*/
void CVideoWndMgr::DelRemoteVideo(const fsp::String& strUserId, const fsp::String& strVideoId)
{
	for (int i = 0; i < VIDEO_WND_COUNT; i++)
	{
		if (m_aVideoWnd[i]->IsHoldRemoteVideo(strUserId, strVideoId))
		{
			m_aVideoWnd[i]->DelRemoteVideo(strUserId, strVideoId);
			return;
		}
	}
}

/*------------------------------------------------------------------------------
 * ��  �������Զ��ĳ�û�����Ƶ
 * ��  ����[in] strUserId	�û�ID
 * ����ֵ����
------------------------------------------------------------------------------*/
void CVideoWndMgr::AddRemoteAudio(const fsp::String& strUserId)
{
	// �ظ��㲥��Ƶ
	for (int i = 0; i < VIDEO_WND_COUNT; i++)
	{
		// FIXME: ��׳�Կ��ǣ�����Զ�˵���
		if (m_aVideoWnd[i]->IsHoldRemoteAudio(strUserId))
		{
			m_aVideoWnd[i]->AddRemoteAudio(strUserId);
			return;
		}		
	}

	// ����֮ǰ�Ƿ������㲥�����û�����Ƶ
	for (int i = 0; i < VIDEO_WND_COUNT; i++)
	{
		if (m_aVideoWnd[i]->IsHoldRemoteVideo(strUserId)
			&& !m_aVideoWnd[i]->IsHoldRemoteAudio(strUserId))
		{
			m_aVideoWnd[i]->AddRemoteAudio(strUserId);
			return;
		}
	}

	// ������һ�����еĴ���
	for (int i = 0; i < VIDEO_WND_COUNT; i++)
	{
		if (m_aVideoWnd[i]->IsIdle())
		{
			m_aVideoWnd[i]->AddRemoteAudio(strUserId);
			return;
		}
	}
}

/*------------------------------------------------------------------------------
 * ��  ����ɾ��Զ��ĳ�û�����Ƶ
 * ��  ����[in] strUserId	�û�ID
 * ����ֵ����
------------------------------------------------------------------------------*/
void CVideoWndMgr::DelRemoteAudio(const fsp::String& strUserId)
{
	for (int i = 0; i < VIDEO_WND_COUNT; i++)
	{
		if (m_aVideoWnd[i]->IsHoldRemoteAudio(strUserId))
		{
			m_aVideoWnd[i]->DelRemoteAudio(strUserId);
			return;
		}
	}
}

/*------------------------------------------------------------------------------
 * ��  ������ӱ��ع㲥����˷�
 * ��  ����[in] dwMicIndex ��˷�����
 * ����ֵ����
------------------------------------------------------------------------------*/
void CVideoWndMgr::AddBroadcastMic(DWORD dwMicIndex)
{
	// �ظ��㲥��Ƶ
	for (int i = 0; i < VIDEO_WND_COUNT; i++)
	{
		if (m_aVideoWnd[i]->IsHoldLocalAudio(dwMicIndex))
			return;
	}

	// �����Ƿ������㲥����Ƶ
	for (int i = 0; i < VIDEO_WND_COUNT; i++)
	{
		if (m_aVideoWnd[i]->IsHoldLocalVideo()
			&& !m_aVideoWnd[i]->IsHoldLocalAudio())
		{
			m_aVideoWnd[i]->StartPublishMic(dwMicIndex);
			return;
		}
	}

	// ������һ�����д���
	for (int i = 0; i < VIDEO_WND_COUNT; i++)
	{
		if (m_aVideoWnd[i]->IsIdle())
		{
			m_aVideoWnd[i]->StartPublishMic(dwMicIndex);
			return;
		}
	}
}

/*------------------------------------------------------------------------------
 * ��  ����ɾ�����ع㲥����˷�
 * ��  ����[in] dwMicIndex ��˷�����
 * ����ֵ����
------------------------------------------------------------------------------*/
void CVideoWndMgr::DelBroadcastMic(DWORD dwMicIndex)
{
	for (int i = 0; i < VIDEO_WND_COUNT; i++)
	{
		if (m_aVideoWnd[i]->StopPublishMic(dwMicIndex))
			return;
	}
}

/*------------------------------------------------------------------------------
 * ��  ������ʱ������
 * ��  ������
 * ����ֵ����
------------------------------------------------------------------------------*/
void CVideoWndMgr::OnTimer()
{
	for (int i = 0; i < VIDEO_WND_COUNT; i++)
	{
		m_aVideoWnd[i]->OnTimer();
	}
}

/*------------------------------------------------------------------------------
 * ��  ������Ƶ�����ı䣬Ҫ��������
 * ��  ������
 * ����ֵ����
------------------------------------------------------------------------------*/
void CVideoWndMgr::OnVideoParamChanged()
{
	for (int i = 0; i < VIDEO_WND_COUNT; i++)
	{
		m_aVideoWnd[i]->OnVideoParamChanged();
	}
}