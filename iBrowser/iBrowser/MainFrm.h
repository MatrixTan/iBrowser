// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _MAIN_FRM_H__
#define _MAIN_FRM_H__

#include <atlframe.h>
#include <atlapp.h>
#include <map>
#include <vector>

#include "resource.h"
#include "core_view.h"
#include "MaskWindow.h"
#include "tab_button.h"
#include "tab_button_add.h"
#include "core_proxy.h"

class CMainFrame : 
	public CFrameWindowImpl<CMainFrame>, 
	public CUpdateUI<CMainFrame>,
	public CMessageFilter, 
	public CIdleHandler
{
public:
	DECLARE_FRAME_WND_CLASS(NULL, IDR_MAINFRAME)

	BEGIN_UPDATE_UI_MAP(CMainFrame)
	END_UPDATE_UI_MAP()

	static const DWORD kStyle = WS_CAPTION|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_SYSMENU|WS_THICKFRAME|WS_OVERLAPPED|WS_MINIMIZEBOX|WS_MAXIMIZEBOX;
	static const DWORD kExStyle = WS_EX_LEFT|WS_EX_LTRREADING|WS_EX_RIGHTSCROLLBAR|WS_EX_WINDOWEDGE|WS_EX_APPWINDOW;

	BEGIN_MSG_MAP(CMainFrame)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_EVENT_NOTIFY, OnEventNotify)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_MOVE, OnMove)
		MESSAGE_HANDLER(WM_IE_FORTEST, OnForTest)
		MESSAGE_HANDLER(WM_SHOW_OPERATION_PANEL, OnShowOprationPanel)
		MESSAGE_HANDLER(WM_CHILD_WINDOW_CREATED, OnChildCreated)
		MESSAGE_HANDLER(WM_SYSCOMMAND, OnSysCommand)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		MESSAGE_HANDLER(WM_CLICK_ADD_TAB, OnAddTab)
		MESSAGE_HANDLER(WM_CLICK_TAB, OnClickTab)
		MESSAGE_HANDLER(WM_CLOSE_TAB, OnCloseTab)
		MESSAGE_HANDLER(WM_HOTKEY_NOTIFY, OnNotifyHotKey)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		MESSAGE_HANDLER(WM_IME_SETCONTEXT, OnSetContext)
		MESSAGE_HANDLER(WM_COMMAND, OnCommand)
		CHAIN_MSG_MAP(CUpdateUI<CMainFrame>)
		CHAIN_MSG_MAP(CFrameWindowImpl<CMainFrame>)
		//CHAIN_MSG_MAP_MEMBER(m_wndMaskWindow)
	END_MSG_MAP()

	LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnEventNotify(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnMove(UINT uMsg, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnForTest(UINT uMsg, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnShowOprationPanel(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnChildCreated(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);	
	LRESULT OnSysCommand(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCommand(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnAddTab(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnClickTab(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCloseTab(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnNotifyHotKey(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnTimer(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnSetContext(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	
	CMainFrame();
	~CMainFrame();
	
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();

	void Init();

	class TabPair
	{
	public:
		CoreProxy *spCoreProxy; //由创建者View销毁
		int nTabIndex;
		base::CScopedRefPtr<CXWindow> spContainerWindow;
		TabPair()
			:spCoreProxy(NULL)
			,nTabIndex(-1)
		{}

		~TabPair()
		{

		}
	};

protected:
	void _AddNewTab(E_CHILEWINDOW_CREATE_FLAG flag , const CString& strUrl);
	void _CreateContainer(E_CHILEWINDOW_CREATE_FLAG flag, const CString& strURL);

	void _ShowCurrentContainer(void);

	HRESULT _ShowOprationPanel(void);
	void _SwitchTo(HWND hTab);
	
	
private:
	CMaskWindow m_wndMaskWindow;
	CTabButtonAdd m_TabButtonAdd;
	int m_nTabButtonWidth;
	typedef std::vector<base::CScopedRefPtr<TabButton> > TabButtonVector;
	TabButtonVector m_vTabButtons;
	typedef TabButtonVector::iterator TabButtonIter;

	BOOL m_bShowMask;
	HWND m_hCurrentTabButton;

	typedef std::map<HWND, TabPair> TabPairMap;
	TabPairMap m_mapTabPairs;
	typedef std::pair<HWND, TabPair> TabPairPair;

	CString m_strHomeURL;
};

#endif //_MAIN_FRM_H__
