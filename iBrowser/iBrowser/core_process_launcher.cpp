// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "stdafx.h"
#include "core_process_launcher.h"


CoreProcessLauncher::CoreProcessLauncher()
{

}

CoreProcessLauncher::~CoreProcessLauncher()
{

}

HANDLE CoreProcessLauncher::Launch( CString strAppPath, CString strCmdLine )
{
	PROCESS_INFORMATION proInfo = {0};
	STARTUPINFO startInfo = {0};
	BOOL bResult = ::CreateProcess(strAppPath.GetBuffer()
		, strCmdLine.GetBuffer(), NULL, NULL, TRUE
		, CREATE_BREAKAWAY_FROM_JOB
		, NULL, NULL, &startInfo, &proInfo);

	return proInfo.hProcess;
}
