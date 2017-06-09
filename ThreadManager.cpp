#include "ThreadManager.h"

using namespace::std;

ThreadManager::ThreadManager() : m_Producers(0), m_Consumers(0), m_Factory(new IFactory), m_Callback(new ICallback)
{
	// m_WinCapture->GetWindowList();
	// m_WinCapture->SetCaptureTarget("命令提示符");
	m_Factory->_winCapture->GetWindowList();
	m_Factory->_winCapture->SetCaptureTarget("命令提示符");
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
	m_Factory->_winCapture->StopCapture();
}

bool ThreadManager::ReStartTasks() {

	m_Factory->m_Sleep = false;
	m_Factory->Reset();

	return true;
}

void ThreadManager::SetCaptureCallback(ICallback* pCallback)
{
	pCallback = m_Factory->_pCallback;
	m_Callback = pCallback;
}

WINCAPTURE_ISCAPTURING ThreadManager::IsCapturing()
{
	return m_Factory->_winCapture->IsCapturing();
}

WResult ThreadManager::SetFPS(unsigned int uFPS)
{
	m_Factory->_ComsumerSleepTime = (unsigned int)1000 / uFPS;
	return m_Factory->_winCapture->SetFPS(uFPS);
}

WResult ThreadManager::SetCaptureTarget(std::string WinText)
{
	return m_Factory->_winCapture->SetCaptureTarget(WinText);
}

WResult ThreadManager::SetCaptureTarget(RECT rt, bool bFollowupCursor, POINT ptAnchor)
{
	return m_Factory->_winCapture->SetCaptureTarget(rt, bFollowupCursor, ptAnchor);
}

WResult ThreadManager::SetCaptureTarget(unsigned int x, unsigned int y, unsigned int width, unsigned int height, bool bFollowupCursor, POINT ptAnchor)
{
	return m_Factory->_winCapture->SetCaptureTarget(x, y, width, height, bFollowupCursor, ptAnchor);
}

void ThreadManager::EnableCursorDisplay(bool bDisplay)
{
	m_Factory->_winCapture->EnableCursorDisplay(bDisplay);
}

void ThreadManager::OnFinishedOneFrame(WINCAPTURE_FRAMEDATA* outFrameData)
{
	m_Factory->_winCapture->OnFinishedOneFrame(outFrameData);
}


