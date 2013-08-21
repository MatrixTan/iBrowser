// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "stdafx.h"
#include "cross_render_host.h"
#include "../global_singleton.h"
#include <atlstr.h>

namespace CrossRender
{
	
LRESULT CrossRenderHost::OnCreate( UINT msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	return 0;
}

BOOL CrossRenderHost::PostToCoreForCPR( UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (!GlobalSingleton::GetInstance()->IsCrossRender()){
		return FALSE;
	}
	switch (msg){
		//case WM_NCHITTEST:
	case WM_MOUSEMOVE:
		{
			CString str;
			int xPos = LOWORD(lParam);
			int yPos = HIWORD(lParam);
			str.Format(L"Mouse: X-%x, Y-%x\n", xPos, yPos);
			//::OutputDebugString(str);
			if (::IsWindow(m_hCoreWindow)){
				::PostMessage(m_hCoreWindow, msg, wParam, lParam);
			}
			return TRUE;
		}
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_LBUTTONDBLCLK:

	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_RBUTTONDBLCLK:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MBUTTONDBLCLK:
	case WM_MOUSEWHEEL:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_XBUTTONDBLCLK:

	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_CHAR:
	case WM_DEADCHAR:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
	case WM_SYSCHAR:
	case WM_SYSDEADCHAR:
	case WM_UNICHAR:
		//case WM_SETCURSOR:
		{
			CString str;
			str.Format(L"Foucs: %x\n", msg);
			::OutputDebugString(str);
			if (::IsWindow(m_hCoreWindow)){
				::PostMessage(m_hCoreWindow, msg, wParam, lParam);
			}
			return TRUE;
		}
	default:
		return FALSE;
	}
	return FALSE;
}

CrossRenderHost::CrossRenderHost()
	:m_hCoreWindow(NULL)
{

}

CrossRenderHost::~CrossRenderHost()
{

}

void CrossRenderHost::SetCoreWindow( HWND hCore )
{
	m_hCoreWindow = hCore;
}

LRESULT CrossRenderHost::OnPosChanged( UINT msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	WINDOWPOS *pPos = (WINDOWPOS*)lParam;
	
	return 0;
}

LRESULT CrossRenderHost::OnEraseBK( UINT msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	return FALSE;
}

}
