// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _MASK_WINDOW_H__
#define _MASK_WINDOW_H__

#include <atlgdi.h>
#include <atltypes.h>
#include <atlctrls.h>
#include <atlstr.h>
#include <atlcrack.h>

#include "MainEdit.h"
#include "XWindow.h"
#include "CoreProxy.h"
#include "maskview.h"


class CMaskWindow : public CWindowImpl<CMaskWindow,CAxWindow>
{
public:
	DECLARE_WND_SUPERCLASS(NULL,CAxWindow::GetWndClassName())

	BOOL PreTranslateMessage(MSG* pMsg);

	BEGIN_MSG_MAP(CMaskWindow)		
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestory)
		MESSAGE_HANDLER(WM_SIZE, OnSize)				
		MESSAGE_HANDLER(WM_KEYUP, OnKeyUp)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
		MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnLButtonDbClick)

		MESSAGE_HANDLER(WM_MAINEDIT_SUBMIT, OnMainEditSubmit)
	END_MSG_MAP()

	CMaskWindow();
	~CMaskWindow();
		
	void Ini(CoreProxy *pCoreProxy);

	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestory(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnMainEditSubmit(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnLButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnLButtonDbClick(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnKeyUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	HRESULT ShowMaskWindow(RECT rect, HBITMAP hBmpBackground);
	HRESULT UpdateMaskWindow();

protected:
	BOOL _IsUrl(const CString& strURL);
private:
	CString m_strBaiduSearch;
	base::CScopedRefPtr<CoreProxy> m_spCoreProxy;
	MaskView m_MaskView;
public:
	
};

#endif //_MASK_WINDOW_H__
