// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _CONTAINER_WINDOW_H__
#define _CONTAINER_WINDOW_H__

#include <atlctrls.h>
#include <atltypes.h>
#include <atlwin.h>
#include <atlstr.h>
#include <atlgdi.h>

#include "MessageDef.h"
#include <time.h>
#include <Base/RefCounted.h>
#include "Base/ipc_message.h"

#define HANDLE_MSG_FOR_CORE \
	bHandled = TRUE; \
	bHandled = PostToCoreForCPR(uMsg, wParam, lParam);\
	if(bHandled) \
	return TRUE;\


class CoreProxy;
class TabButton;

class CXWindow :
	public base::RefCounted<CXWindow>,
	public CWindowImpl<CXWindow,CAxWindow>,
	public CMessageFilter,
	public CIdleHandler	
{
public:
	DECLARE_WND_SUPERCLASS(NULL, CAxWindow::GetWndClassName())
	
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();
	

	BEGIN_MSG_MAP(CXWindow)
		HANDLE_IPC_MSG(CXWindow)
		HANDLE_MSG_FOR_CORE
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestory)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBKGnd)
		MESSAGE_HANDLER(WM_CHILD_WINDOW_CREATED, OnChildWindowCreated);
		MESSAGE_HANDLER(WM_CORE_PROCESS_HOST_READY, OnCoreProcessHostReady);
		MESSAGE_HANDLER(WM_BEFORE_NAVIGATE, OnBeforeNavigate);
		MESSAGE_HANDLER(WM_TITLE_CHANGE, OnTitleChange);
		MESSAGE_HANDLER(WM_NAVIGATE_COMPLETE, OnNavigateComplete)
		MESSAGE_HANDLER(WM_CORE_DESTROYED, OnCoreDestroyed)
		MESSAGE_HANDLER(WM_CORE_WINDOW_CREATED, OnCoreWindowCreated)
		MESSAGE_HANDLER(WM_RENDER_BACK_STORE, OnRenderBackStore)
	END_MSG_MAP()

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestory(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnTimer(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnEraseBKGnd(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnChildWindowCreated(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnCoreProcessHostReady(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnBeforeNavigate(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnTitleChange(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNavigateComplete(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCoreDestroyed(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCoreWindowCreated(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnRenderBackStore(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	BOOL PostToCoreForCPR(UINT msg, WPARAM wParam, LPARAM lParam);

	CXWindow();
	~CXWindow();
	void Initialize(E_CHILEWINDOW_CREATE_FLAG flag, const CString& strURL
		, TabButton *pTabButton);
	void Uninitialize(void);
	const CStringW& GetTitle(void) const;
	const CStringW& GetURL(void) const;
	BOOL ShowWindow(int nCmd);
	void Focus(void);
	bool AddCurrentBookmark(void);

	static const DWORD kNormalStyle = WS_CHILDWINDOW|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|0x4c;
	static const DWORD kFreezingStyle = WS_POPUP|WS_CLIPSIBLINGS|WS_DISABLED;

	static const DWORD kNormalExStyle = WS_EX_LEFT|WS_EX_LTRREADING|WS_EX_RIGHTSCROLLBAR|WS_EX_TOOLWINDOW|WS_EX_NOPARENTNOTIFY;
	static const DWORD kFreezingExStyle = kNormalExStyle;

private:

	void ShowURL(void)const;
	void ShowTitle(void)const;
	void ReadStrFromIPC(void *pData, CStringW &str)const;
	void UpdateTabColor(void);

	HWND m_hCoreViewWindow;
	HWND m_hParentWindow;
	HWND m_hCoreWindow;
	//HWND m_hTabButton;
	BOOL m_bFreezing;
	CString m_strURL;
	UINT m_nCreateFlag;
	CStringW m_strTitle;
	base::CScopedRefPtr<CoreProxy> m_spCoreProxy;
	base::CScopedRefPtr<TabButton> m_spTabButton;
};

#endif