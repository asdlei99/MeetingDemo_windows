/*##############################################################################
 * 文件：MessageBox.cpp
 * 描述：弹出窗口实现
 * 作者：Teck
 * 时间：2018年5月24日
 * 版权：Copyright(C) 2018 Fsmeeting.com. All rights reserved.
 ##############################################################################*/
#include "stdafx.h"
#include "MessageBox.h"


// 消息映射
DUI_BEGIN_MESSAGE_MAP(CMessageBox, WindowImplBase)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_END_MESSAGE_MAP()

/*------------------------------------------------------------------------------
 * 描  述：构造函数
 * 参  数：无
 * 返回值：无
------------------------------------------------------------------------------*/
CMessageBox::CMessageBox()
{
}

/*------------------------------------------------------------------------------
 * 描  述：析构函数
 * 参  数：无
 * 返回值：无
------------------------------------------------------------------------------*/
CMessageBox::~CMessageBox()
{
}

/*------------------------------------------------------------------------------
 * 描  述：虚拟函数，DUI调用获取窗口xml布局文件目录
 * 参  数：无
 * 返回值：xml布局文件目录
 ------------------------------------------------------------------------------*/
CDuiString CMessageBox::GetSkinFolder()
{
	return CDuiString(L"skin");
}

/*------------------------------------------------------------------------------
 * 描  述：虚拟函数，DUI调用获取窗口xml布局文件名称
 * 参  数：无
 * 返回值：xml布局文件名称
 ------------------------------------------------------------------------------*/
CDuiString CMessageBox::GetSkinFile()
{
	return CDuiString(L"messagebox.xml");
}

/*------------------------------------------------------------------------------
 * 描  述：虚拟函数，DUI调用获取窗口类名称
 * 参  数：无
 * 返回值：窗口类名称
 ------------------------------------------------------------------------------*/
LPCTSTR CMessageBox::GetWindowClassName(void) const
{
	return L"CMessageBox";
}

/*------------------------------------------------------------------------------
 * 描  述：窗口销毁处理
 * 参  数：略
 * 返回值：LRESULT
 ------------------------------------------------------------------------------*/
LRESULT CMessageBox::OnDestroy(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	return 0;
}

/*------------------------------------------------------------------------------
 * 描  述：DUILIB点击消息处理
 * 参  数：[in] msg 通知消息
 * 返回值：无
 ------------------------------------------------------------------------------*/
void CMessageBox::OnClick(TNotifyUI& msg)
{
	if (msg.pSender->GetName() == L"btn_ok")
	{
		Close(IDOK);
	}
	else if(msg.pSender->GetName() == L"btn_cancel"){
		Close(IDCANCEL);
	}
}

/*------------------------------------------------------------------------------
 * 描  述：设置提示文本
 * 参  数：[in] szText 提示文本
 * 返回值：无
 ------------------------------------------------------------------------------*/
void CMessageBox::SetText(LPCTSTR szText)
{
	CLabelUI* pLabel = (CLabelUI*)m_PaintManager.FindControl(L"label_text");
	pLabel->SetText(szText);
}

/*------------------------------------------------------------------------------
 * 描  述：设置按钮文本
 * 参  数：[in] szText 提示文本
 * 返回值：无
 ------------------------------------------------------------------------------*/
void CMessageBox::SetBtnText(LPCTSTR szCancel, LPCTSTR szOk)
{
	CButtonUI* pBtnCancel = (CButtonUI*)m_PaintManager.FindControl(L"btn_cancel");
	pBtnCancel->SetText(szCancel);

	CButtonUI* pBtnOk = (CButtonUI*)m_PaintManager.FindControl(L"btn_ok");
	pBtnOk->SetText(szOk);
}

/*------------------------------------------------------------------------------
 * 描  述：窗口销毁前，删除自身
 * 参  数：[in] hWnd 窗口句柄
 * 返回值：无
 ------------------------------------------------------------------------------*/
void CMessageBox::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}