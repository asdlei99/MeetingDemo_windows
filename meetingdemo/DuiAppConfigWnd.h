/*##############################################################################
 * �ļ���DuiAppConfigWnd.h
 * ������Ӧ�����ô�����������붨��
 * ���ߣ�Teck
 * ʱ�䣺2018��8��2��
 * ��Ȩ��Copyright(C) 2018 Fsmeeting.com. All rights reserved.
 ##############################################################################*/
#pragma once

#include "UIlib.h"
#include "FloatWnd.h"

using namespace DuiLib;
class CDuiAppConfigWnd : public WindowImplBase
{
public:
	CDuiAppConfigWnd();
	~CDuiAppConfigWnd();

	virtual CDuiString GetSkinFolder() override;
	virtual CDuiString GetSkinFile() override;
	virtual LPCTSTR GetWindowClassName(void) const override;

	DUI_DECLARE_MESSAGE_MAP()

private:
	void OnClick(TNotifyUI& msg);
	virtual void InitWindow() override;

	void SetUseDefaultConfig();
	void SetUseUserDefineConfig();
	void RestartApp();

private:
	bool m_bUseDefault;
};

