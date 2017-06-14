// WinCapture0.1.cpp : 定义控制台应用程序的入口点。
//

#include "Bridge-WinCapture.h"

#define TEST_TIME 100

int _tmain(int argc, _TCHAR* argv[])
{
	ICallback* pCallback = new ICallback;
	
	ZEGO::Capture::InitWinCapture();

	ZEGO::Capture::AddProducer();
	ZEGO::Capture::AddConsumer();

	bool bRes = ZEGO::Capture::IsCapturing();

	ZEGO::Capture::SetFPS(20);

	// ZEGO::Capture::SetCaptureTargetByWinText("DesktopDuplication - Microsoft Visual Studio");
	ZEGO::Capture::SetCaptureTargetByWinText("命令提示符");
	// ZEGO::Capture::SetCaptureTargetByWinText("learnZego.farbox.com/test.txt");
	// ZEGO::Capture::SetCaptureTargetByWinText("图片");
	// ZEGO::Capture::SetCaptureTargetByWinText("新标签页 - Google Chrome");
	// ZEGO::Capture::SetCaptureTargetByWinText("PowerPoint 幻灯片放映 - [演示文稿1]");
	//RECT testRECT = { 0, 0, 1366, 768 };
	//POINT testAnchor = { 0, 0 };
	//ZEGO::Capture::SetCaptureTargetByRECT(testRECT, false, testAnchor);
	//ZEGO::Capture::SetCaptureTargetByXYWH(0, 0, 1920, 1080);
	
	ZEGO::Capture::SetEnableCursorDisplay(false);

	WINCAPTURE_WINLIST* testList = new WINCAPTURE_WINLIST;
	ZEGO::Capture::GetWindowList(testList);
	
	/*for (unsigned int i = 0; i < testList->Title.size(); i++)
	{
		cout << testList->Title[i] << endl;
	}*/

	ZEGO::Capture::SetCaptureCallback(&pCallback);
	// ZEGO::Capture::SetCaptureCallback(pCallback);


	OutputDebugString("任务完成\n");
	/*Sleep(20000);
	threadManager->StopProducers();*/

	/*Sleep(1000);
	threadManager->ReStartTasks();*/

	/*while (1) {
		Sleep(10);
		cout << "pCallback时间戳为 " << pCallback->uTimeStamp << endl;
	}*/

	getchar();
	return 0;
}

