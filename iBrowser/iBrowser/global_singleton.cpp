// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#include "stdafx.h"
#include "global_singleton.h"
#include "core_proxy.h"
#include "core_process_manager.h"
#include "profile.h"
#include "bookmark_manager.h"

GlobalSingleton* GlobalSingleton::s_Instance = NULL;

GlobalSingleton::GlobalSingleton()
	:m_hMaskWin(NULL)
	,m_nProcessType(EPT_NONE)
	,m_nProcessMode(EPM_SINGLE)
	,m_pCoreProcessManager(NULL)
	,m_pProfile(NULL)
	,m_pBookmarkManager(NULL)
	,m_bEnableCrossProcessRender(false)
{
	m_pCoreProcessManager = new CoreProcessManager();
	m_pProfile = new Profile();
	m_pBookmarkManager = new BookmarkManager();
}

GlobalSingleton* GlobalSingleton::GetInstance(){
	if (s_Instance == NULL){
		s_Instance = new GlobalSingleton();
	}
	return s_Instance;
}

void GlobalSingleton::SetMaskHWND(HWND hMask){
	m_hMaskWin = hMask;
}

HWND GlobalSingleton::GetMaskHWND(){
	return m_hMaskWin;
}

void GlobalSingleton::SetCurrentCoreProxy( CoreProxy *pCore )
{
	m_spCurrentCoreProxy = pCore;
}

CoreProxy* GlobalSingleton::GetCurrentCoreProxy( void )
{
	return m_spCurrentCoreProxy.get();
}

int GlobalSingleton::GetProcessType( void )
{
	return m_nProcessType;
}

void GlobalSingleton::SetProcessType( int nType )
{
	m_nProcessType = nType;
}

int GlobalSingleton::GetProcessMode( void )
{
	return m_nProcessMode;
}

void GlobalSingleton::SetProcessMode( int nMode )
{
	m_nProcessMode = nMode;
}

GlobalSingleton::~GlobalSingleton()
{
	if (m_pCoreProcessManager){
		delete m_pCoreProcessManager;
		m_pCoreProcessManager = NULL;
	}
	if (m_pProfile){
		delete m_pProfile;
		m_pProfile = NULL;
	}
	if (m_pBookmarkManager){
		delete m_pBookmarkManager;
		m_pBookmarkManager = NULL;
	}
}

CoreProcessManager* GlobalSingleton::GetCoreProcessManager( void )
{
	return m_pCoreProcessManager;
}

Profile* GlobalSingleton::GetProfile( void )
{
	return m_pProfile;
}

BookmarkManager* GlobalSingleton::GetBookmarkManager( void )
{
	return m_pBookmarkManager;
}

bool GlobalSingleton::GetCrossProcessRender( void )
{
	return m_bEnableCrossProcessRender;
}

void GlobalSingleton::SetCrossProcessRender( bool bEnable )
{
	m_bEnableCrossProcessRender = bEnable;
}
