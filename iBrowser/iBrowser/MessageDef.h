// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _MESSAGE_DEF_H__
#define _MESSAGE_DEF_H__

///自定义窗口消息的起始
#define WM_CUSTOM_BASE WM_USER+1985
///用户提交主输入框的输入
#define WM_MAINEDIT_SUBMIT WM_CUSTOM_BASE
///显示操作窗口
#define WM_SHOW_OPERATION_PANEL WM_CUSTOM_BASE+1
///Open the Console Window


#define WM_SHOW_CONSOLE_WINDOW WM_CUSTOM_BASE+13
///Delegate Message Frome IEEventDelegate
#define WM_EVENT_DELEGATE_MESSAGE WM_CUSTOM_BASE+14
///内核调用消息
#define WM_CORE_INVOKE WM_CUSTOM_BASE+17
#define WM_CORE_NAVIGATE WM_CUSTOM_BASE+18
#define WM_CORE_REFRESH WM_CUSTOM_BASE+19
#define WM_CORE_GOBACK WM_CUSTOM_BASE+20
#define WM_CORE_GOFORWARD WM_CUSTOM_BASE+21

///通知父窗口子窗口创建完毕
#define WM_CHILD_WINDOW_CREATED WM_CUSTOM_BASE+21
enum E_CHILEWINDOW_CREATE_FLAG
{
	ECCF_CreateNew =0,
	ECCF_NewWindow
};


///点击添加Tab消息
#define WM_CLICK_ADD_TAB WM_CUSTOM_BASE+31
///点击Tab消息
#define WM_CLICK_TAB WM_CUSTOM_BASE+32
///关闭Tab
#define WM_CLOSE_TAB WM_CUSTOM_BASE+33


///快捷键通知给MainFrame
#define WM_HOTKEY_NOTIFY WM_CUSTOM_BASE+40
#define HOTKEY_ShowMaskWindow 1
#define HOTKEY_Refresh 2
///IE内核通知给主Frame的时间
#define WM_EVENT_NOTIFY WM_CUSTOM_BASE+41
#define EVENT_NewWindow3 1


/////////////////////

#define WM_CORE_PROCESS_CREATED WM_CUSTOM_BASE+1001
#define WM_CORE_PROCESS_HOST_READY WM_CUSTOM_BASE+1002
#define WM_CREATE_CORE WM_CUSTOM_BASE+1003
#define WM_BEFORE_NAVIGATE WM_CUSTOM_BASE+1004
#define WM_TITLE_CHANGE WM_CUSTOM_BASE+1005

///For Test
#define WM_IE_FORTEST WM_CUSTOM_BASE+5000

#endif //_MESSAGE_DEF_H__
