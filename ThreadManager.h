#pragma once

#include "CommonTypes.h"
#include "WinCapture.h"

// --- �̹߳�����

typedef THREAD_FACTORY_TYPE T;

const unsigned int MAX_BUFNUM = 4;

using namespace::std;

//
// �����������ߵĹ�����Դ��
//
template<typename Type>
class Respository {
public:
	deque<Type> itemsBuff;
	mutex	mtx;
	mutex	mtxProduce;
	mutex	mtxConsume;
	condition_variable REPO_NOT_FULL;
	condition_variable REPO_NOT_EMPTY;

	// ����
	Respository() : itemsBuff(deque<Type>(0)) {};

	// ��ʼ��
	void Init() {
		itemsBuff.clear();
	}
};

// 
// ��������
//
class FactoryBase
{
private:
	Respository<T> m_repo;
	friend ThreadManager;

	// ����һ������
	void m_ProduceItem(Respository<T> &m_repo, T item) {
		unique_lock<mutex> lock(m_repo.mtx);
		while (m_repo.itemsBuff.size() == MAX_BUFNUM) {
			cout << "Respository is full, waiting ..." << endl;
			(m_repo.REPO_NOT_FULL).wait(lock);
		}
		m_repo.itemsBuff.push_back(item);
		(m_repo.REPO_NOT_EMPTY).notify_all();
		lock.unlock();
	}

	// ����һ������
	T m_ConsumeItem(Respository<T> &m_repo) {
		unique_lock<mutex> lock(m_repo.mtx);
		while (m_repo.itemsBuff.size() == 0) {
			cout << "Respository is empty, waiting ..." << endl;
			(m_repo.REPO_NOT_EMPTY).wait(lock);
		}
		T data;
		if (m_repo.itemsBuff.size()) {
			data = m_repo.itemsBuff.front();
			m_repo.itemsBuff.pop_front();
		}
		else
			data = NULL;
		(m_repo.REPO_NOT_FULL).notify_all();
		lock.unlock();
		return data;
	}

protected:
	ICallback* _pCallback;
	WinCapture* _winCapture = new WinCapture;
	UINT _ComsumerSleepTime = 51;
	T item;

public:
	bool m_Sleep;
	
	FactoryBase() : _pCallback(new ICallback), m_Sleep(false), item(new WINCAPTURE_FRAMEDATA) {
		// ��ȫ��ʼ��
		_pCallback->pwcFrameData = new WINCAPTURE_FRAMEDATA;
		_pCallback->uTimeStamp = new UINT64;
		_pCallback->ptMouse = new POINT;

	}

	void Reset() {
		m_repo.Init();
	}

	// �����¼�
	virtual void ProduceTask() {
		while (true) {
			if (m_Sleep) continue;
			Sleep(1);
			unique_lock<mutex> lock(m_repo.mtxProduce);

			_winCapture->m_CaptureSetting->IsDisplay = false;
			// _winCapture->SetCaptureTarget(RECT{ 0, 0, 1366, 768 }, 0, {0, 0});
			_winCapture->SetCaptureTarget("������ʾ��");
			
			_winCapture->StartCapture();
			_winCapture->OnFinishedOneFrame(item);
			_winCapture->OnCapturedFrameAvailable(_pCallback->pwcFrameData, *(_pCallback->uTimeStamp), _pCallback->ptMouse);
			
			cout << "p-id " << this_thread::get_id() << " is producing x = " << item->CursorPos->x << " y = " << item->CursorPos->y <<endl;
			m_ProduceItem(m_repo, item);

			lock.unlock();
		}
	}

	// �����¼�
	virtual void ConsumeTask() {
		while (true) {
			Sleep(_ComsumerSleepTime);
			unique_lock<mutex> lock(m_repo.mtxConsume);

			T item = m_ConsumeItem(m_repo);
			cout << "c-id " << this_thread::get_id() << " is consuming " << endl;

			static int i = 0;
			i++;
			string output("���ѳɹ�");
			char string[5];
			_itoa_s(i, string, 10);
			for (int j = 0; j < 5; j++)
				output.push_back(string[j]);
				OutputDebugString(output.c_str()); OutputDebugString("\n");

			lock.unlock();
		}
	}

	// ����
	virtual ~FactoryBase() {

	};
};

class IFactory : public FactoryBase
{
};

class ThreadManager
{
private:
	std::vector<thread*> m_Producers;
	std::vector<thread*> m_Consumers;
	FactoryBase* m_Factory;
	ICallback* m_Callback;

public:
	ThreadManager();
	~ThreadManager();

	bool AddProducer();

	bool AddConsumer();

	void StopProducers();

	bool ReStartTasks();

	void SetCaptureCallback(ICallback* pCallback);

	WINCAPTURE_ISCAPTURING IsCapturing();

	WResult SetFPS(unsigned int uFPS);

	WResult SetCaptureTarget(std::string WinText);

	WResult SetCaptureTarget(RECT rt, bool bFollowupCursor = false, POINT ptAnchor = { 0, 0 });

	WResult SetCaptureTarget(unsigned int x, unsigned int y, unsigned int width, unsigned int height, bool bFollowupCursor = false, POINT ptAnchor = { 0, 0 });

	void EnableCursorDisplay(bool bDisplay);

	void  OnFinishedOneFrame(WINCAPTURE_FRAMEDATA* outFrameData);

};

