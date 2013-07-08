// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _BOOKMARK_STORAGE_H__
#define _BOOKMARK_STORAGE_H__

#include "storage.h"
#include <map>
#include "bookmark.h"

class Bookmark;
class BookmarkStorage : public Storage
{
public:
	BookmarkStorage();
	~BookmarkStorage();

	bool AddBookmark(const Bookmark* pBookmark);
	bool DeleteBookmark(const Bookmark* pBookmark);
	void GetBookmarks(BookmarkMap& bookmarks);

protected:
private:
};


#endif //_BOOKMARK_STORAGE_H__