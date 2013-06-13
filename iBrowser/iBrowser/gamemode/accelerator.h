// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _GAME_MODE_ACCELERATOR_H__
#define _GAME_MODE_ACCELERATOR_H__
#include "..\APIHook.h"

namespace GameMode
{	

	typedef DWORD (WINAPI* PFunGetTickCount)(void);
	class Accelerator
	{
	public:
		static Accelerator* GetInstance();
		~Accelerator();

		bool SetRate(float fRate);
		float GetRate(void);
				
		static PFunGetTickCount Real_GetTickCount;
		static DWORD WINAPI HOOK_GetTickCount( void );
	protected:
	private:
		Accelerator();
		float m_fRate;
		DWORD m_dwStartTickCount;
		
		static Accelerator* s_Instance;
		static CRITICAL_SECTION s_cs;
	};

}


#endif //_GAME_MODE_ACCELERATOR_H__