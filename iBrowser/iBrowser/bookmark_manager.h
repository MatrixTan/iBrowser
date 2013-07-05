// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _BOOKMARK_MANAGER_H__
#define _BOOKMARK_MANAGER_H__


#include <atlstr.h>
#include <vector>
#include <map>
#include "bookmark.h"

class BookmarkManager
{
public:
	bool AddBookmark(const CStringW& strURL, const CStringW& strTitle, const CStringW& strFaviconURL);
	void DeleteBookmark(const CStringW& strURL);
	bool CheckIsAdded(const CStringW& strURL);
	void GetBookmarksJson(CStringW& strBookmarks);
protected:
private:

	//typedef std::vector<Bookmark> BookmarkVector;
	typedef std::map<CStringW, Bookmark> BookmarkMap;
	typedef std::pair<CStringW, Bookmark> BookmarkPair;
	BookmarkMap m_Bookmarks;
};


#endif //_BOOKMARK_MANAGER_H__
