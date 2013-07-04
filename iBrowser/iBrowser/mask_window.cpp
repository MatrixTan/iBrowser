// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "stdafx.h"
#include "mask_window.h"
#include "resource.h"
#include "url_util.h"
#include "global_singleton.h"


HMODULE gRichEditDll = ::LoadLibrary(CRichEditCtrl::GetLibraryName());

MaskWindow::MaskWindow()
{

}

MaskWindow::~MaskWindow()
{

}

void MaskWindow::Ini( CoreProxy *pCoreProxy )
{
	m_spCoreProxy = pCoreProxy;
	GlobalSingleton::GetInstance()->SetMaskHWND(m_hWnd);
}


BOOL MaskWindow::PreTranslateMessage( MSG* pMsg )
{
	if (m_MaskView.IsWindow()){
		return m_MaskView.PreTranslateMessage(pMsg);
	}
	return FALSE;
}

LRESULT MaskWindow::OnPaint( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	CRect rect;
	GetWindowRect(&rect);
	rect.MoveToXY(0,0);
	PAINTSTRUCT paint;
	BeginPaint(&paint);
	CDC dcWindow(paint.hdc);

	dcWindow.FillSolidRect(rect, RGB(0, 0, 0));
	EndPaint(&paint);
	return 0;
}

LRESULT MaskWindow::OnCreate( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	CRect maskViewRect(0, 0, 200, 600);
	GetWindowRect(&maskViewRect);
	m_MaskView.Create(m_hWnd, maskViewRect, _T("{8856F961-340A-11D0-A96B-00C04FD705A2}"), MaskView::kStyle , MaskView::kExStyle);

	m_strBaiduSearch.LoadString(IDS_BAIDU_SEARCH);

	return 0;
}

LRESULT MaskWindow::OnDestory( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	return 0;
}

HRESULT MaskWindow::ShowMaskWindow( RECT rect, HBITMAP hBmpBackGround)
{
	if (IsWindow()){
		SetWindowPos(HWND_TOP,&rect,SWP_SHOWWINDOW);
		m_MaskView.OnShow();
	}
	return S_OK;
}

LRESULT MaskWindow::OnSize( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	CRect rect;
	GetClientRect(&rect);
	m_MaskView.SetWindowPos(HWND_TOP,&rect, SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE);
	return 0;
}

LRESULT MaskWindow::OnMainEditSubmit( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled )
{
	BSTR bsURL = (BSTR)lParam;
	CString strText(bsURL);
	ShowWindow(SW_HIDE);
	if (m_spCoreProxy.get())
	{
		if (true)
		{

		}
		else
		{
			CString strURL;
			strURL.Format(m_strBaiduSearch,strText);
			strText = strURL;				
		}
		m_spCoreProxy->Navigate(strText);
	}
	::SysFreeString(bsURL);
	return 0;
}

BOOL MaskWindow::_IsUrl( const CString& strURL )
{
	return TRUE;	
}


LRESULT MaskWindow::OnKeyUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	TCHAR vk = (TCHAR)wParam;
	UINT cRepeat = (UINT)LOWORD(lParam);
	UINT flags = (UINT)HIWORD(lParam);
	bHandled = FALSE;
	return 0;
}


LRESULT MaskWindow::OnLButtonDown( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/ )
{
	return 0;
}

LRESULT MaskWindow::OnLButtonUp( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	bHandled = FALSE;
	return 0;
}

LRESULT MaskWindow::OnLButtonDbClick( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	return 0;
}
