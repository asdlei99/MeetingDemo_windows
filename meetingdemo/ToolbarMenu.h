/*##############################################################################
 * �ļ���ToolbarMenu.h
 * �������ײ��������˵���������붨��
 * ���ߣ�Teck
 * ʱ�䣺2018��5��24��
 * ��Ȩ��Copyright(C) 2018 Fsmeeting.com. All rights reserved.
 ##############################################################################*/
#pragma once

#include <list>
#include <memory>
#include "UIlib.h"
#include "UIMenu.h"

using namespace DuiLib;

////////////////////////////////////////////////////////////////////////////////
// Menu Item Format:
// | Select Label | Menu Text | Sub-Menu Label
////////////////////////////////////////////////////////////////////////////////
class CToolbarMenu
{
public:
	enum MenuItemType
	{
		MENU_ITEM_TYPE_NONE,	// ����
		MENU_ITEM_TYPE_RADIO,	// ��ѡ��
		MENU_ITEM_TYPE_CHECKBOX	// ��ѡ��
	};

public:
	CToolbarMenu(HWND hParent, 
		CPaintManagerUI* pPaintManager, 
		INotifyUI* pNotifyReceiver);

	~CToolbarMenu();

	bool Init(DWORD dwMenuItemHeigh, 
		DWORD dwMenuItemWidth, 
		DWORD dwMenuLabelWidth);

	void AddMenuItem(LPCTSTR szName, 
		MenuItemType enMenuType,
		LPCTSTR szLabelName,
		LPCTSTR szText,
		LPCTSTR szGroup,
		bool bSelected);

	void ShowMenu(POINT ptAlignPoint);

private:
	struct Menu;
	typedef std::shared_ptr<Menu> MenuSP;

	struct MenuItem
	{
		CDuiString		szName;
		CDuiString		szLabelName;
		CDuiString		szLabelGroup;
		CDuiString		szText;
		bool			bSelected;
		MenuItemType	enType;
	};

private:
	DWORD GetTopMenuHeight();
	MenuItem* FindMenuItem(Menu* pMenu, LPCTSTR szMenuItemName);
	void AddMenuItem(Menu* pMenu, LPCTSTR szMenuItemName, MenuItemType enMenuType, bool hasSubMenu);
	void ConstructMenu(CMenuElementUI* pParent, const std::list<MenuItem>& items);
	void ConstructMenu(CMenuWnd* pMenu, const std::list<MenuItem>& items);
	CMenuElementUI* ConstructMenuItem(const MenuItem& item);

private:
	HWND m_hParent;
	INotifyUI* m_pNotifyReceiver;
	CPaintManagerUI* m_pPaintManager;

	DWORD m_dwMenuItemWidth;
	DWORD m_dwMenuItemHeight;
	DWORD m_dwMenuLabelWidth;
	DWORD m_dwMenuTextWidth;
	CDuiString	m_szLabelNormlImg;
	CDuiString	m_szLabelHotImg;
	CDuiString	m_szLabelPressedImg;
	CDuiString	m_szLabelSelectedImg;
	std::list<MenuItem> m_menuItems;
};

