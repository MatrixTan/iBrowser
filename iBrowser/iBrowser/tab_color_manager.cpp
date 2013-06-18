// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "stdafx.h"
#include "tab_color_manager.h"


TabColorManager* TabColorManager::s_Instance = NULL;
//color scheme: http://www.peise.net/2013/0617/3250.html
const DWORD TabColorManager::kColors[] = {0xFFF09F0A, 0xFF98F046, 0xFFF078C0, 0xFF055387, 0xFF87C4ED};

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