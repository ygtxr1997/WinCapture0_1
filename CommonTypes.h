#pragma once

/******************************
*	һЩ���������ڴ˶���      *
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

#define	WINCAPTURE_SAVE_FILE		false

//
// �쳣����
//
enum WResult
{
	WINCAPTURE_SUCCESS = 0,
	WINCAPTURE_ERROR_EXPECTED = 1,
	WINCAPTURE_ERROR_UNEXPECTED = 2
};


//
// �Ƿ����ڲ���
//
enum WINCAPTURE_ISCAPTURING
{
	WINCAPTURE_TURE = 1,
	WINCAPTURE_FALSE = 0
};

//
// ����ʽ
//
enum WINCAPTURE_MODE
{
	WINCAPTURE_MODE_FULLSCREEN = 0,
	WINCAPTURE_MODE_WINID,
	WINCAPTURE_MODE_RECT,
	WINCAPTURE_MODE_XYWH
};

//
// �����б�, ��������ͱ���
//
struct WINCAPTURE_WINLIST
{
	std::vector<std::string>		Handle = std::vector<std::string>(0, "");
	std::vector<std::string>		Title = std::vector<std::string>(0, "");
};

//
// ��������, ����FPS, Ŀ�����, �Ƿ�������ָ��
//
struct WINCAPTURE_SETTING
{
	/*WINCAPTURE_SETTING() : TimeStamp(0),
		FPS(15),
		WinID(NULL),
		IsFollowCursor(true),
		TargetRect({ 0, 0, 0, 0 }),
		Anchor({ 0 }),
		IsDisplay(true)
	{}*/

	UINT64		TimeStamp;
	UINT		FPS;
	HWND		WinID;
	bool			IsFollowCursor;
	RECT		TargetRect;
	POINT		Anchor;
	bool			IsDisplay;
};

//
// ֡��Ϣ
//
struct WINCAPTURE_FRAMEDATA
{
	WINCAPTURE_FRAMEDATA() : pData(new BYTE), BytesPerLine(32), CursorPos(new POINT)
	{}

	BYTE*		pData;
	UINT64		uSize = 0;
	UINT		BytesPerLine;
	POINT*		CursorPos;
};


//
// �ص�ָ��
//
struct ICallback
{
	WINCAPTURE_FRAMEDATA* pwcFrameData;
	UINT64* uTimeStamp;
	POINT* ptMouse;
};

//
// �̹߳�����item����
//
typedef  WINCAPTURE_FRAMEDATA*  THREAD_FACTORY_TYPE;