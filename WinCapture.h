#pragma once

#include "CommonTypes.h"

class WinCapture
{
private:
	// 成员
	WINCAPTURE_ISCAPTURING m_IsCapturing;		// 是否正在捕获
	WINCAPTURE_MODE m_CaptureMode;				// 捕获模式(全屏, 窗口)
	WINCAPTURE_WINLIST* m_WinList;						// 当前窗口列表
	WINCAPTURE_SETTING*	m_CaptureSetting;		// 相关捕获设置
	WINCAPTURE_FRAMEDATA* m_FrameData;		// 帧数据
	// 方法
	WResult _GetSnapShotByRect(RECT targetRECT);
			// BOOL CALLBACK _EnumWindowsProc(HWND hWnd, LPARAM lParam);
public:
	WinCapture();
	~WinCapture();

	WResult SetCaptureCallback();

	WResult StartCapture();

	WResult StopCapture();

	WINCAPTURE_ISCAPTURING IsCapturing();

	WResult SetFPS(unsigned int uFPS);

	WResult GetWindowList();

	void FreeWindowList();

	WResult SetCaptureTarget(std::string WinID);

	WResult SetCaptureTarget(RECT rt, bool bFollowupCursor = false, POINT ptAnchor = { 0, 0 });

	WResult SetCaptureTarget(unsigned int x, unsigned int y, unsigned int width, unsigned int height, bool bFollowupCursor = false, POINT ptAnchor = { 0, 0 });

	void EnableCursorDisplay(bool bDisplay);

	// callback
	void OnFinishedOneFrame(WINCAPTURE_FRAMEDATA* frameData);

};

