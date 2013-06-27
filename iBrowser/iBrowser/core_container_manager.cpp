// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "stdafx.h"
#include "core_container_manager.h"
#include "XWindow.h"
#include "tab_button.h"

CoreContainerManager* CoreContainerManager::s_Instance = NULL;

CoreContainerManager::CoreContainerManager()
{

}

CoreContainerManager::~CoreContainerManager()
{

}

CXWindow* CoreContainerManager::CreateContainer(HWND hParent 
	, RECT rect 
	, E_CHILEWINDOW_CREATE_FLAG flag 
	, const CString& strURL 
	, TabButton* pTabButton )
{
	base::CScopedRefPtr<CXWindow> spContainerWindow;
	spContainerWindow.CreateInstance();
	ATLASSERT(spContainerWindow);
	spContainerWindow->Initialize(flag, strURL, pTabButton);

	HWND hContainedWnd = spContainerWindow->Create(hParent, rect, _T("XWindow"),CXWindow::kNormalStyle	,CXWindow::kNormalExStyle);
	ATLASSERT(hContainedWnd);

	m_Containers.push_back(spContainerWindow);
	return spContainerWindow.get();
}

CoreContainerManager* CoreContainerManager::GetInstance( void )
{
	if (s_Instance == NULL){
		s_Instance = new CoreContainerManager();
	}
	return s_Instance;
}

void CoreContainerManager::DeleteContainer( CXWindow* pContainer )
{
	ContainerVector::iterator iter = m_Containers.begin();
	for (; iter != m_Containers.end(); ++iter){
		if (*iter == pContainer){
			(*iter)->ShowWindow(SW_HIDE);
			(*iter)->Uninitialize();
			m_DeletedContainers.push_back(*iter);
			m_Containers.erase(iter);
			return;
		}
	}
}

void CoreContainerManager::DestroyContainer( HWND hWnd )
{
	ContainerVector::iterator iter = m_DeletedContainers.begin();
	for(; iter != m_DeletedContainers.end(); ++iter){
		if ((*iter)->m_hWnd == hWnd){
			m_DeletedContainers.erase(iter);
		}
	}
}
