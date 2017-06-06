#pragma once

/******************************
*	一些公共类型在此定义      *
*******************************/

#include <stdio.h>
#include <tchar.h>

#include <deque>
#include <vector>
#include <string>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <Windows.h>
#include <iostream>

#include <time.h>

#define	WINCAPTURE_MAX_WIND	200
#define	WINCAPTURE_MAX_FPS		220

//
// 异常处理
//
enum WResult
{
	WINCAPTURE_SUCCESS = 0,
	WINCAPTURE_ERROR_EXPECTED = 1,
	WINCAPTURE_ERROR_UNEXPECTED = 2
};


//
// 是否正在捕获
//
enum WINCAPTURE_ISCAPTURING
{
	WINCAPTURE_TURE = 1,
	WINCAPTURE_FALSE = 0
};

//
// 捕获方式
//
enum WINCAPTURE_MODE
{
	WINCAPTURE_MODE_FULLSCREEN = 0,
	WINCAPTURE_MODE_WINID,
	WINCAPTURE_MODE_RECT,
	WINCAPTURE_MODE_XYWH
};

//
// 窗口列表, 包括句柄和标题
//
struct WINCAPTURE_WINLIST
{
	std::vector<std::string>		Handle = std::vector<std::string>(0, "");
	std::vector<std::string>		Title = std::vector<std::string>(0, "");
};

//
// 捕获设置, 例如FPS, 目标矩形, 是否绘制鼠标指针
//
struct WINCAPTURE_SETTING
{
	WINCAPTURE_SETTING() : TimeStamp(0),
		FPS(15),
		WinID(new HWND),
		IsFollowCursor(true),
		TargetRect({ 0, 0, 0, 0 }),
		Anchor({ 0 }),
		IsDisplay(true)
	{}

	UINT64		TimeStamp;
	UINT		FPS;
	void*		WinID;
	bool			IsFollowCursor;
	RECT		TargetRect;
	POINT		Anchor;
	bool			IsDisplay;
};

//
// 帧信息
//
struct WINCAPTURE_FRAMEDATA
{
	WINCAPTURE_FRAMEDATA() : Data(new BYTE), BytesPerLine(32), CursorPos(new POINT)
	{}

	BYTE*		Data;
	UINT		BytesPerLine;
	POINT*		CursorPos;
};

//
// 线程工厂的item类型
//
typedef  WINCAPTURE_FRAMEDATA*  THREAD_FACTORY_TYPE;