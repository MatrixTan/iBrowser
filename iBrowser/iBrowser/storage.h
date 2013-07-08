// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _STORAGE_H__
#define _STORAGE_H__

#include <string>

class Storage
{
public:
	Storage();
	~Storage();
protected:

	bool SetValue(const char *db, const char* key, const char* value);
	bool GetValue(const char *db, const char* key, std::string& strValue);
	bool DeleteValue(const char*db, const char* key);

private:
};


#endif //_STORAGE_H__