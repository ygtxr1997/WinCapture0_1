#pragma once

#include "CommonTypes.h"

class WinCapture
{
private:
	// ��Ա
	WINCAPTURE_ISCAPTURING m_IsCapturing;		// �Ƿ����ڲ���
	WINCAPTURE_MODE m_CaptureMode;				// ����ģʽ(ȫ��, ����)
	WINCAPTURE_WINLIST* m_WinList;						// ��ǰ�����б�
	WINCAPTURE_SETTING*	m_CaptureSetting;		// ��ز�������
	WINCAPTURE_FRAMEDATA* m_FrameData;		// ֡����
	// ����
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

