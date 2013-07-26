// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "stdafx.h"
#include "core_proxy.h"
#include "MessageDef.h"
#include "global_singleton.h"
#include <Base/ipc_message.h>
#include "XWindow.h"

CoreProxy::CoreProxy( HWND hCoreView, CXWindow *pContainer, UINT flag)
	:m_hCoreView(hCoreView)
	,m_pContainer(pContainer)
	,m_nFlag(flag)
{

}

CoreProxy::~CoreProxy()
{

}

HWND CoreProxy::GetCoreView( void )
{
	return m_hCoreView;
}

HWND CoreProxy::GetContainerHWND( void )
{
	return m_pContainer->m_hWnd;
}

void CoreProxy::Refresh( void )
{
	::PostMessage(m_hCoreView, WM_CORE_REFRESH, 0, 0);
}

void CoreProxy::Navigate( const CString& strURL )
{
	int type = GlobalSingleton::GetInstance()->GetProcessMode();
	if(type == EPM_MULTIPLE){
		IPC::PostIPCMessage<CStringW>(m_hCoreView, WM_CORE_NAVIGATE, strURL);
	}else if(type == EPM_SINGLE){
		void* pData = NULL;
		UINT nSize = 0;
		Serialize<CStringW>::Write(&strURL, &pData, &nSize);
		::PostMessage(m_hCoreView, WM_CORE_NAVIGATE, (WPARAM)pData, 0);
	}
	
}

void CoreProxy::GoBack()
{
	::PostMessage(m_hCoreView, WM_CORE_GOBACK, 0, 0);
}

void CoreProxy::GoForward()
{
	::PostMessage(m_hCoreView, WM_CORE_GOFORWARD, 0, 0);
}

UINT CoreProxy::GetFlag( void )
{
	return m_nFlag;
}

void CoreProxy::Destroy()
{
	::PostMessage(m_hCoreView, WM_CORE_DESTROY, 0, 0);
}

void CoreProxy::Focus()
{
	::PostMessage(m_hCoreView, WM_CORE_FOCUS, 0, 0);
}

void CoreProxy::GetURL( CStringW& strURL )
{
	strURL = m_pContainer->GetURL();
}

bool CoreProxy::AddCurrentBookmark( void )
{
	return m_pContainer->AddCurrentBookmark();
}

void CoreProxy::RefreshCoreWindow( void )
{
	::PostMessage(m_hCoreView, WM_REFRESH_CORE_WINDOW, 0, 0);
}
