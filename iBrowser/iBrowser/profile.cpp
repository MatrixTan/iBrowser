// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "stdafx.h"
#include "profile.h"
#include <fstream>

#include <leveldb/leveldb.h>
#include <jsoncpp/json.h>
#include "util_common.h"
#include "resource.h"
#include <atlconv.h>

static const char *kUIProfile = "ui.profile";
static const char *kMainProfile = "main.profile";

Profile::Profile()
{

}

Profile::~Profile()
{

}

bool Profile::GetMainFrameRect( RECT *pRect , bool* pIsMax)
{
	std::string strMainFrameProfile;
	bool ret = GetValue(kUIProfile, "main_frame_profile", strMainFrameProfile);
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

	Json::Value jsonValue;
	jsonValue["left"] = rect.left;
	jsonValue["right"] = rect.right;
	jsonValue["top"] = rect.top;
	jsonValue["bottom"] = rect.bottom;
	jsonValue["is_max"] = bIsMax;

	Json::FastWriter writer;
	std::string strProfile = writer.write(jsonValue);
	return SetValue(kUIProfile, "main_frame_profile", strProfile.c_str());
}

bool Profile::GetHomeURL( CStringW &strURL )
{
	strURL.LoadString(IDS_HOME_URL);
	return true;
}

bool Profile::GetVersion( iVersion& version )
{
	std::string versionString;
	bool ret = GetValue(kMainProfile, "version", versionString);
	if (!ret){
		return false;
	}
	
	Json::Reader reader;
	Json::Value jsonValue;
	if (!reader.parse(versionString, jsonValue)){
		return false;
	}

	version.v1 = jsonValue["v1"].asInt();
	version.v2 = jsonValue["v2"].asInt();
	version.v3 = jsonValue["v3"].asInt();
	version.v4 = jsonValue["v4"].asInt();
	return true;
}

bool Profile::SetVersion( const iVersion& version )
{
	Json::Value versinJson;
	versinJson["v1"] = version.v1;
	versinJson["v2"] = version.v2;
	versinJson["v3"] = version.v3;
	versinJson["v4"] = version.v4;

	Json::FastWriter writer;
	std::string strVersionString = writer.write(versinJson);
	return SetValue(kMainProfile, "version", strVersionString.c_str());
}

CStringW Profile::GetInstallPath( void )
{
	std::string path;
	GetValue(kMainProfile, "install_path", path);

	return CA2W(path.c_str());
}

bool Profile::SetInstallPath( const CStringW& strPath )
{
	std::string path = CW2A(strPath, CP_UTF8);
	return SetValue(kMainProfile, "install_path", path.c_str());
}

bool Profile::SetInstallTempPath( const CStringW& strTemp )
{
	std::string path = CW2A(strTemp, CP_UTF8);
	return SetValue(kMainProfile, "install_temp_path", path.c_str());
}

bool Profile::GetInstallTempPath( CStringW& strTempPath )
{
	std::string path;
	if (GetValue(kMainProfile, "install_temp_path", path)){
		strTempPath = CA2W(path.c_str());
		return true;
	}
	return false;
}

void Profile::DeleteInstallTempPath( void )
{
	DeleteValue(kMainProfile, "install_temp_path");
}


