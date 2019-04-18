/*##############################################################################
 * �ļ���DuiLoginErrorWnd.h
 * ��������¼ʧ�ܴ�����������붨��
 * ���ߣ�Teck
 * ʱ�䣺2018��5��24��
 * ��Ȩ��Copyright(C) 2018 Fsmeeting.com. All rights reserved.
 ##############################################################################*/
#pragma once

#include "UIlib.h"
#include "fsp_engine.h"

using namespace DuiLib;

class CSdkManager;

class CDuiLoginErrorWnd : public WindowImplBase
{
public:
	CDuiLoginErrorWnd();
	~CDuiLoginErrorWnd();
	
	void UpdateErrInfo(const CDuiString& strErrInfo);

	DUI_DECLARE_MESSAGE_MAP()

private:
	void OnClick(TNotifyUI& msg);
	
	virtual CDuiString GetSkinFolder() override;
	virtual CDuiString GetSkinFile() override;
	virtual LPCTSTR GetWindowClassName(void) const override;
	virtual void InitWindow() override;
	LRESULT OnDestroy(UINT, WPARAM, LPARAM, BOOL& bHandled) override;

};

