// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _BOOKMARK_H__
#define _BOOKMARK_H__

#include <atlstr.h>
#include <map>

class Bookmark
{
public:
	Bookmark();
	~Bookmark();
	CStringW m_strURL;
	CStringW m_strTitle;
	CStringW m_strFaviconURL;
	unsigned int m_nUseCount;
};

typedef std::map<CStringW, Bookmark> BookmarkMap;
typedef std::pair<CStringW, Bookmark> BookmarkPair;

#endif //_BOOKMARK_H__