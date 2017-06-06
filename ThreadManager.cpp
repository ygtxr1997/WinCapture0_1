#include "ThreadManager.h"

using namespace::std;

ThreadManager::ThreadManager() : m_Producers(0), m_Consumers(0), m_WinCapture(new WinCapture), m_Factory(new Factory<THREAD_FACTORY_TYPE>), m_Sleep(false)
{
	m_WinCapture->GetWindowList();
	m_WinCapture->SetCaptureTarget("WinCaptureDemo - Microsoft Visual Studio");
}


ThreadManager::~ThreadManager()
{
}

bool ThreadManager::AddProducer() {
	
	thread* someProducer = new thread(&Factory<THREAD_FACTORY_TYPE>::ProduceTask, m_Factory);	// thread 对象不可以拷贝构造
	m_Producers.push_back(someProducer);

	return true;
}

bool ThreadManager::AddConsumer( ) {
	
	thread* someConsumer = new thread(&Factory<THREAD_FACTORY_TYPE>::ConsumeTask, m_Factory);
	m_Consumers.push_back(someConsumer);

	return true;
}

bool ThreadManager::ReStartTasks() {

	for (unsigned int i = 0; i < m_Producers.size(); i++) {
		m_Producers[i]->join();
	}
	for (unsigned int i = 0; i < m_Consumers.size(); i++) {
		m_Consumers[i]->join();
	}

	return true;
}

void ThreadManager::StopProducers() {
	ThreadSleep = true;
}