/*##############################################################################
 * �ļ���MessageBox.cpp
 * ������������������
 * ���ߣ�Teck
 * ʱ�䣺2018��5��24��
 * ��Ȩ��Copyright(C) 2018 Fsmeeting.com. All rights reserved.
 ##############################################################################*/
#pragma once

#include "UIlib.h"

using namespace DuiLib;

class CMessageBox : public WindowImplBase
{
public:
	CMessageBox();
	~CMessageBox();

	void SetText(LPCTSTR szText);
	void SetBtnText(LPCTSTR szCancel, LPCTSTR szOk);

	DUI_DECLARE_MESSAGE_MAP()

private:
	virtual CDuiString GetSkinFolder() override;
	virtual CDuiString GetSkinFile() override;
	virtual LPCTSTR GetWindowClassName(void) const override;

	virtual void OnFinalMessage(HWND hWnd) override;

	void OnClick(TNotifyUI& msg);

	LRESULT OnDestroy(UINT, WPARAM, LPARAM, BOOL& bHandled) override;
};

