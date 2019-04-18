/*##############################################################################
 * �ļ���DuiLoginWaitWnd.h
 * ��������¼�ȴ�������������붨��
 * ���ߣ�Teck
 * ʱ�䣺2018��5��24��
 * ��Ȩ��Copyright(C) 2018 Fsmeeting.com. All rights reserved.
 ##############################################################################*/
#pragma once

#include "UIlib.h"

using namespace DuiLib;

class CSdkManager;

class CDuiLoginWaitWnd : public WindowImplBase
{
public:
	CDuiLoginWaitWnd();
	~CDuiLoginWaitWnd();

private:
	virtual CDuiString GetSkinFolder() override;
	virtual CDuiString GetSkinFile() override;
	virtual LPCTSTR GetWindowClassName(void) const override;

	virtual void InitWindow() override;
	LRESULT OnDestroy(UINT, WPARAM, LPARAM, BOOL& bHandled) override;
};

