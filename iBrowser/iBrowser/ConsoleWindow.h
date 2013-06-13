// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _IBROWSER_CONSOLE_WINDOW_H__
#define _IBROWSER_CONSOLE_WINDOW_H__

#include <atltypes.h>
#include <atlctrls.h>
#include <atlstr.h>

class CConsoleWindow : public CWindowImpl<CConsoleWindow, CAxWindow>
{
public:
	CConsoleWindow();
	~CConsoleWindow();

	DECLARE_WND_SUPERCLASS(NULL, CAxWindow::GetWndClassName())

	BEGIN_MSG_MAP(CConsoleWindow)
		MESSAGE_HANDLER(WM_CREATE, OnCreate);
		MESSAGE_HANDLER(WM_PAINT, OnPaint);	
	END_MSG_MAP()
	
	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	BOOL Log(const CStringW& strLog);
	BOOL Clear(void);
		
protected:
private:
	CStringW m_strLog;
	CStatic m_static;
	CRichEditCtrl m_Edit;
};


#endif //_IBROWSER_CONSOLE_WINDOW_H__