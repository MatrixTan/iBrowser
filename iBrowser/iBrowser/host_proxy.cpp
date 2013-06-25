// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "stdafx.h"
#include "host_proxy.h"
#include "global_singleton.h"
#include "Base/ipc_message.h"
#include "MessageDef.h"

HostProxy::HostProxy( HWND hHostWindow )
	:m_hHostWindow(hHostWindow)
{

}

HostProxy::~HostProxy()
{

}

void HostProxy::NotifyBeforeNavigate(const CString& strURL)
{
	PostStrToHost(WM_BEFORE_NAVIGATE, strURL);
}

void HostProxy::NotifyTitleChange( const CString& strTitle )
{
	PostStrToHost(WM_TITLE_CHANGE, strTitle);
}

void HostProxy::NotifyNavigateComplete( const CString& strURL )
{
	PostStrToHost(WM_NAVIGATE_COMPLETE, strURL);
}

void HostProxy::PostStrToHost( UINT msg, const CString& str )
{
	int type = GlobalSingleton::GetInstance()->GetProcessMode();
	if(type == EPM_MULTIPLE){
		IPC::PostIPCMessage<CStringW>(m_hHostWindow, msg, str);
	}else if(type == EPM_SINGLE){
		void* pData = NULL;
		UINT nSize = 0;
		Serialize<CStringW>::Write(&str, &pData, &nSize);
		::PostMessage(m_hHostWindow, msg, (WPARAM)pData, 0);
	}
}
