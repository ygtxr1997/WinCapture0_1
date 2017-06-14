// WinCapture0.1.cpp : �������̨Ӧ�ó������ڵ㡣
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
	ZEGO::Capture::SetCaptureTargetByWinText("������ʾ��");
	// ZEGO::Capture::SetCaptureTargetByWinText("learnZego.farbox.com/test.txt");
	// ZEGO::Capture::SetCaptureTargetByWinText("ͼƬ");
	// ZEGO::Capture::SetCaptureTargetByWinText("�±�ǩҳ - Google Chrome");
	// ZEGO::Capture::SetCaptureTargetByWinText("PowerPoint �õ�Ƭ��ӳ - [��ʾ�ĸ�1]");
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


	OutputDebugString("�������\n");
	/*Sleep(20000);
	threadManager->StopProducers();*/

	/*Sleep(1000);
	threadManager->ReStartTasks();*/

	/*while (1) {
		Sleep(10);
		cout << "pCallbackʱ���Ϊ " << pCallback->uTimeStamp << endl;
	}*/

	getchar();
	return 0;
}

