// WinCapture0.1.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "CommonTypes.h"
#include "ThreadManager.h"
#include "WinCapture.h"

#define TEST_TIME 100

int _tmain(int argc, _TCHAR* argv[])
{
	ThreadManager* threadManager = new ThreadManager();
	threadManager->EnableCursorDisplay(false);
	
	threadManager->AddProducer();
	threadManager->AddConsumer();
	threadManager->AddConsumer();

	OutputDebugString("�������\n");
	Sleep(20000);
	threadManager->StopProducers();

	/*Sleep(1000);
	threadManager->ReStartTasks();*/

	getchar();
	return 0;
}

