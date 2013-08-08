// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef _UPDATE_MANAGER_H__
#define _UPDATE_MANAGER_H__

#include "Base/TSingleton.h"
#include <atlstr.h>

class UpdateManager : public TSingleton<UpdateManager>
{
public:
	void StartCheck(void);
	
protected:

	void FirstRunCheck(void);
	///true means need to update. 
	bool VersionCheck(CStringW &strDownLoadURL);	

	///check if it's in install process. working in the temp process.
	void InstallCheck();

	///check if there is a temp file to be installed.
	void TempCheck();

	void ExecuteNext(const CStringW& strTempFile);
	void SetInstallPath(void);
private:
};

#endif //_UPDATE_MANAGER_H__