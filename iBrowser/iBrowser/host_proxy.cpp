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
	int type = GlobalSingleton::GetInstance()->GetProcessMode();
	if(type == EPM_MULTIPLE){
		IPC::PostIPCMessage<CStringW>(m_hHostWindow, WM_BEFORE_NAVIGATE, strURL);
	}else if(type == EPM_SINGLE){
		void* pData = NULL;
		UINT nSize = 0;
		Serialize<CStringW>::Write(&strURL, &pData, &nSize);
		::PostMessage(m_hHostWindow, WM_BEFORE_NAVIGATE, (WPARAM)pData, 0);
	}
}

void HostProxy::NotifyTitleChange( const CString& strTitle )
{
	int type = GlobalSingleton::GetInstance()->GetProcessMode();
	if(type == EPM_MULTIPLE){
		IPC::PostIPCMessage<CStringW>(m_hHostWindow, WM_TITLE_CHANGE, strTitle);
	}else if(type == EPM_SINGLE){
		void* pData = NULL;
		UINT nSize = 0;
		Serialize<CStringW>::Write(&strTitle, &pData, &nSize);
		::PostMessage(m_hHostWindow, WM_TITLE_CHANGE, (WPARAM)pData, 0);
	}
}
