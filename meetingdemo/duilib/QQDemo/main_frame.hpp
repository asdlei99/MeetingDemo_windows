#ifndef MAINFRAME_HPP
#define MAINFRAME_HPP

#include "skin_change_event.hpp"
#include "UIFriends.hpp"

class MainFrame : public WindowImplBase
{
public:

	MainFrame();
	~MainFrame();

public:

	// ����������ƣ�������WindowWnd��ע��
	LPCTSTR GetWindowClassName() const;	

	// ��������ǰ�����һ����Ϣ֪ͨ���յ�����Destroy��Ϣ��
	virtual void OnFinalMessage(HWND hWnd);

	// WindowImplBase���麯����OnCreateʱ����
	virtual void InitWindow();

	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);

	// WindowImplBase���麯������ͨ���ļ����ز���ʱʹ��
	virtual CDuiString GetSkinFile();
	virtual CDuiString GetSkinFolder();
	virtual UILIB_RESOURCETYPE GetResourceType() const;

	// WindowImplBase�̳���IDialogBuilderCallback�ӿڵķ���
	virtual CControlUI* CreateControl(LPCTSTR pstrClass);


	virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LPCTSTR GetResourceID() const;

    DWORD GetBkColor();
    void SetBkColor(DWORD dwBackColor);

protected:	

	void Notify(TNotifyUI& msg);
	void OnPrepare(TNotifyUI& msg);
	void OnExit(TNotifyUI& msg);
	void OnTimer(TNotifyUI& msg);

private:

	void UpdateFriendsList();

	void UpdateGroupsList();

	void UpdateMicroBlogList();

private:
	int bk_image_index_;

	FriendListItemInfo myself_info_;
	std::vector<FriendListItemInfo> friends_;

	SkinChangedObserver skin_changed_observer_;
};

#endif // MAINFRAME_HPP