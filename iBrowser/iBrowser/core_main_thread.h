// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _CORE_MAIN_THREAD_H__
#define _CORE_MAIN_THREAD_H__

#include <Base/ipc_message.h>
#include "MessageDef.h"


class CoreMainThread :public CMessageFilter
	,public CWindowImpl<CoreMainThread, CAxWindow>
{
public:
	CoreMainThread();
	~CoreMainThread();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	BEGIN_MSG_MAP(CoreMainThread)
		HANDLE_IPC_MSG(CoreMainThread)
		MESSAGE_HANDLER(WM_CREATE_CORE, OnCreateCore)		
	END_MSG_MAP()

	LRESULT OnCreateCore(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	int Run(void);
protected:
private:
};


#endif //_CORE_MAIN_THREAD_H__