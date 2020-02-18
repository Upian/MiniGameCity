#include "ThreadPool.h"

namespace Util {
	using Lock = std::unique_lock<std::mutex>;

	ThreadPool::ThreadPool() {

	}
	ThreadPool::~ThreadPool() {
		m_allStopFlag = true;
		m_condition.notify_all();
		m_runningThreadCount = 0;
		for (auto& worker : m_threadList) {
			worker.join();
		}
		
	}

	void ThreadPool::EnqueueJob(std::function<void(void)> job) {
		Lock lock(m_mutex);

		m_jobQueue.push(job);
		m_condition.notify_one();
	}

	void ThreadPool::Initialize() {
		this->AllocationThread();
	}

	void ThreadPool::WorkerThread() {
		while (true) {
			Lock lock(m_mutex);
			m_condition.wait(lock, [this]()->bool {
				return (false == m_jobQueue.empty()) || (true == m_allStopFlag);
			});
			if (true == m_jobQueue.empty() && true == m_allStopFlag)
				return;

			std::function<void(void)> job = std::move(m_jobQueue.front());
			m_jobQueue.pop();
			++m_runningThreadCount;
			lock.unlock();

			job(); //Do work

			lock.lock();
			--m_runningThreadCount;
		}
	}
	void ThreadPool::AllocationThread() {
		for (int count = 0; count < 2 * m_cpuCoresCount; ++count) {
			m_threadList.emplace_back([this]()->void { this->WorkerThread(); });
		}
	}
}


