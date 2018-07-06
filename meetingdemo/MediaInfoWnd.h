/*##############################################################################
 * �ļ���MediaInfoWnd.h
 * ������ý����Ϣ��ʾ���������붨��
 * ���ߣ�Teck
 * ʱ�䣺2018��5��24��
 * ��Ȩ��Copyright(C) 2018 Fsmeeting.com. All rights reserved.
 ##############################################################################*/
#pragma once
#include "FloatWnd.h"

class CMediaInfoWnd : public CFloatWnd
{
public:
	CMediaInfoWnd(LPCTSTR szLayoutXml, LPCTSTR szWndClassName);
	virtual ~CMediaInfoWnd();

private:
	virtual void SetWndRect(const RECT& rectWnd) override;
};

