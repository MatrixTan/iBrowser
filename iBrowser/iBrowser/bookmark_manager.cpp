// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "stdafx.h"
#include "bookmark_manager.h"
#include "util_str.h"


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

void BookmarkManager::GetBookmarksJson( CStringW& strBookmarks )
{
	strBookmarks =L"[";
	BookmarkMap::iterator iter = m_Bookmarks.begin();
	for (; iter != m_Bookmarks.end(); ++iter){
		CStringW strURL, strTitle, strFaviconURL;
		Util::Stringify(iter->second.m_strURL, strURL);
		Util::Stringify(iter->second.m_strTitle, strTitle);
		Util::Stringify(iter->second.m_strFaviconURL, strFaviconURL);
		if (iter != m_Bookmarks.begin()){
			strBookmarks += L",";
		}
		
		strBookmarks += L"{\"url\":\"";
		strBookmarks += strURL;
		strBookmarks += L"\", \"title\":\"";
		strBookmarks += strTitle;
		strBookmarks += L"\", \"icon\":\"";
		strBookmarks += strFaviconURL;
		strBookmarks += L"\"}";
	}
	strBookmarks += L"]";
}
