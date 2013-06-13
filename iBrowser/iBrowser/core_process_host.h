// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _CORE_PROCESS_HOST_H__
#define _CORE_PROCESS_HOST_H__

#include <atlapp.h>
#include <atlstr.h>
#include <Base/ipc_message.h>
#include "MessageDef.h"

class CoreProcessStartContext
{
public:
	CoreProcessStartContext()
		:hostWnd(NULL){};
	CString strURL;
	HWND hostWnd;
};


class CoreProcessHost : public CMessageFilter
	,public CWindowImpl<CoreProcessHost, CAxWindow>
{
public:
	CoreProcessHost();
	virtual ~CoreProcessHost();

	int Start(CoreProcessStartContext *pParam);
	void Stop(void);
	void CreateCore(HWND hHost);

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	BEGIN_MSG_MAP(CoreProcessHost)
	    HANDLE_IPC_MSG(CoreProcessHost)
		MESSAGE_HANDLER(WM_CORE_PROCESS_CREATED, OnCoreProcessCreated);
	END_MSG_MAP()

	LRESULT OnCoreProcessCreated(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

protected:
private:
	CoreProcessStartContext *m_pStartContext;
	HWND m_hCoreMsgHWND;
	DWORD m_ThreadId;
};


#endif //_CORE_PROCESS_HOST_H__