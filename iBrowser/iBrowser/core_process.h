// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _CORE_PROCESS_H__
#define _CORE_PROCESS_H__

class CoreProcess
{
public:
	CoreProcess();
	~CoreProcess();

	int Run(LPTSTR lpCmdLine);
protected:
private:
};

#endif //_CHILD_PROCESS_H__