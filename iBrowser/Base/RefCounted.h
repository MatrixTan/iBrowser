// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _BASE_REF_COUNTED_H__
#define _BASE_REF_COUNTED_H__

namespace base
{

	class CRefCountedBase
	{
	protected:
		void AddRef();

		// Returns true if the object should self-delete.
		bool Release();

		CRefCountedBase();
		~CRefCountedBase();

	private:
		int m_nRefCount;

	private:
		CRefCountedBase(const CRefCountedBase & right);
		CRefCountedBase & operator =(const CRefCountedBase & right);
	};

	class CRefCountedThreadSafeBase
	{
	protected:
		CRefCountedThreadSafeBase();
		virtual ~CRefCountedThreadSafeBase();

		void AddRef();

		// Returns true if the object should self-delete.
		bool Release();

	private:
		int m_nRefCount;

	private:
		CRefCountedThreadSafeBase(const CRefCountedThreadSafeBase & right);
		CRefCountedThreadSafeBase & operator =(const CRefCountedThreadSafeBase & right);
	};

	template<class T>
	class RefCounted : public CRefCountedBase
	{
	public:
		RefCounted(void);
		virtual ~RefCounted(void);

		void AddRef() {
			CRefCountedBase::AddRef();
		}

		void Release() {
			if (CRefCountedBase::Release()) {
				delete static_cast<T*>(this);
			}
		}

	private:
		RefCounted(const RefCounted<T> & right);
		RefCounted<T> & operator = (const RefCounted<T> & right);
	};

	template <class T>
	class CRefCountedThreadSafe : public CRefCountedThreadSafeBase 
	{
	public:
		CRefCountedThreadSafe() { }
		~CRefCountedThreadSafe() { }

		virtual void AddRef() 
		{
			CRefCountedThreadSafeBase::AddRef();
		}

		virtual void Release() 
		{
			if (CRefCountedThreadSafeBase::Release()) 
			{
				delete static_cast<T*>(this);
			}
		}

	private:
		CRefCountedThreadSafe(const CRefCountedThreadSafe<T> & right);
		CRefCountedThreadSafe<T> & operator = (const CRefCountedThreadSafe<T> & right);
	};

#define NON_VIRTUAL
	class CFakeRefCounted
	{
	public:
		NON_VIRTUAL void AddRef(){};
		NON_VIRTUAL void Release(){};
		static bool ImplementsThreadSafeReferenceCounting() {return true;}
	};

	template <class T>
	class CScopedRefPtr {
	public:
		CScopedRefPtr() : m_ptr(NULL) {
		}

		CScopedRefPtr(T* p) : m_ptr(p) {
			if (m_ptr)
				m_ptr->AddRef();
		}

		CScopedRefPtr(const CScopedRefPtr<T>& r) : m_ptr(r.m_ptr) {
			if (m_ptr)
				m_ptr->AddRef();
		}

		template <typename U>
		CScopedRefPtr(const CScopedRefPtr<U>& r) : m_ptr(r.get()) {
			if (m_ptr)
				m_ptr->AddRef();
		}

		~CScopedRefPtr() {
			if (m_ptr)
				m_ptr->Release();
		}

		T* get() const { return m_ptr; }
		operator T*() const { return m_ptr; }
		T* operator->() const { return m_ptr; }

		// 	T** operator&()
		// 	{
		// 		ATLASSERT(m_ptr==NULL);
		// 		return &m_ptr;
		// 	}

		// Release a pointer.
		// The return value is the current pointer held by this object.
		// If this object holds a NULL pointer, the return value is NULL.
		// After this operation, this object will hold a NULL pointer,
		// and will not own the object any more.
		T* release() {
			T* retVal = m_ptr;
			m_ptr = NULL;
			return retVal;
		}

		CScopedRefPtr<T>& operator=(T* p) {
			// AddRef first so that self assignment should work
			if (p)
				p->AddRef();
			if (m_ptr )
				m_ptr ->Release();
			m_ptr = p;
			return *this;
		}

		void Reset(T* p = NULL)
		{
			if(p != m_ptr)
			{
				enum { TypeMustBeComplete = sizeof(T) };

				if(NULL != m_ptr)
				{
					m_ptr->Release();
				}

				m_ptr = p;
			}
		}

		CScopedRefPtr<T>& operator=(const CScopedRefPtr<T>& r) {
			return *this = r.m_ptr;
		}

		template <typename U>
		CScopedRefPtr<T>& operator=(const CScopedRefPtr<U>& r) {
			return *this = r.get();
		}

		void swap(T** pp) {
			T* p = m_ptr;
			m_ptr = *pp;
			*pp = p;
		}

		void swap(CScopedRefPtr<T>& r) {
			swap(&r.m_ptr);
		}

		bool CreateInstance()
		{
			ATLASSERT(m_ptr == NULL);
			m_ptr = new T();
			if (m_ptr != NULL)
			{
				m_ptr->AddRef();
				return true;
			}				
			return false;
		}

	public:
		T* m_ptr;
	};

	template <typename T>
	CScopedRefPtr<T> make_scoped_refptr(T* t) {
		return CScopedRefPtr<T>(t);
	}

	template<class T>
	RefCounted<T>::RefCounted(void)
	{
	}

	template<class T>
	RefCounted<T>::~RefCounted(void)
	{
	}
}

#endif  //_BASE_REF_COUNTED_H__