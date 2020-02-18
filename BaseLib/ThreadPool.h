#pragma once
#ifndef __BASELIB_THREAD_POOL_H__
#define __BASELIB_THREAD_POOL_H__

#include <functional>
#include <queue>
#include <list>
#include <thread>
#include <mutex>
#include "Singleton.h"

namespace Util {
	class ThreadPool : public Singleton<ThreadPool>{
	public:
		void EnqueueJob(std::function<void(void)>);
		void Initialize();

	private:
		DECLARE_SINGLETON(ThreadPool)

		void WorkerThread();
		void AllocationThread();

		const size_t m_cpuCoresCount = std::thread::hardware_concurrency();

		std::mutex m_mutex;
		std::condition_variable m_condition;

		bool m_allStopFlag = false;
		size_t m_threadCount = 0;
		size_t m_runningThreadCount = 0;

		std::queue<std::function<void(void)> > m_jobQueue;
		std::list<std::thread> m_threadList;
	};
}

#endif // !__BASELIB_THREAD_POOL_H__
