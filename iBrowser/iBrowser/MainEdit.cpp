// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#include "stdafx.h"
#include "MainEdit.h"

LRESULT CMainEdit::OnKeyUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	TCHAR vk = (TCHAR)wParam;
	UINT cRepeat = (UINT)LOWORD(lParam);
	UINT flags = (UINT)HIWORD(lParam);
	bHandled = FALSE;
	switch(vk)
	{
	case VK_RETURN:
		//::PostMessage(::GetParent(m_hWnd),WM_MAINEDIT_SUBMIT,0,(LPARAM)m_hWnd);
		bHandled = TRUE;
		break;;
	case VK_BACK:
		bHandled = TRUE;
		break;
	default:
		break;
	}
	return 0;
}

CMainEdit::CMainEdit()
{

}

CMainEdit::~CMainEdit()
{

}

LRESULT CMainEdit::OnCreate( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	CHARFORMAT cf;
	ZeroMemory(&cf, sizeof(CHARFORMAT));
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_BOLD | CFM_COLOR | CFM_FACE |
		CFM_ITALIC | CFM_SIZE | CFM_UNDERLINE;
	cf.dwEffects = 0;
	cf.yHeight = 15 * 20;//文字高度
	cf.crTextColor = RGB(0, 0, 0); //文字颜色
	cf.dwEffects = CFE_DISABLED;
	StrCpyW(cf.szFaceName ,_T("微软雅黑"));//设置字体

	SetDefaultCharFormat(cf);
	ShowCaret();
	SHAutoComplete(m_hWnd,SHACF_URLALL);
	return 0;
}

LRESULT CMainEdit::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

