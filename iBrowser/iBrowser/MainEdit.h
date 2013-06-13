// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef _MAIN_EDIT_H__
#define _MAIN_EDIT_H__

#include <atlctrls.h>
#include <atlwin.h>
#include <atlcrack.h>
#include "MessageDef.h"


class CMainEdit : public CWindowImpl<CMainEdit,CRichEditCtrl>
{
public:
	DECLARE_WND_SUPERCLASS(NULL, CRichEditCtrl::GetWndClassName())
	CMainEdit();
	~CMainEdit();

	BEGIN_MSG_MAP(CMainEdit)
		MESSAGE_HANDLER(WM_KEYUP, OnKeyUp)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	END_MSG_MAP()

	LRESULT OnKeyDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
protected:
public:
	LRESULT OnKeyUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};

#endif //_MAIN_EDIT_H__
