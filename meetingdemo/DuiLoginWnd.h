/*##############################################################################
 * �ļ���DuiLoginWnd.h
 * ��������¼������������붨��
 * ���ߣ�Teck
 * ʱ�䣺2018��5��24��
 * ��Ȩ��Copyright(C) 2018 Fsmeeting.com. All rights reserved.
 ##############################################################################*/
#pragma once

#include "UIlib.h"
#include "Resource.h"

using namespace DuiLib;

class CSdkManager;

class CDuiLoginWnd : public WindowImplBase
{
public:
	CDuiLoginWnd();
	~CDuiLoginWnd();

	DUI_DECLARE_MESSAGE_MAP()

private:
	virtual CDuiString GetSkinFolder() override;
	virtual CDuiString GetSkinFile() override;
	virtual LPCTSTR GetWindowClassName(void) const override;
	virtual void InitWindow() override;
	LRESULT OnDestroy(UINT, WPARAM, LPARAM, BOOL& bHandled) override;

	void OnClick(TNotifyUI& msg);
};

