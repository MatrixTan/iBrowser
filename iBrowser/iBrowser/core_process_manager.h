// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _CORE_PROCESS_MANAGER_H__
#define _CORE_PROCESS_MANAGER_H__

#include <atlstr.h>
#include <vector>

class CoreProcessHost;
class CoreProcessManager
{
public:
	CoreProcessManager();
	~CoreProcessManager();

	CoreProcessHost* GetProcessHost();
	void StopAll(void);
	bool AssignProcessToJob(HANDLE hProcess);

protected:
private:

	HANDLE m_hJob;
	typedef std::vector<CoreProcessHost*> HostVector;
	typedef HostVector::iterator HostIter;
	HostVector m_vHosts;
};


#endif //_CORE_PROCESS_MANAGER_H__