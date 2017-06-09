// WinCapture0.1.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "C-Bridge.h"

#define TEST_TIME 100

int _tmain(int argc, _TCHAR* argv[])
{
	//ThreadManager* threadManager = new ThreadManager();
	//threadManager->EnableCursorDisplay(false);
	ICallback* pCallback = new ICallback;
	
	ZEGO_InitWinCapture();

	ZEGO_AddProducer();
	ZEGO_AddProducer();
	ZEGO_AddConsumer();

	bool bRes = ZEGO_IsCapturing();

	ZEGO_SetFPS(1);

	ZEGO_SetCaptureTargetByWinText("������ʾ��");
	//RECT testRECT = { 0, 0, 1366, 768 };
	//POINT testAnchor = { 0, 0 };
	//ZEGO_SetCaptureTargetByRECT(testRECT, false, testAnchor);
	//ZEGO_SetCaptureTargetByXYWH(0, 0, 1920, 1080);
	
	ZEGO_SetEnableCursorDisplay(true);

	// WINCAPTURE_FRAMEDATA* outFrameData = new WINCAPTURE_FRAMEDATA;
	// ZEGO_OnFinishedOneFrame(outFrameData);


	/*ZEGO_SetCaptureCallback(pCallback);*/
	ZEGO_SetCaptureCallback(pCallback);


	OutputDebugString("�������\n");
	/*Sleep(20000);
	threadManager->StopProducers();*/

	/*Sleep(1000);
	threadManager->ReStartTasks();*/

	getchar();
	return 0;
}

