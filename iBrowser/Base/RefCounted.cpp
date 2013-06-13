// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "RefCounted.h"
#include <Windows.h>

namespace base
{

	void CRefCountedBase::AddRef()
	{
		++ m_nRefCount;
	}

	bool CRefCountedBase::Release()
	{
		if (0 == --m_nRefCount)
			return true;
		else
			return false;
	}

	CRefCountedBase::CRefCountedBase()
		: m_nRefCount(0)
	{
	}

	CRefCountedBase::~CRefCountedBase()
	{

	}

	CRefCountedThreadSafeBase::CRefCountedThreadSafeBase()
		: m_nRefCount(0)
	{

	}

	CRefCountedThreadSafeBase::~CRefCountedThreadSafeBase()
	{

	}

	void CRefCountedThreadSafeBase::AddRef()
	{
		InterlockedExchangeAdd(reinterpret_cast<volatile long*>(&m_nRefCount), 1);
	}

	// Returns true if the object should self-delete.
	bool CRefCountedThreadSafeBase::Release()
	{
		long nNewValue = InterlockedExchangeAdd(
			reinterpret_cast<volatile long*>(&m_nRefCount), -1) - 1;

		if (0 == nNewValue)
			return true;
		else
			return false;
	}

}