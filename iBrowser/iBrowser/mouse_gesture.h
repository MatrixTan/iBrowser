// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _MOUSE_GESTURE_H__
#define _MOUSE_GESTURE_H__

#include <atltypes.h>
#include <atlstr.h>


enum E_GESTURE_TYPE{
	EGT_LEFT = 0x01000000,
	EGT_RIGHT = 0x02000000,
	EGT_UP = 0x04000000,
	EGT_DOWN = 0x08000000,
	EGT_LEFT_RIGHT = 0x01020000,
	EGT_LEFT_UP = 0x01040000,
	EGT_LEFT_DOWN = 0x01080000,
	EGT_RIGHT_LEFT = 0x02010000,
	EGT_RIGHT_UP = 0x02040000,
	EGT_RIGHT_DOWN = 0x02080000,
	EGT_UP_LEFT = 0x04010000,
	EGT_UP_RIGHT = 0x04020000,
	EGT_UP_DOWN = 0x04080000,
	EGT_DOWN_LEFT = 0x08010000,
	EGT_DOWN_RIGHT = 0x08020000,
	EGT_DOWN_UP = 0x08040000,
	EGT_RIGHT_DOWN_RIGHT = 0x02080200,
};

class CoreView;
class MouseGesture : public CWindowImpl<MouseGesture>
{
public:
	BEGIN_MSG_MAP(MouseGesture)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBackGround)
		MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		MESSAGE_HANDLER(WM_RBUTTONUP, OnRButtonUp)
		MESSAGE_HANDLER(WM_MOUSELEAVE, OnMouseLeave)
	END_MSG_MAP()
	
	
	MouseGesture(CoreView* pCore);	
	~MouseGesture();
	void Start();
	void Stop();
	bool GetStarted(void);
	LRESULT OnEraseBackGround(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& bHandler);
	LRESULT OnMouseMove(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& bHandler);
	LRESULT OnRButtonUp(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& bHandler);
	LRESULT OnMouseLeave(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& bHandler);

	static const int kMinDistance = 10;
	static const int kDirectionCount = 4;

protected:

	void DrawTip(const CPoint& pt);
	void DoCommand(void);
private:
	enum E_DIRECTION{
		ED_None = 0,
		ED_LEFT = 1,
		ED_RIGHT = 2,
		ED_UP = 4,
		ED_DOWN = 8
	};

	CoreView* m_pCore;
	bool m_bStarted;
	bool m_bMouseTrack;
	CPoint m_ptLast;
	CPoint m_ptLastGesture;
	BYTE m_Directions[kDirectionCount];
	BYTE m_LastDirection;

	class Tip:public CWindowImpl<Tip>
	{
	public:
		Tip();
		~Tip();

		void Show();
		void Hide();
		void EnsureCreated(void);

		BEGIN_MSG_MAP(Tip)
			MESSAGE_HANDLER(WM_PAINT, OnPaint)
		END_MSG_MAP()

		LRESULT OnPaint(UINT Msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		void Draw(void);
		CString m_Text;
	};

	Tip m_Tip;
};

#endif //_MOUSE_GESTURE_H__