// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef _GLOBAL_SINGLETON_H__
#define _GLOBAL_SINGLETON_H__
#include <Base\RefCounted.h>

enum E_PROCESS_TYPE{
	EPT_NONE = 0,
	EPT_MAIN,
	EPT_CORE,
};
enum E_PROCESS_MODE{
	EPM_NONE = 0,
	EPM_SINGLE,
	EPM_MULTIPLE,
};

class CoreProxy;
class CoreProcessManager;
class Profile;
class BookmarkManager;

class GlobalSingleton
{
public:
	static GlobalSingleton* GetInstance(void);
	~GlobalSingleton();

	void SetMaskHWND(HWND hMask);
	HWND GetMaskHWND(void);

	void SetCurrentCoreProxy(CoreProxy *pCore);
	CoreProxy* GetCurrentCoreProxy(void);

	int GetProcessType(void);
	void SetProcessType(int nType);
	int GetProcessMode(void);
	void SetProcessMode(int nMode);
	bool GetCrossProcessRender(void);
	void SetCrossProcessRender(bool bEnable);

	CoreProcessManager* GetCoreProcessManager(void);
	Profile* GetProfile(void);
	BookmarkManager* GetBookmarkManager(void);

protected:
private:
	GlobalSingleton();
	static GlobalSingleton* s_Instance;

	HWND m_hMaskWin;
	base::CScopedRefPtr<CoreProxy> m_spCurrentCoreProxy;
	int m_nProcessType;
	int m_nProcessMode;
	CoreProcessManager* m_pCoreProcessManager;
	Profile* m_pProfile;
	BookmarkManager* m_pBookmarkManager;
	bool m_bEnableCrossProcessRender;
};
#endif //_GLOBAL_SINGLETON_H__