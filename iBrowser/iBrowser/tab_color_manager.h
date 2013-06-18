// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _TAB_COLOR_MANAGER_H__
#define _TAB_COLOR_MANAGER_H__

#include <atlstr.h>
#include <map>

class TabColorManager
{
public:
	static TabColorManager* GetInstance();
	~TabColorManager();

	DWORD GetColor(const CString& strHost);

protected:
private:
	TabColorManager();
	static TabColorManager* s_Instance;

	typedef std::map<CString, DWORD> ColorMap;
	typedef ColorMap::iterator ColorIter;
	typedef std::pair<CString, DWORD> ColorPair;

	ColorMap m_mapColor;

	static const int kAllColorCount = 5;
	static const DWORD kColors[];
	int m_nNextIndex;
};

#endif //_TAB_COLOR_MANAGER_H__