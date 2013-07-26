// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#include "stdafx.h"
#include "core_window.h"
#include "core_view.h"


CoreWindow::CoreWindow(CoreView* pHost)
	:m_pHost(pHost)
{
	
}

CoreWindow::~CoreWindow()
{
	if (IsWindow()){
		UnsubclassWindow(TRUE);
	}
}

LRESULT CoreWindow::OnPaint( UINT msg, WPARAM wParam, LPARAM lParam, BOOL &bHandler )
{
	bHandler = FALSE;
	return 0;
}

void CoreWindow::Init( HWND hCore )
{
	SubclassWindow(hCore);
}

LRESULT CoreWindow::OnSetCursor( UINT msg, WPARAM wParam, LPARAM lParam, BOOL &bHandler )
{
	bHandler = FALSE;
	return 0;
}
