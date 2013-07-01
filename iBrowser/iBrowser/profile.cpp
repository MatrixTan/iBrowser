// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "stdafx.h"
#include "profile.h"
#include <fstream>

#include <atlstr.h>
#include <leveldb/leveldb.h>
#include <jsoncpp/json.h>
#include "util_common.h"

static const char *kProfilePath = "\\iBrowser\\";
static const char *kUIProfile = "ui.profile";

Profile::Profile()
{

}

Profile::~Profile()
{

}

bool Profile::GetMainFrameRect( RECT *pRect , bool* pIsMax)
{
	std::string strPath = Util::GetAppDataPathA().GetBuffer();
	strPath += kProfilePath;
	strPath += kUIProfile;	

	std::string strMainFrameProfile;
	bool ret = GetValue(strPath.c_str(), "main_frame_profile", strMainFrameProfile);
	if (!ret){
		return false;
	}
	Json::Reader reader;
	Json::Value jsonValue;
	if (!reader.parse(strMainFrameProfile, jsonValue)){
		return false;
	}
	if (pRect){
		pRect->left = jsonValue["left"].asInt();
		pRect->right = jsonValue["right"].asInt();
		pRect->top = jsonValue["top"].asInt();
		pRect->bottom = jsonValue["bottom"].asInt();
	}
	if (pIsMax){
		*pIsMax = jsonValue["is_max"].asBool();
	}	
	return true;
}

bool Profile::SetMainFrameProfile( RECT rect , bool bIsMax)
{
	std::string strPath = Util::GetAppDataPathA().GetBuffer();
	strPath += kProfilePath;
	strPath += kUIProfile;	
	
	Json::Value jsonValue;
	jsonValue["left"] = rect.left;
	jsonValue["right"] = rect.right;
	jsonValue["top"] = rect.top;
	jsonValue["bottom"] = rect.bottom;
	jsonValue["is_max"] = bIsMax;

	Json::FastWriter writer;
	std::string strProfile = writer.write(jsonValue);
	return SetValue(strPath.c_str(), "main_frame_profile", strProfile.c_str());
}

bool Profile::SetValue( const char *db, const char* key, const char* value )
{
	leveldb::DB* database = NULL;
	leveldb::Options options;
	options.create_if_missing = true;
	leveldb::Status s = leveldb::DB::Open(options, db, &database);
	if (NULL == database || !s.ok()){
		return false;
	}

	bool bRet = true;
	s = database->Put(leveldb::WriteOptions(), key, value);
	if (!s.ok()){
		bRet = false;
	}

	delete database;
	database = NULL;
	return bRet;
}

bool Profile::GetValue( const char *db, const char* key, std::string& strValue )
{
	leveldb::DB* database = NULL;
	leveldb::Options options;
	options.create_if_missing = true;
	
	leveldb::Status s = leveldb::DB::Open(options, db, &database);
	if (NULL == database || !s.ok()){
		return false;
	}

	bool bRet = true;
	std::string value;
	s = database->Get(leveldb::ReadOptions(), key, &value);
	if (!s.ok()){
		bRet = false;
	}
	strValue = value;
	delete database;
	database = NULL;
	return bRet;
}
