// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _CORE_PROCESS_LAUNCHER_H__
#define _CORE_PROCESS_LAUNCHER_H__

#include <atlstr.h>

class CoreProcessLauncher
{
public:
	CoreProcessLauncher();
	~CoreProcessLauncher();

	HANDLE Launch(CString strAppPath, CString strCmdLine);
protected:
private:
};


#endif //_CORE_PROCESS_LAUNCHER_H__