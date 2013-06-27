// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _CORE_CONTAINER_MANAGER_H__
#define _CORE_CONTAINER_MANAGER_H__

#include "MessageDef.h"
#include <atlstr.h>
#include <vector>
#include "base/RefCounted.h"

class CXWindow;
class TabButton;

class CoreContainerManager
{
public:
	static CoreContainerManager* GetInstance(void);
	~CoreContainerManager();

	CXWindow* CreateContainer(HWND hParent
		, RECT rect
		, E_CHILEWINDOW_CREATE_FLAG flag
		, const CString& strURL
		, TabButton* pTabButton);

	void DeleteContainer(CXWindow* pContainer);

	void DestroyContainer(HWND hWnd);

protected:
private:
	CoreContainerManager();

	typedef std::vector<base::CScopedRefPtr<CXWindow> > ContainerVector;
	ContainerVector m_Containers;
	ContainerVector m_DeletedContainers;

	static CoreContainerManager* s_Instance;
};


#endif //_CORE_CONTAINER_MANAGER_H__