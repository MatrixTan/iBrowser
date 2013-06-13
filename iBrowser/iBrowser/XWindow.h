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
#include <Base\RefCounted.h>

class CoreProxy;

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
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestory)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_CHILD_WINDOW_CREATED, OnChildWindowCreated);
		MESSAGE_HANDLER(WM_CORE_PROCESS_HOST_READY, OnCoreProcessHostReady);
	END_MSG_MAP()

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestory(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnTimer(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnChildWindowCreated(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnCoreProcessHostReady(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	CXWindow();
	~CXWindow();
	void Initialize(E_CHILEWINDOW_CREATE_FLAG flag, const CString& strURL);

	static const DWORD kNormalStyle = WS_CHILDWINDOW|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|0x4c;
	static const DWORD kFreezingStyle = WS_POPUP|WS_CLIPSIBLINGS|WS_DISABLED;

	static const DWORD kNormalExStyle = WS_EX_LEFT|WS_EX_LTRREADING|WS_EX_RIGHTSCROLLBAR|WS_EX_TOOLWINDOW|WS_EX_NOPARENTNOTIFY;
	static const DWORD kFreezingExStyle = kNormalExStyle;

private:
	clock_t m_nLastHeartBeatClock;
	HWND m_hChildWindow;
	HWND m_hParentWindow;
	//HWND m_hTabButton;
	BOOL m_bFreezing;
	CString m_strURL;
	UINT m_nCreateFlag;
	base::CScopedRefPtr<CoreProxy> m_spCoreProxy;	
};

#endif