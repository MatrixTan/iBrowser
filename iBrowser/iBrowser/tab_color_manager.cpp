// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "stdafx.h"
#include "tab_color_manager.h"


TabColorManager* TabColorManager::s_Instance = NULL;
const DWORD TabColorManager::kColors[] = {0xFF990000, 0xFF009900, 0xFF000099, 0xFF999900, 0xFF009999, 0xFF990099};

TabColorManager* TabColorManager::GetInstance()
{
	if (NULL == s_Instance){
		s_Instance = new TabColorManager();
	}
	return s_Instance;	
}

TabColorManager::~TabColorManager()
{

}

TabColorManager::TabColorManager()
	:m_nNextIndex(0)
{

}

DWORD TabColorManager::GetColor( const CString& strHost )
{
	ColorIter iter = m_mapColor.find(strHost);
	if (iter != m_mapColor.end()){
		return iter->second;
	}
	ColorPair pair(strHost, kColors[m_nNextIndex++]);
	if (m_nNextIndex >= kAllColorCount){
		m_nNextIndex = 0;
	}
	m_mapColor.insert(pair);
	return pair.second;
}