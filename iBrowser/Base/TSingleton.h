// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef _T_SINGLETON_H__
#define _T_SINGLETON_H__

template<class T>
class TSingleton
{
public:
	static T* GetInstance()
	{
		if (NULL == m_Instance)
		{
			m_Instance = new T();
		}
		return m_Instance;
	}
	static void Destory()
	{
		if (m_Instance)
		{
			delete m_Instance;
			m_Instance = NULL;
		}
	}
	TSingleton(){};
	virtual ~TSingleton(){};

protected:
	static T* m_Instance;
};

template<class T>
T* TSingleton<T>::m_Instance = NULL;

#endif //_T_SINGLETON_H__