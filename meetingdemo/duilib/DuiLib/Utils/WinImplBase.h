#ifndef WIN_IMPL_BASE_HPP
#define WIN_IMPL_BASE_HPP

namespace DuiLib
{

	enum UILIB_RESOURCETYPE
	{
		UILIB_FILE=1,		// ���Դ����ļ�
		UILIB_ZIP,			// ���Դ���zipѹ����
		UILIB_RESOURCE,		// ������Դ
		UILIB_ZIPRESOURCE,	// ������Դ��zipѹ����
	};

	class DUILIB_API WindowImplBase
		: public CWindowWnd
		, public CNotifyPump
		, public INotifyUI
		, public IMessageFilterUI
		, public IDialogBuilderCallback
	{
	public:
		WindowImplBase(){};
		virtual ~WindowImplBase(){};

		// OnCreate�����л����
		virtual void InitWindow(){};

		// CWindowWnd���麯����WM_DESTROY��Ϣ֪ͨ����
		virtual void OnFinalMessage( HWND hWnd );

		// INotifyUI����
		virtual void Notify(TNotifyUI& msg);

		// ��Ϣӳ��
		DUI_DECLARE_MESSAGE_MAP()
		//	private:                                                                  
		//	static const DUI_MSGMAP_ENTRY _messageEntries[];                      
		//	protected:                                                                
		//	static const DUI_MSGMAP messageMap;                                   
		//	static const DUI_MSGMAP* PASCAL _GetBaseMessageMap();                 
		// virtual const DUI_MSGMAP* GetMessageMap() const;                      

	protected:
		// ÿ�����ڶ���һ��xml�����ļ���ʵ�����·���ָ��xml�����ļ�
		virtual CDuiString GetSkinFolder() = 0;
		virtual CDuiString GetSkinFile() = 0;

		// CWindowWnd���麯��
		virtual LPCTSTR GetWindowClassName(void) const = 0 ;

		// TODO:
		virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);

		// ������Ƶ�Manager
		CPaintManagerUI m_PaintManager;

		// zip��ʽ��Դ�ļ�������
		static LPBYTE m_lpResourceZIPBuffer;

	public:
		// CWindowWnd���麯��
		virtual UINT GetClassStyle() const;

		// ָ����Դ���ͣ�����鿴UILIB_RESOURCETYPE����
		virtual UILIB_RESOURCETYPE GetResourceType() const;

		// �����Դ��zip�ļ�����ָ��zip�ļ���
		virtual CDuiString GetZIPFileName() const;

		// �������windows��Դ��ʽ������Ҫָ����ԴID
		virtual LPCTSTR GetResourceID() const;

		// IDialogBuilderCallback�ӿڷ�����DialogBuilder��ص��˷��������ؼ�
		virtual CControlUI* CreateControl(LPCTSTR pstrClass);

		// IMessageFilterUI�ӿڷ���
		virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM /*lParam*/, bool& /*bHandled*/);

		// WinImplBase�Ѿ�����ķ���
		virtual void OnClick(TNotifyUI& msg);
		virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

		// WinImplBaseû�д������������д�ķ���
		virtual LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
		virtual LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
		virtual LRESULT OnNcActivate(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled);
		virtual LRESULT OnNcPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		virtual LRESULT OnMouseWheel(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
		virtual LRESULT OnMouseHover(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT OnChar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		virtual LRESULT OnKeyDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
		virtual LRESULT OnKillFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
		virtual LRESULT OnSetFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
		virtual LRESULT OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
		virtual LRESULT OnLButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
		virtual LRESULT OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);

		// ��д��CWindowWnd��HandleMessage�������Զ��崰����Ϣ����
		virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

		// ���������д�˷����Դ����Զ�����Ϣ
		virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

		virtual LONG GetStyle();
	};
}

#endif // WIN_IMPL_BASE_HPP
