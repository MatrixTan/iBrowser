// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "stdafx.h"
#include "profile.h"
#include <fstream>

#include <atlstr.h>
#include <leveldb/leveldb.h>
#include "util_common.h"

static const char *kProfilePath = "\\iBrowser\\";
static const char *kUIProfile = "ui.profile";

Profile::Profile()
{

}

Profile::~Profile()
{

}

bool Profile::GetMainFrameRect( RECT *pRect )
{
	return false;
}

bool Profile::SetMainFrameRect( RECT rect )
{
	leveldb::DB* db = NULL;
	leveldb::Options options;
	options.create_if_missing = true;
	std::string strPath = Util::GetAppDataPathA().GetBuffer();
	strPath += kProfilePath;
	strPath += kUIProfile;
	leveldb::Status s = leveldb::DB::Open(options, strPath, &db);
	if (NULL == db || !s.ok()){
		return false;
	}
	
	CStringA strMainFrameRect;
	strMainFrameRect.Format("%d,%d,%d,%d", rect.left, rect.top, rect.right, rect.bottom);

	s = db->Put(leveldb::WriteOptions(), "main_frame_rect", strMainFrameRect.GetBuffer());
	if (!s.ok()){
		return false;
	}

	delete db;
	db = NULL;
	return true;
}
