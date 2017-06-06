#pragma once

#include "CommonTypes.h"
#include "WinCapture.h"

// --- �̹߳�����

typedef THREAD_FACTORY_TYPE T;

const unsigned int MAX_BUFNUM = 4;
static bool ThreadSleep = false;

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
public:
	void Reset() {
		m_repo.Init();
	}

	// �����¼�
	virtual void ProduceTask() {
		while (true && !ThreadSleep) {
			Sleep(1);
			unique_lock<mutex> lock(m_repo.mtxProduce);

			WinCapture* _winCapture = new WinCapture;
			T item = new WINCAPTURE_FRAMEDATA;
			_winCapture->StartCapture();
			_winCapture->OnFinishedOneFrame(item);
			
			cout << "p-id " << this_thread::get_id() << " is producing x = " << item->CursorPos->x << " y = " << item->CursorPos->y <<endl;
			m_ProduceItem(m_repo, item);

			lock.unlock();
		}
	}

	// �����¼�
	virtual void ConsumeTask() {
		while (true) {
			Sleep(1);
			unique_lock<mutex> lock(m_repo.mtxConsume);

			T item = m_ConsumeItem(m_repo);
			cout << "c-id " << this_thread::get_id() << " is consuming " << endl;

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
	WinCapture* m_WinCapture;
	FactoryBase* m_Factory;

	bool m_Sleep;
public:
	ThreadManager();
	~ThreadManager();

	// bool AddProducer( Factory<THREAD_FACTORY_TYPE>* args);
	bool AddProducer();

	// bool AddConsumer( Factory<THREAD_FACTORY_TYPE>* args);
	bool AddConsumer();

	void StopProducers();

	bool ReStartTasks();
};

