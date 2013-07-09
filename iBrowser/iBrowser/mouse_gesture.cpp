// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "stdafx.h"
#include "mouse_gesture.h"
#include "core_view.h"
#include <GdiPlus.h>

MouseGesture::MouseGesture(CoreView* pCore)
	:m_pCore(pCore)
	,m_bStarted(false)
	,m_bMouseTrack(false)
	,m_ptLast(-1, -1)
	,m_ptLastGesture(-1, -1)
	,m_LastDirection(ED_None)
{
	RECT rc = {0};
	Create(NULL, rc, L"Mouse Gesture", WS_CLIPCHILDREN|WS_CLIPSIBLINGS|WS_POPUP, WS_EX_TOOLWINDOW|WS_EX_NOACTIVATE);
}

MouseGesture::~MouseGesture()
{

}

void MouseGesture::Start()
{
	RECT rc = {0};
	HWND hParent = m_pCore->GetParent().m_hWnd;
	SetParent(hParent);
	m_pCore->GetClientRect(&rc);
	SetWindowPos(NULL, &rc, SWP_NOMOVE|SWP_NOACTIVATE);
	ShowWindow(SW_SHOW);
	m_bStarted = true;
	ZeroMemory(&m_Directions, sizeof(BYTE)*kDirectionCount);
	m_LastDirection = ED_None;
	m_ptLast.SetPoint(-1, -1);
	m_ptLastGesture.SetPoint(-1, -1);

	while(true){
		MSG msg;
		if (0 == ::PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE)){
			::MsgWaitForMultipleObjects(0, NULL, FALSE, INFINITE, QS_ALLINPUT|QS_ALLEVENTS);
			continue;
		}
		if (WM_QUIT == msg.message || (msg.message > WM_KEYFIRST && msg.message < WM_KEYLAST)){
			Stop();
			break;
		}
		
		BOOL bRet = ::GetMessage(&msg, NULL, NULL, NULL);
		if (bRet == 0 || bRet == -1){
			Stop();
			break;
		}
		if (msg.message == WM_MOUSELEAVE 
			|| msg.message == WM_RBUTTONUP){
			Stop();
			m_bMouseTrack = false;
			break;;
		}else if (msg.message == WM_PAINT){
			//continue;
		}else if (msg.message == WM_TIMER){
			continue;
		}else{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}
}

void MouseGesture::Stop()
{
	ShowWindow(SW_HIDE);
	m_pCore->UpdateWindow();
	m_bStarted = false;
	m_Tip.Hide();
	DoCommand();
}

bool MouseGesture::GetStarted( void )
{
	return m_bStarted;
}

LRESULT MouseGesture::OnEraseBackGround( UINT msg, WPARAM wParam, LPARAM lParam, BOOL& bHandler )
{
	return 0;
}

LRESULT MouseGesture::OnMouseMove( UINT msg, WPARAM wParam, LPARAM lParam, BOOL& bHandler )
{
	if (!m_bMouseTrack){
		TRACKMOUSEEVENT e;
		e.cbSize = sizeof(e);
		e.dwFlags = TME_LEAVE;
		e.dwHoverTime = 20;
		e.hwndTrack = m_hWnd;

		::TrackMouseEvent(&e);
		m_bMouseTrack = true;
	}

	if (wParam & MK_RBUTTON){
		CPoint pt(lParam);
		if (m_ptLast.x == -1 && m_ptLast.y == -1){
			m_ptLast = pt;
		}else{			
			HDC hdc = GetDC();
			Gdiplus::Graphics g(hdc);
			g.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
			Gdiplus::Pen p(Gdiplus::Color(255, 255, 0, 0), 5);
			p.SetEndCap(Gdiplus::LineCapRound);
			g.DrawLine(&p, m_ptLast.x, m_ptLast.y, pt.x, pt.y);
			ReleaseDC(hdc);
			m_ptLast = pt;
		}

		if (m_ptLastGesture.x == -1 && m_ptLastGesture.y == -1){
			m_ptLastGesture = CPoint(lParam);
		}else{
			int offsetX = abs(pt.x - m_ptLastGesture.x);
			int offsetY = abs(pt.y - m_ptLastGesture.y);
			if (max(offsetX, offsetY) > kMinDistance){
				BYTE direction = ED_None;
				if (offsetX >= offsetY){
					direction = pt.x > m_ptLastGesture.x ? ED_RIGHT : ED_LEFT;
				}else{
					direction = pt.y > m_ptLastGesture.y ? ED_DOWN : ED_UP;
				}
				for(int i = kDirectionCount-1; i >= 0; --i){
					if (m_Directions[i] == ED_None && direction != m_LastDirection){
						m_LastDirection = direction;
						m_Directions[i] = direction;
						break;		
					}
				}
				m_ptLastGesture = pt;
			}
		}
		ClientToScreen(&pt);
		pt.x += 40;
		DrawTip(pt);
	}
	
	return 0;
}

LRESULT MouseGesture::OnRButtonUp( UINT msg, WPARAM wParam, LPARAM lParam, BOOL& bHandler )
{
	return 0;
}

LRESULT MouseGesture::OnMouseLeave( UINT msg, WPARAM wParam, LPARAM lParam, BOOL& bHandler )
{
	return 0;
}

void MouseGesture::DrawTip(const CPoint& pt){
	DWORD dwGesture = *((DWORD*)m_Directions);
	CString strText;
	switch (dwGesture)
	{
	case EGT_LEFT:
		strText = L"Go Back";
		break;
	case EGT_RIGHT:
		strText = L"Go Forward";
		break;
	case EGT_UP:
		strText = L"Up";
		break;
	case EGT_DOWN:
		strText = L"Down";
		break;
	case EGT_LEFT_RIGHT:
		strText = L"Left Right";
		break;
	case EGT_LEFT_UP:
		strText = L"Left Up";
		break;
	case EGT_LEFT_DOWN:
		strText = L"Left Down";
		break;
	case EGT_RIGHT_LEFT:
		strText = L"Right Left";
		break;
	case EGT_RIGHT_UP:
		strText = L"Right Up";
		break;
	case EGT_RIGHT_DOWN:
		strText = L"Right Down";
		break;
	case EGT_UP_LEFT:
		strText = L"Up Left";
		break;
	case EGT_UP_RIGHT:
		strText = L"Up Right";
		break;
	case EGT_UP_DOWN:
		strText = L"Up Down";
		break;
	case EGT_DOWN_LEFT:
		strText = L"Down Left";
		break;
	case EGT_DOWN_RIGHT:
		strText = L"Close";
		break;
	case EGT_DOWN_UP:
		strText = L"Down Up";
		break;
	default:
		strText = L"";
	}

	if (m_Tip.m_Text != strText){
		m_Tip.m_Text = strText;		
	}
	
	if (m_Tip.m_Text.IsEmpty()){
		m_Tip.Hide();
	}else{
		m_Tip.Show();
		m_Tip.SetWindowPos(HWND_TOPMOST, pt.x, pt.y, 0, 0, SWP_NOACTIVATE|SWP_NOSIZE);
		m_Tip.Draw();
	}	
}

void MouseGesture::DoCommand( void )
{
	DWORD dwGesture = *((DWORD*)m_Directions);
	switch (dwGesture)
	{
	case EGT_LEFT:
		m_pCore->_GoBack();
		break;
	case EGT_RIGHT:
		m_pCore->_GoForward();
		break;
	case EGT_UP:
		
		break;
	case EGT_DOWN:
		
		break;
	case EGT_LEFT_RIGHT:
		
		break;
	case EGT_LEFT_UP:
		
		break;
	case EGT_LEFT_DOWN:
		
		break;
	case EGT_RIGHT_LEFT:
		
		break;
	case EGT_RIGHT_UP:
		
		break;
	case EGT_RIGHT_DOWN:
		
		break;
	case EGT_UP_LEFT:
		
		break;
	case EGT_UP_RIGHT:
		
		break;
	case EGT_UP_DOWN:
		
		break;
	case EGT_DOWN_LEFT:
		
		break;
	case EGT_DOWN_RIGHT:
		
		break;
	case EGT_DOWN_UP:
		
		break;
	case EGT_RIGHT_DOWN_RIGHT:
		m_pCore->_ShowMaskWindow();
		break;
	default:
		break;
	}
}


MouseGesture::Tip::Tip()
{
	
}

MouseGesture::Tip::~Tip()
{
	if (IsWindow()){
		DestroyWindow();
	}	
}

void MouseGesture::Tip::Show()
{
	EnsureCreated();
	ShowWindow(SW_SHOW);
}

void MouseGesture::Tip::Hide()
{
	EnsureCreated();
	ShowWindow(SW_HIDE);
}

LRESULT MouseGesture::Tip::OnPaint( UINT Msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	Draw();
	return 0;
}

void MouseGesture::Tip::Draw( void )
{
	RECT rc;
	GetClientRect(&rc);
	Gdiplus::Bitmap bitmap(rc.right,rc.bottom);
	Gdiplus::Graphics *mg = Gdiplus::Graphics::FromImage(&bitmap);
	Gdiplus::Font font(L"Arial", 16);
	Gdiplus::PointF pos(0, 2);
	Gdiplus::SolidBrush brush(Gdiplus::Color(255, 255, 255, 255));
	Gdiplus::SolidBrush backBrush(Gdiplus::Color(255, 0, 0, 255));

	mg->FillRectangle(&backBrush,rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top);
	mg->DrawString(m_Text.GetBuffer(), -1, &font, pos, &brush);

	HDC hdc = GetDC();
	Gdiplus::Graphics g(hdc);
	g.DrawImage(&bitmap, rc.left, rc.top, rc.right, rc.bottom);
	delete mg;
	mg = NULL;
	ReleaseDC(hdc);
}

void MouseGesture::Tip::EnsureCreated( void )
{
	if (!IsWindow()){
		CRect rect(0, 0, 120, 30);
		Create(NULL, rect, L"MouseGestureTip---", WS_POPUP|WS_DISABLED, WS_EX_TOOLWINDOW|WS_EX_LAYERED);
		SetLayeredWindowAttributes(m_hWnd, 0, 200, LWA_ALPHA);
	}
}
