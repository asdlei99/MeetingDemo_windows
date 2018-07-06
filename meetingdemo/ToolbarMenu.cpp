/*##############################################################################
 * �ļ���ToolbarMenu.cpp
 * �������ײ��������˵�ʵ��
 * ���ߣ�Teck
 * ʱ�䣺2018��5��24��
 * ��Ȩ��Copyright(C) 2018 Fsmeeting.com. All rights reserved.
 ##############################################################################*/
#include "stdafx.h"
#include "ToolbarMenu.h"
#include "UIMenu.h"

/*------------------------------------------------------------------------------
 * ��  �������캯��
 * ��  ������
 * ����ֵ����
------------------------------------------------------------------------------*/
CToolbarMenu::CToolbarMenu(HWND hParent, CPaintManagerUI* pPaintManager, INotifyUI* pNotifyReceiver)
	: m_hParent(hParent)
	, m_pPaintManager(pPaintManager)
	, m_pNotifyReceiver(pNotifyReceiver)
{
	m_szLabelNormlImg = L"img\\video\\checkbox.png";
	m_szLabelHotImg = L"img\\video\\checkbox_hot.png";
	m_szLabelPressedImg = L"img\\video\\checkbox_pressed.png";
	m_szLabelSelectedImg = L"img\\video\\checkbox_sel.png";
}

/*------------------------------------------------------------------------------
 * ��  ������������
 * ��  ������
 * ����ֵ����
------------------------------------------------------------------------------*/
CToolbarMenu::~CToolbarMenu()
{
}

/*------------------------------------------------------------------------------
 * ��  ������ʼ���˵�
 * ��  ����[in] dwMenuItemHeigh		�˵���߶�
 *         [in] dwMenuItemWidth		�˵�����
 *         [in] dwMenuLabelWidth	�˵���ǰ���ǩ���
 * ����ֵ���ɹ�/ʧ��
------------------------------------------------------------------------------*/
bool CToolbarMenu::Init(DWORD dwMenuItemHeigh, DWORD dwMenuItemWidth, DWORD dwMenuLabelWidth)
{
	if (dwMenuLabelWidth > dwMenuItemWidth)
		return false;

	m_dwMenuItemWidth = dwMenuItemWidth;
	m_dwMenuItemHeight = dwMenuItemHeigh;
	m_dwMenuLabelWidth = dwMenuLabelWidth;
	m_dwMenuTextWidth = dwMenuItemWidth - dwMenuLabelWidth;

	return true;
}

/*------------------------------------------------------------------------------
 * ��  ������Ӳ˵���
 * ��  ����[in] szName		�˵�������
 *         [in] enMenuType	MenuItemType
 *         [in] szLabelName	��ǩ����
 *         [in] szText		�˵�����ʾ���ı�
 *         [in] szGroup		��ѡ��ť��Ҫ����
 *         [in] bSelected	�Ƿ���ʾΪ��ѡ��
 * ����ֵ����
------------------------------------------------------------------------------*/
void CToolbarMenu::AddMenuItem(LPCTSTR szName, MenuItemType enMenuType, LPCTSTR szLabelName, 
	LPCTSTR szText, LPCTSTR szGroup, bool bSelected)
{
	// �˵�������ֲ����ظ�
	for (auto iter : m_menuItems)
		if (iter.szName == szName)
			return;

	MenuItem item;
	item.szName = szName;
	item.enType = enMenuType;
	item.bSelected = bSelected;
	item.szLabelGroup = szGroup;
	item.szLabelName = szLabelName;
	item.szText = szText;

	m_menuItems.push_back(item);
}

/*------------------------------------------------------------------------------
 * ��  ���������˵���
 * ��  ����[in] item �˵�������
 * ����ֵ���˵������
------------------------------------------------------------------------------*/
CMenuElementUI* CToolbarMenu::ConstructMenuItem(const MenuItem& item)
{
	WCHAR szTmp[32];

	CMenuElementUI* pElement = new CMenuElementUI;
	pElement->SetName(item.szName);
	pElement->SetAttribute(L"width", _itow(m_dwMenuItemWidth, szTmp, 10));
	pElement->SetAttribute(L"height", _itow(m_dwMenuItemHeight, szTmp, 10));

	CHorizontalLayoutUI* pHLayout = new CHorizontalLayoutUI;
	pHLayout->SetAttribute(L"inset", L"10,10,10,10");
	pHLayout->SetAttribute(L"width", _itow(m_dwMenuItemWidth, szTmp, 10));
	pHLayout->SetAttribute(L"height", _itow(m_dwMenuItemHeight, szTmp, 10));
	
	COptionUI* pSelectOpt = new COptionUI;
	pSelectOpt->SetName(item.szLabelName);
	pSelectOpt->SetAttribute(L"height", _itow(16, szTmp, 10));
	pSelectOpt->SetAttribute(L"width", _itow(16, szTmp, 10));
	if (item.enType == MENU_ITEM_TYPE_RADIO)
		pSelectOpt->SetAttribute(L"group", item.szLabelGroup);
	pSelectOpt->SetAttribute(L"normalimage", m_szLabelNormlImg);
	pSelectOpt->SetAttribute(L"hotimage", m_szLabelHotImg);
	pSelectOpt->SetAttribute(L"pushedimage", m_szLabelPressedImg);
	pSelectOpt->SetAttribute(L"selectedimage", m_szLabelSelectedImg);
	pSelectOpt->SetAttribute(L"selected", item.bSelected ? L"true" : L"false");
	pHLayout->Add(pSelectOpt);

	CLabelUI* pTextLabel = new CLabelUI;
	pTextLabel->SetAttribute(L"height", _itow(m_dwMenuItemHeight, szTmp, 10));
	pTextLabel->SetAttribute(L"width", _itow(m_dwMenuTextWidth, szTmp, 10));
	pTextLabel->SetText(item.szText);
	pTextLabel->SetAttribute(L"padding", L"8,0,0,0");
	pHLayout->Add(pTextLabel);

	pElement->Add(pHLayout);

	return pElement;
}

/*------------------------------------------------------------------------------
 * ��  �������ݴ�������ݹ����˵�
 * ��  ����[in] pMenu	�˵�����
 *         [in] items	�˵�������
 * ����ֵ����
------------------------------------------------------------------------------*/
void CToolbarMenu::ConstructMenu(CMenuWnd* pMenu, const std::list<MenuItem>& items)
{
	// FIXME: xml�����ļ��ж���������һ���յ�MenuElement��Ϊռλ��������̬��ӵ�
	// �˵����޷���ʾ�������Ǹ�bug����ʱû��ʱ��ȥ�������������Ҫ�Ƴ�ռλ��
	((CMenuUI*)pMenu->m_pm.GetRoot())->Remove(pMenu->m_pm.FindControl(L"placeholder"));

	for (auto iter : items)
	{
		CMenuElementUI* pElement = ConstructMenuItem(iter);
		if (pElement)
			((CMenuUI*)pMenu->m_pm.GetRoot())->Add(pElement);
	}
}

/*------------------------------------------------------------------------------
 * ��  ������ʾ�˵�
 * ��  ����[in] ptAlignPoint	�˵���ʾ�����
 * ����ֵ���ɹ�/ʧ��
------------------------------------------------------------------------------*/
void CToolbarMenu::ShowMenu(POINT ptAlignPoint)
{
	CMenuWnd* pMenu = new CMenuWnd(m_hParent);
	if (pMenu == NULL)
		return;

	pMenu->Init(NULL, NULL, L"toolbar_menu.xml", _T("xml"), ptAlignPoint);

	// ��̬��Ӳ˵���
	ConstructMenu(pMenu, m_menuItems);

	// ���¼���˵���С����ʾ
	pMenu->ResizeMenu();

	pMenu->m_pm.AddNotifier(m_pNotifyReceiver);
}