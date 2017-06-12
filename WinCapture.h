#pragma once

#include "CommonTypes.h"

class WinCapture
{
private:
	bool m_bModeFirstTime;
protected:
	// ��Ա, ��Ԫ���Է���
	WINCAPTURE_ISCAPTURING m_IsCapturing;		// �Ƿ����ڲ���
	int m_CaptureMode;				// ����ģʽ(ȫ��, ����)
	WINCAPTURE_WINLIST* m_WinList;						// ��ǰ�����б�
	WINCAPTURE_SETTING*	m_CaptureSetting;		// ��ز�������
	WINCAPTURE_FRAMEDATA* m_FrameData;		// ֡����
	// ����
	WResult _GetSnapShotByRect(RECT targetRECT);
public:
	WinCapture();
	~WinCapture();

	friend class ThreadManager;
	friend class FactoryBase;

	WResult SetCaptureCallback(WINCAPTURE_FRAMEDATA* pwcFrameData); 

	WResult StartCapture(); // p

	WResult StopCapture(); // p

	WINCAPTURE_ISCAPTURING IsCapturing();

	WResult SetFPS(unsigned int uFPS);

	WResult GetWindowList(); // p

	void FreeWindowList(); // pi

	WResult SetCaptureTarget(std::string WinText);

	WResult SetCaptureTarget(RECT rt, bool bFollowupCursor = false, POINT ptAnchor = { 0, 0 });

	WResult SetCaptureTarget(unsigned int x, unsigned int y, unsigned int width, unsigned int height, bool bFollowupCursor = false, POINT ptAnchor = { 0, 0 });

	void EnableCursorDisplay(bool bDisplay);

	// callback
	void OnFinishedOneFrame(WINCAPTURE_FRAMEDATA* frameData);

	void OnCapturedFrameAvailable(WINCAPTURE_FRAMEDATA* userFrameData, UINT64 uTimeStamp, POINT* ptMouse);

};

