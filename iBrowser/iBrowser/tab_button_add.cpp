// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#include "stdafx.h"
#include "tab_button_add.h"
#include "ui_util.h"
#include "resource.h"
#include "MessageDef.h"

extern CAppModule _Module;


LRESULT CTabButtonAdd::OnPaint( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	PAINTSTRUCT paintStruct;
	::BeginPaint(m_hWnd, &paintStruct);

	RECT rc;
	GetClientRect(&rc);
	Gdiplus::Bitmap bitmap(rc.right, rc.bottom);
	Gdiplus::Graphics *mg = Gdiplus::Graphics::FromImage(&bitmap);
	Gdiplus::SolidBrush bkBrush(Gdiplus::Color(255,255,255,255));
	mg->FillRectangle(&bkBrush, rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top);
	mg->DrawImage(m_pCurrentImage,0,0,m_pCurrentImage->GetWidth(),m_pCurrentImage->GetHeight());

	Gdiplus::Graphics g(paintStruct.hdc);
	g.DrawImage(&bitmap,rc.left, rc.top, rc.right-rc.left ,rc.bottom-rc.top);

	::EndPaint(m_hWnd, &paintStruct);
	return 0;
}

CTabButtonAdd::CTabButtonAdd()
:m_hParentWindow(NULL)
,m_bMouseTrack(TRUE)
,m_pNormalImage(NULL)
,m_pHoverImage(NULL)
,m_pCurrentImage(NULL)
{

}

CTabButtonAdd::~CTabButtonAdd()
{
	if (m_pNormalImage){
		delete m_pNormalImage;
		m_pNormalImage = NULL;
	}
	if (m_pHoverImage){
		delete m_pHoverImage;
		m_pHoverImage = NULL;
	}
	m_pCurrentImage = NULL;

	if (IsWindow()){
		DestroyWindow();
		m_hWnd = NULL;
	}
}

void CTabButtonAdd::SetParentHWND( HWND hParent )
{
	m_hParentWindow = hParent;
}

LRESULT CTabButtonAdd::OnCreate( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	ImageFromIDResource(_Module.m_hInstResource, IDR_TAB_BUTTON_ADD, L"PNG", m_pNormalImage);
	ImageFromIDResource(_Module.m_hInstResource, IDR_TAB_BUTTON_ADD_OVER, L"PNG", m_pHoverImage);
	m_pCurrentImage = m_pNormalImage;
	return 0;
}

LRESULT CTabButtonAdd::OnLButtonUp( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	::PostMessage(m_hParentWindow, WM_CLICK_ADD_TAB, (WPARAM)m_hWnd, 0);
	return 0;
}

LRESULT CTabButtonAdd::OnMouseHover( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	m_pCurrentImage = m_pHoverImage;
	RedrawWindow();
	return 0;
}

LRESULT CTabButtonAdd::OnMouseLeave( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	m_pCurrentImage = m_pHoverImage;
	RedrawWindow();
	m_bMouseTrack = TRUE;
	return 0;
}

LRESULT CTabButtonAdd::OnMouseMove( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if (m_bMouseTrack)
	{
		TRACKMOUSEEVENT csTME;
		csTME.cbSize = sizeof(csTME);
		csTME.dwFlags = TME_LEAVE|TME_HOVER;
		csTME.hwndTrack = m_hWnd;
		csTME.dwHoverTime = 10;  
		::_TrackMouseEvent(&csTME); 
		m_bMouseTrack=FALSE;
	}
	return 0;
}

LRESULT CTabButtonAdd::OnEraseBKGnd( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	return 0;
}
