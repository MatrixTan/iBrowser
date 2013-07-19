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

static const char *kUIProfile = "ui.profile";

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


