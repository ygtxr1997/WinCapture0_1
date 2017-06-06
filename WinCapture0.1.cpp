// WinCapture0.1.cpp : 定义控制台应用程序的入口点。
//

#include "CommonTypes.h"
#include "ThreadManager.h"
#include "WinCapture.h"

#define TEST_TIME 100

int _tmain(int argc, _TCHAR* argv[])
{
	ThreadManager* threadManager = new ThreadManager();
	// class Factory<THREAD_FACTORY_TYPE>* myFactory = new Factory<THREAD_FACTORY_TYPE>;
	
	threadManager->AddProducer();
	threadManager->AddConsumer();
	threadManager->AddConsumer();

	/*WinCapture* winCapture = new WinCapture;
	winCapture->GetWindowList();
	winCapture->SetCaptureTarget("WinCaptureDemo - Microsoft Visual Studio");
	for (int i = 0; i < TEST_TIME; ++i)
		winCapture->StartCapture();*/

	OutputDebugString("任务完成\n");
	Sleep(5000);
	threadManager->StopProducers();
	getchar();
	return 0;
}

