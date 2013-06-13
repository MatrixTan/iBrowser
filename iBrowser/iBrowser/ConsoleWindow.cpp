// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "stdafx.h"
#include "ConsoleWindow.h"


LRESULT CConsoleWindow::OnPaint( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	return 0;
}

CConsoleWindow::CConsoleWindow()
{

}

CConsoleWindow::~CConsoleWindow()
{

}

BOOL CConsoleWindow::Log( const CStringW& strLog )
{
	m_strLog += strLog;
	m_strLog += L"\n";
	if (IsWindow())
	{
		m_Edit.SetWindowText(m_strLog);
		m_Edit.RedrawWindow();
	}	
	return TRUE;
}

BOOL CConsoleWindow::Clear( void )
{
	m_strLog = L"";
	if(IsWindow())
	{
		m_Edit.SetWindowText(m_strLog);
	}	
	return TRUE;
}

LRESULT CConsoleWindow::OnCreate( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	CRect rectClient;
	GetClientRect(&rectClient);
	m_Edit.Create(m_hWnd, rectClient,NULL, WS_VISIBLE|WS_CHILD|WS_VSCROLL|ES_MULTILINE|WS_EX_CLIENTEDGE|ES_READONLY,0); 
	return 0;
}

