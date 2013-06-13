// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef _MASK_VIEW_H__
#define _MASK_VIEW_H__


class MaskView : public CWindowImpl<MaskView,CAxWindow>
{
public:
	DECLARE_WND_SUPERCLASS(NULL,CAxWindow::GetWndClassName())

	static const DWORD kStyle = WS_CHILDWINDOW|WS_VISIBLE|WS_CLIPSIBLINGS;
	static const DWORD kExStyle = WS_EX_LEFT|WS_EX_LTRREADING|WS_EX_RIGHTSCROLLBAR|WS_EX_TOOLWINDOW;

	BEGIN_MSG_MAP(MaskView)		
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestory)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp);
		MESSAGE_HANDLER(WM_TIMER, OnTimer);
	END_MSG_MAP()

	MaskView();
	~MaskView();

	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestory(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnLButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnTimer(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
protected:

private:
	CComPtr<IWebBrowser2> m_spWebBrowser2;
};

#endif