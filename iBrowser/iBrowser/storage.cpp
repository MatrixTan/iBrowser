// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "stdafx.h"
#include "storage.h"
#include <fstream>

#include <atlstr.h>
#include <leveldb/leveldb.h>
#include <jsoncpp/json.h>
#include "util_common.h"


static const char *kProfilePath = "\\iBrowser\\";

bool Storage::SetValue( const char *db, const char* key, const char* value )
{
	std::string strPath = Util::GetAppDataPathA().GetBuffer();
	strPath += kProfilePath;
	strPath += db;	

	leveldb::DB* database = NULL;
	leveldb::Options options;
	options.create_if_missing = true;
	leveldb::Status s = leveldb::DB::Open(options, strPath.c_str(), &database);
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

bool Storage::GetValue( const char *db, const char* key, std::string& strValue )
{
	std::string strPath = Util::GetAppDataPathA().GetBuffer();
	strPath += kProfilePath;
	strPath += db;

	leveldb::DB* database = NULL;
	leveldb::Options options;
	options.create_if_missing = true;

	leveldb::Status s = leveldb::DB::Open(options, strPath.c_str(), &database);
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

Storage::Storage()
{

}

Storage::~Storage()
{

}

bool Storage::DeleteValue( const char*db, const char* key )
{
	std::string strPath = Util::GetAppDataPathA().GetBuffer();
	strPath += kProfilePath;
	strPath += db;

	leveldb::DB* database = NULL;
	leveldb::Options options;
	options.create_if_missing = false;

	leveldb::Status s = leveldb::DB::Open(options, strPath.c_str(), &database);
	if (NULL == database || !s.ok()){
		return false;
	}

	bool bRet = true;
	s = database->Delete(leveldb::WriteOptions(), key);
	if (!s.ok()){
		bRet = false;
	}
	delete database;
	database = NULL;
	return bRet;
}
