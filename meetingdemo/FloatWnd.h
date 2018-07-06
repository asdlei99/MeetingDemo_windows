/*##############################################################################
 * �ļ���FloatWnd.h
 * ����������������������붨��
 * ���ߣ�Teck
 * ʱ�䣺2018��5��24��
 * ��Ȩ��Copyright(C) 2018 Fsmeeting.com. All rights reserved.
 ##############################################################################*/
#pragma once

#include "UIlib.h"

using namespace DuiLib;

class CFloatWnd : public WindowImplBase
{
public:
	CFloatWnd(LPCTSTR szLayoutXml, LPCTSTR szWndClassName);
	virtual ~CFloatWnd();

	virtual CDuiString GetSkinFolder() override;
	virtual CDuiString GetSkinFile() override;
	virtual LPCTSTR GetWindowClassName(void) const override;

	bool CreateWnd(HWND hParentWnd, LPCTSTR szWndName, DWORD dwWndStyle, bool bShowAfterCreate);

	virtual void SetWndRect(const RECT& rectWnd);

	CPaintManagerUI& GetPaintManager() { return m_PaintManager; }

private:
	static DWORD WINAPI WndThreadProc(LPVOID lpParam);


protected:
	CDuiString	m_szLayoutXml;
	CDuiString	m_szWndClassName;
	HWND		m_hParentWnd;
	DWORD		m_dwThreadId;
	bool		m_bShowAfterCreate;
};

