// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "stdafx.h"
#include "core_process_host.h"
#include "core_process_manager.h"


CoreProcessManager::CoreProcessManager()
	:m_hJob(NULL)
{
	m_hJob = ::CreateJobObject(NULL, NULL);
	JOBOBJECT_EXTENDED_LIMIT_INFORMATION limit = {0};
	limit.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE | JOB_OBJECT_LIMIT_BREAKAWAY_OK;
	::SetInformationJobObject(m_hJob, JobObjectExtendedLimitInformation, &limit, sizeof(limit));
}

CoreProcessManager::~CoreProcessManager(){
	CloseHandle(m_hJob);
	m_hJob = NULL;
}

CoreProcessHost* CoreProcessManager::GetProcessHost(){
	 CoreProcessHost* pHost = new CoreProcessHost();
	 m_vHosts.push_back(pHost);
	 return pHost;
}

bool CoreProcessManager::AssignProcessToJob( HANDLE hProcess )
{
	BOOL bRet = ::AssignProcessToJobObject(m_hJob, hProcess);
	return bRet == TRUE;
}

void CoreProcessManager::StopAll( void )
{
	HostIter iter = m_vHosts.begin();
	for (; iter != m_vHosts.end(); ++iter){
		(*iter)->Stop();
	}
}
