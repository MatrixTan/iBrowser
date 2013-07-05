// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "stdafx.h"
#include "bookmark_manager.h"


bool BookmarkManager::AddBookmark( const CStringW& strURL, const CStringW& strTitle, const CStringW& strFaviconURL )
{
	Bookmark bookmark;
	bookmark.m_strURL = strURL;
	bookmark.m_strTitle = strTitle;
	bookmark.m_strFaviconURL = strFaviconURL;

	m_Bookmarks.insert(BookmarkPair(strURL, bookmark));
	return true;
}

bool BookmarkManager::CheckIsAdded( const CStringW& strURL )
{
	return m_Bookmarks.find(strURL) != m_Bookmarks.end();
}

void BookmarkManager::DeleteBookmark( const CStringW& strURL )
{
	BookmarkMap::iterator iter = m_Bookmarks.find(strURL);
	if (iter != m_Bookmarks.end()){
		m_Bookmarks.erase(iter);
	}
}
