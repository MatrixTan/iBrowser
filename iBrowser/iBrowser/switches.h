#ifndef _SWITCHES_H__
#define _SWITCHES_H__

namespace switches{

static const WCHAR* kProcessType = L"type";
static const WCHAR* kProcessTypeMain = L"main";
static const WCHAR* kProcessTypeCore = L"core";

static const WCHAR* kTrue = L"true";

static const WCHAR* kHWND = L"hwnd";
static const WCHAR* kURL = L"url";
static const WCHAR* kCoreWaitDebugger = L"core_wait_debugger";

static const WCHAR* kProcessMode = L"process_mode";
static const WCHAR* kSingle = L"single";
static const WCHAR* kMultiple = L"multiple";

static const WCHAR* kProcessStartEvent = L"process_start_event";
static const WCHAR* kCoreProcessHostThread = L"host_thread";
static const WCHAR* kProcessHostHWND = L"host_hwnd";

static const WCHAR* kCrossProcessRender = L"cross_process_render";
}
#endif //_SWITCHES_H__