// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "stdafx.h"
#include "accelerator.h"

namespace GameMode{

	CRITICAL_SECTION Accelerator::s_cs;
	DWORD Accelerator::HOOK_GetTickCount( void )
	{
		float rate = Accelerator::GetInstance()->GetRate();
		if (rate - 1.0f > 0.0000001 || rate - 1.0f < -0.000001){
			DWORD result = Real_GetTickCount();
			return result * rate;
		}else
			return Real_GetTickCount();
		
	}

	PFunGetTickCount Accelerator::Real_GetTickCount = NULL;

	Accelerator* Accelerator::s_Instance = NULL;

	Accelerator::Accelerator()
		:m_fRate(1.0f)
		,m_dwStartTickCount(0)
	{
		::InitializeCriticalSection(&s_cs);	

	}

	Accelerator::~Accelerator(){

	}

	bool Accelerator::SetRate( float fRate ){
		if (Real_GetTickCount == NULL){
			return false;
		}
		::EnterCriticalSection(&s_cs);
		m_dwStartTickCount = Real_GetTickCount();
		m_fRate = fRate;
		::LeaveCriticalSection(&s_cs);
		return true;
	}

	float Accelerator::GetRate( void ){
		::EnterCriticalSection(&s_cs);
		float f = m_fRate;
		::LeaveCriticalSection(&s_cs);
		return f;
	}

	Accelerator* Accelerator::GetInstance(){
		if (NULL == s_Instance){
			s_Instance = new Accelerator();
		}
		return s_Instance;
	}
}