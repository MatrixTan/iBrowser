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

	RECT rc;
	GetClientRect(&rc);
	
	Gdiplus::Bitmap tempBitMap(rc.right, rc.bottom);
	Gdiplus::Graphics *tempMg = Gdiplus::Graphics::FromImage(&tempBitMap);
	tempMg->DrawImage(m_pCurrentImage,0,0,m_pCurrentImage->GetWidth(),m_pCurrentImage->GetHeight());
	for (int iRow = 0; iRow < tempBitMap.GetHeight(); ++iRow){
		for (int iCollumn = 0; iCollumn<tempBitMap.GetWidth(); ++iCollumn){
			Gdiplus::Color srcColor;
			tempBitMap.GetPixel(iCollumn, iRow, &srcColor);
			if (srcColor.GetAlpha() >200){
				Gdiplus::Color destColor(255, srcColor.GetR()*(1 - m_fMaskAlpha) + m_colorMask.GetR()*m_fMaskAlpha
					, srcColor.GetG()*(1 - m_fMaskAlpha) + m_colorMask.GetG()*m_fMaskAlpha
					, srcColor.GetB()*(1 - m_fMaskAlpha) + m_colorMask.GetB()*m_fMaskAlpha);
				tempBitMap.SetPixel(iCollumn, iRow, destColor);
			}
		}
	}
	Gdiplus::Bitmap bitmap(rc.right, rc.bottom);
	Gdiplus::Graphics *mg = Gdiplus::Graphics::FromImage(&bitmap);
	Gdiplus::SolidBrush bkBrush(Gdiplus::Color(255,255,255,255));
	mg->FillRectangle(&bkBrush, rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top);
	mg->DrawImage(&tempBitMap, 0, 0, tempBitMap.GetWidth(), tempBitMap.GetHeight());

	Gdiplus::Graphics g(paintStruct.hdc);
	g.DrawImage(&bitmap,rc.left, rc.top, rc.right-rc.left ,rc.bottom-rc.top);

	::EndPaint(m_hWnd, &paintStruct);
	return 0;
}

CTabButton::CTabButton()
:m_hParentWindow(NULL)
,m_bMouseTrack(TRUE)
,m_pNormalImage(NULL)
,m_pCurrentImage(NULL)
,m_colorMask(0,0,0,0)
,m_fMaskAlpha(0.0f)
{
	
}

CTabButton::~CTabButton()
{
	if (IsWindow()){
		DestroyWindow();
		m_hWnd = NULL;
	}
	if (m_pNormalImage){
		delete m_pNormalImage;
		m_pNormalImage = NULL;
	}
	if (m_pHoverImage){
		delete m_pHoverImage;
		m_pHoverImage;
	}
	m_pCurrentImage = NULL;
}

void CTabButton::SetParentHWND( HWND hParent )
{
	m_hParentWindow = hParent;
}

LRESULT CTabButton::OnCreate( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	ImageFromIDResource(_Module.m_hInstResource, IDR_TAB_BUTTON, L"PNG", m_pNormalImage);
	ImageFromIDResource(_Module.m_hInstResource, IDR_TAB_BUTTON_OVER, L"PNG", m_pHoverImage);
	m_pCurrentImage = m_pNormalImage;
	ATLASSERT(m_pNormalImage && m_pHoverImage && m_pCurrentImage);
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
	m_pCurrentImage = m_pHoverImage;
	RedrawWindow();
	return 0;
}

LRESULT CTabButton::OnMouseLeave( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	m_pCurrentImage = m_pNormalImage;
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

LRESULT CTabButton::OnEraseBKGnd( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	return 0;
}

void CTabButton::SetMaskColor( DWORD dwColor)
{
	m_colorMask = Gdiplus::Color(dwColor);
	m_fMaskAlpha = 0.5f;
	RedrawWindow();
}
