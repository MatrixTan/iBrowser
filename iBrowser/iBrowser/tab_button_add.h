// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _TAB_BUTTON_ADD_H__
#define _TAB_BUTTON_ADD_H__

#include <GdiPlus.h>

class CTabButtonAdd : public CWindowImpl<CTabButtonAdd, CAxWindow>
{
public:
	DECLARE_WND_SUPERCLASS(NULL,CAxWindow::GetWndClassName())

	const static DWORD kStyle = WS_VISIBLE|WS_CHILDWINDOW;
	const static DWORD kExStyle = WS_EX_LEFT|WS_EX_LTRREADING|WS_EX_RIGHTSCROLLBAR;

	BOOL PreTranslateMessage(MSG* pMsg);

	BEGIN_MSG_MAP(CTabButtonAdd)		
		MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_MOUSEHOVER, OnMouseHover)
		MESSAGE_HANDLER(WM_MOUSELEAVE, OnMouseLeave)
		MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBKGnd)
	END_MSG_MAP()

	CTabButtonAdd();
	~CTabButtonAdd();

	const static int kDefaultWidth = 33;
	const static int kDefaultHeight = 25;

	void SetParentHWND(HWND hParent);
protected:
	LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMouseHover(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnEraseBKGnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
private:

	HWND m_hParentWindow;
	BOOL m_bMouseTrack;
	Gdiplus::Image *m_pNormalImage;
	Gdiplus::Image *m_pHoverImage;
	Gdiplus::Image *m_pCurrentImage;
};

#endif //_TAB_BUTTON_ADD_H__