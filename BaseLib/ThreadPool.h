#pragma once
#ifndef __BASELIB_THREAD_POOL_H__
#define __BASELIB_THREAD_POOL_H__

#include <tuple>
#include <thread>

#include "Singleton.h"

namespace Util {
	enum ThreadState {
		threadStateNone = 0,

		threadStateIdle,
		threadStateRunning,

		threadStateCount,

	};

	class ThreadPool : public Singleton<ThreadPool> {
	private:
		using Worker = std::tuple<ThreadState, std::thread>;

	public:
		void EnqueueJob(std::function<void(void)>);
		void Initialize();

	private:
		DECLARE_SINGLETON(ThreadPool)

		

	};

}
#endif // !__BASELIB_THREAD_POOL_H__
