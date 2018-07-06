/*##############################################################################
 * �ļ���VideoWnd.h
 * ��������Ƶ���������붨��
 * ���ߣ�Teck
 * ʱ�䣺2018��5��24��
 * ��Ȩ��Copyright(C) 2018 Fsmeeting.com. All rights reserved.
 ##############################################################################*/
#pragma once
#include "FloatWnd.h"
#include "define.h"

class CVideoWnd : public CFloatWnd
{
public:
	CVideoWnd(LPCTSTR szLayoutXml, LPCTSTR szWndClassName);
	virtual ~CVideoWnd();

	void SetEventCallback(IEventCallback* pCallback);

private:
	virtual void SetWndRect(const RECT& rectWnd) override;
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) override;

private:
	IEventCallback* m_pEventCallback;
};

