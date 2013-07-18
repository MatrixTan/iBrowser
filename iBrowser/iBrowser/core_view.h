// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _BROWSER_VIEW_H__
#define _BROWSER_VIEW_H__


#include <atlcrack.h>

#include "IIEEventDelegate.h"
#include "ConsoleWindow.h"
#include "MessageDef.h"
#include "core_proxy.h"
#include <Base/ipc_message.h>
#include "mouse_gesture.h"
#include "host_proxy.h"


class CoreView : public CWindowImpl<CoreView,CAxWindow>
	,public CMessageFilter
	,public CIdleHandler
{
public:
	DECLARE_WND_SUPERCLASS(NULL,CAxWindow::GetWndClassName())

	static const DWORD kStyle = WS_CHILDWINDOW|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN;
	static const DWORD kExStyle = WS_EX_LEFT|WS_EX_LTRREADING|WS_EX_RIGHTSCROLLBAR|WS_EX_TOOLWINDOW;

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();

	BEGIN_MSG_MAP(CoreView)
		HANDLE_IPC_MSG(CoreView)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestory)
		MESSAGE_HANDLER(WM_SIZE, OnSize)

		MESSAGE_HANDLER(WM_SHOW_CONSOLE_WINDOW, OnShowConsole)
		MESSAGE_HANDLER(WM_EVENT_DELEGATE_MESSAGE, OnEventDelegateMessage);
		MESSAGE_HANDLER(WM_CORE_NAVIGATE, OnCoreNavigate);
		MESSAGE_HANDLER(WM_CORE_REFRESH, OnCoreRefresh);
		MESSAGE_HANDLER(WM_CORE_GOBACK, OnGoBack);
		MESSAGE_HANDLER(WM_CORE_GOFORWARD, OnGoForward);
		MESSAGE_HANDLER(WM_CORE_DESTROY, OnCoreDestroy);
		MESSAGE_HANDLER(WM_CORE_FOCUS, OnCoreFocus);

		MESSAGE_HANDLER(WM_IE_FORTEST, OnForTest);
		MESSAGE_HANDLER(WM_MOVE, OnMove)
	END_MSG_MAP()

	CoreView();
	~CoreView();

	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestory(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnShowConsole(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnEventDelegateMessage(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCoreInvoke(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCoreNavigate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCoreRefresh(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnGoBack(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnGoForward(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCoreDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnForTest(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCoreFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	void Initialize(E_CHILEWINDOW_CREATE_FLAG flag, const CString& strURL);
	void NotifyHotKey(UINT uMsg, WPARAM wParam, LPARAM lParam);
protected:
	HRESULT _Navegate(LPCWSTR strURL);
	HRESULT _Refresh(void);
	HRESULT _GoBack(void);
	HRESULT _GoForward(void);
	HRESULT _Close(void);
	HRESULT _ShowMaskWindow(void);	

private:
	friend class MouseGesture;
	CComPtr<IWebBrowser2> m_spWebBrowser2;
	CComPtr<IIEEventDelegate> m_spIEEventDelegate;
	CConsoleWindow m_wndConsole;
	UINT m_nCreateFlag;
	CString m_strURL;
	MouseGesture m_MouseGesture;
	bool m_bBeforeGesture;
	HostProxy *m_HostProxy;

	HRESULT _CreateCoreServer(void);
	CComPtr<IOleObject>             m_spOleObject ;
	CComPtr<IOleInPlaceObjectWindowless> m_spWindowless ;

public:
	LRESULT OnMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};


#endif //_BROWSER_VIEW_H__