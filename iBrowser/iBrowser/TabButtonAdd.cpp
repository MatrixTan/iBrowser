// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#include "stdafx.h"
#include "TabButtonAdd.h"
#include "UIUtil.h"
#include "resource.h"
#include "MessageDef.h"

extern CAppModule _Module;


LRESULT CTabButtonAdd::OnPaint( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
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

CTabButtonAdd::CTabButtonAdd()
:m_hParentWindow(NULL)
,m_bMouseTrack(TRUE)
,m_nCurrentImageId(IDR_TAB_BUTTON_ADD)
{

}

CTabButtonAdd::~CTabButtonAdd()
{

}

void CTabButtonAdd::SetParentHWND( HWND hParent )
{
	m_hParentWindow = hParent;
}

LRESULT CTabButtonAdd::OnCreate( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	return 0;
}

LRESULT CTabButtonAdd::OnLButtonUp( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	::PostMessage(m_hParentWindow, WM_CLICK_ADD_TAB, (WPARAM)m_hWnd, 0);
	return 0;
}

LRESULT CTabButtonAdd::OnMouseHover( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	m_nCurrentImageId = IDR_TAB_BUTTON_ADD_OVER;
	RedrawWindow();
	return 0;
}

LRESULT CTabButtonAdd::OnMouseLeave( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	m_nCurrentImageId = IDR_TAB_BUTTON_ADD;
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
