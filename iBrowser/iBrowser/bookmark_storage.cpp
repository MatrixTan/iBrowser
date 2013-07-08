// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "stdafx.h"
#include "bookmark_storage.h"
#include <jsoncpp/json.h>
#include "bookmark.h"

static const char* kBookmarkStorage = "bookmarks";

bool BookmarkStorage::AddBookmark( const Bookmark* pBookmark )
{
	if (NULL == pBookmark){
		return false;
	}

	Json::Value bookmark;
	std::string strUrl = CW2A(pBookmark->m_strURL, CP_UTF8).m_psz;
	bookmark["url"] = strUrl.c_str();
	bookmark["title"] = CW2A(pBookmark->m_strTitle, CP_UTF8).m_psz;
	bookmark["icon"] = CW2A(pBookmark->m_strFaviconURL, CP_UTF8).m_psz;

	Json::FastWriter writer;
	std::string strValue = writer.write(bookmark);
	SetValue(kBookmarkStorage, strUrl.c_str(), strValue.c_str());
	return true;	
}

bool BookmarkStorage::DeleteBookmark( const Bookmark* pBookmark )
{
	if (NULL == pBookmark){
		return false;
	}
	return DeleteValue(kBookmarkStorage
		,CW2A(pBookmark->m_strURL).m_psz);
}

void BookmarkStorage::GetBookmarks( std::vector<Bookmark*>& bookmarks )
{

}
