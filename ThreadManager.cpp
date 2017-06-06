#include "ThreadManager.h"

using namespace::std;

ThreadManager::ThreadManager() : m_Producers(0), m_Consumers(0), m_WinCapture(new WinCapture), m_Factory(new IFactory)
{
	m_WinCapture->GetWindowList();
	m_WinCapture->SetCaptureTarget("WinCaptureDemo - Microsoft Visual Studio");
}


ThreadManager::~ThreadManager()
{
}

bool ThreadManager::AddProducer() {
	
	thread* someProducer = new thread(&IFactory::ProduceTask, m_Factory);	// thread 对象不可以拷贝构造
	m_Producers.push_back(someProducer);

	return true;
}

bool ThreadManager::AddConsumer( ) {
	
	thread* someConsumer = new thread(&IFactory::ConsumeTask, m_Factory);
	m_Consumers.push_back(someConsumer);

	return true;
}

void ThreadManager::StopProducers() {
	m_Factory->m_Sleep = true;
	m_WinCapture->StopCapture();
}

bool ThreadManager::ReStartTasks() {

	m_Factory->m_Sleep = false;
	m_Factory->Reset();

	return true;
}

WResult ThreadManager::SetCaptureCallback()
{
	return m_WinCapture->SetCaptureCallback();
}

WINCAPTURE_ISCAPTURING ThreadManager::IsCapturing()
{
	return m_WinCapture->IsCapturing();
}

WResult ThreadManager::SetFPS(unsigned int uFPS)
{
	return m_WinCapture->SetFPS(uFPS);
}

WResult ThreadManager::SetCaptureTarget(std::string WinID)
{
	return m_WinCapture->SetCaptureTarget(WinID);
}

WResult ThreadManager::SetCaptureTarget(RECT rt, bool bFollowupCursor, POINT ptAnchor)
{
	return m_WinCapture->SetCaptureTarget(rt, bFollowupCursor, ptAnchor);
}

WResult ThreadManager::SetCaptureTarget(unsigned int x, unsigned int y, unsigned int width, unsigned int height, bool bFollowupCursor, POINT ptAnchor)
{
	return m_WinCapture->SetCaptureTarget( x, y, width, height, bFollowupCursor, ptAnchor);
}

void ThreadManager::EnableCursorDisplay(bool bDisplay)
{
	m_WinCapture->EnableCursorDisplay(bDisplay);
}