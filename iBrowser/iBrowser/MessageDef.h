// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _MESSAGE_DEF_H__
#define _MESSAGE_DEF_H__

///�Զ��崰����Ϣ����ʼ
#define WM_CUSTOM_BASE WM_USER+1985
///�û��ύ������������
#define WM_MAINEDIT_SUBMIT WM_CUSTOM_BASE
///��ʾ��������
#define WM_SHOW_OPERATION_PANEL WM_CUSTOM_BASE+1
///Open the Console Window


#define WM_SHOW_CONSOLE_WINDOW WM_CUSTOM_BASE+13
///Delegate Message Frome IEEventDelegate
#define WM_EVENT_DELEGATE_MESSAGE WM_CUSTOM_BASE+14
///�ں˵�����Ϣ
#define WM_CORE_INVOKE WM_CUSTOM_BASE+17
#define WM_CORE_NAVIGATE WM_CUSTOM_BASE+18
#define WM_CORE_REFRESH WM_CUSTOM_BASE+19
#define WM_CORE_GOBACK WM_CUSTOM_BASE+20
#define WM_CORE_GOFORWARD WM_CUSTOM_BASE+21

///֪ͨ�������Ӵ��ڴ������
#define WM_CHILD_WINDOW_CREATED WM_CUSTOM_BASE+21
enum E_CHILEWINDOW_CREATE_FLAG
{
	ECCF_CreateNew =0,
	ECCF_NewWindow
};


///������Tab��Ϣ
#define WM_CLICK_ADD_TAB WM_CUSTOM_BASE+31
///���Tab��Ϣ
#define WM_CLICK_TAB WM_CUSTOM_BASE+32
///�ر�Tab
#define WM_CLOSE_TAB WM_CUSTOM_BASE+33


///��ݼ�֪ͨ��MainFrame
#define WM_HOTKEY_NOTIFY WM_CUSTOM_BASE+40
#define HOTKEY_ShowMaskWindow 1
#define HOTKEY_Refresh 2
///IE�ں�֪ͨ����Frame��ʱ��
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
