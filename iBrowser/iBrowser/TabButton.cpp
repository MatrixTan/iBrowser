// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "stdafx.h"
#include "TabButton.h"
#include "UIUtil.h"
#include "resource.h"
#include "MessageDef.h"

extern CAppModule _Module;


LRESULT CTabButton::OnPaint( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	PAINTSTRUCT paintStruct;
	::BeginPaint(m_hWnd, &paintStruct);
	
	Gdiplus::Image *pImage = NULL;
	ImageFromIDResource(_Module.m_hInstResource, m_nCurrentImageId, L"PNG", pImage);

	Gdiplus::Graphics g(paintStruct.hdc);
	g.DrawImage(pImage,0,0,pImage->GetWidth(),pImage->GetHeight());
	delete pImage;

	::EndPaint(m_hWnd, &paintStruct);
	return 0;
}

CTabButton::CTabButton()
:m_hParentWindow(NULL)
,m_nCurrentImageId(IDR_TAB_BUTTON)
,m_bMouseTrack(TRUE)
{
	
}

CTabButton::~CTabButton()
{
	if (IsWindow())
	{
		DestroyWindow();
	}	
}

void CTabButton::SetParentHWND( HWND hParent )
{
	m_hParentWindow = hParent;
}

LRESULT CTabButton::OnCreate( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	return 0;
}

LRESULT CTabButton::OnLButtonUp( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	::PostMessage(m_hParentWindow, WM_CLICK_TAB, (WPARAM)m_hWnd, NULL);
	return 0;
}

LRESULT CTabButton::OnRButtonUp( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	::PostMessage(m_hParentWindow, WM_CLOSE_TAB, (WPARAM)m_hWnd, NULL);
	return 0;
}

LRESULT CTabButton::OnMouseHover( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	m_nCurrentImageId = IDR_TAB_BUTTON_OVER;
	RedrawWindow();
	return 0;
}

LRESULT CTabButton::OnMouseLeave( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	m_nCurrentImageId = IDR_TAB_BUTTON;
	RedrawWindow();
	m_bMouseTrack = TRUE;
	return 0;
}

LRESULT CTabButton::OnMouseMove( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
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
