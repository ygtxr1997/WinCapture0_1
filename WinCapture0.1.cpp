// WinCapture0.1.cpp : 定义控制台应用程序的入口点。
//

#include "C-Bridge.h"

#define TEST_TIME 100

int _tmain(int argc, _TCHAR* argv[])
{
	//ThreadManager* threadManager = new ThreadManager();
	//threadManager->EnableCursorDisplay(false);
	
	ZEGO_InitWinCapture();
	ZEGO_AddProducer();
	ZEGO_AddProducer();
	ZEGO_AddConsumer();

	// threadManager->AddConsumer();

	OutputDebugString("任务完成\n");
	/*Sleep(20000);
	threadManager->StopProducers();*/

	/*Sleep(1000);
	threadManager->ReStartTasks();*/

	getchar();
	return 0;
}

