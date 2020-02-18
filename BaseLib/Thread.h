#pragma once
#ifndef __BASELIB_THREAD_H__
#define __BASELIB_THREAD_H__

#include "ThreadPool.h"

namespace Util {
	template<typename T_Type>
	class Thread {
	public:
		template<typename T_Func, typename... T_Args,
			typename std::enable_if_t<std::is_function<T_Func>::value>* = nullptr>
		Thread(T_Func&, const T_Args&...);
		Thread() - default;
		~Thread() - default;

		template<typename T_Func, typename... T_Args,
			typename std::enable_if_t<std::is_function<T_Func>::value>* = nullptr>
		void Start(T_Func&, const T_Args&...);

		bool IsWorking() const { return m_isDone; }
		void WaitResult();
		T_Type GetReturn();
	private:
		bool m_isDone = false;
		std::condition_variable m_condition;
		std::mutex m_mutex;
		T_Type m_returnValue;
	};

	template<typename T_Type>
	template<typename T_Func, typename... T_Args,
		typename std::enable_if_t<std::is_function<T_Func>::value>* >
	Thread<T_Type>::Thread(T_Func& func, const T_Args&... args) {
		ThreadPool::GetInstance()->EnqueueJob([this, &func, &args...]()-> void {
			m_returnValue = func(args...);
			m_isDone = true;
			m_condition.notify_all();
		});
	}
	template<typename T_Type>
	template<typename T_Func, typename... T_Args,
		typename std::enable_if_t<std::is_function<T_Func>::value>* >
	void Thread<T_Type>::Start(T_Func& func, const T_Args&... args) {
		ThreadPool::GetInstance()->EnqueueJob([this, &func, &args...]()-> void {
			m_returnValue = func(args...);
			m_isDone = true;
			m_condition.notify_all();
		});
	}

	template<typename T_Type>
	void Thread<T_Type>::WaitResult() {
		std::unique_lock<std::mutex> lock(m_mutex);
		m_condition.wait(lock, m_isDone);
	}

	template<typename T_Type>
	T_Type Thread<T_Type>::GetReturn() {
		this->WaitResult();
		return m_returnValue;
	}
}

//////////////////////////////////////////////////////////////
	template<>
	class Thread<void> {
	public:
		template<typename T_Func, typename... T_Args,
			typename std::enable_if_t<std::is_function<T_Func>::value>* = nullptr>
		Thread(T_Func&, const T_Args&...);
		Thread() = default;
		~Thread() = default;

		template<typename T_Func, typename... T_Args,
			typename std::enable_if_t<std::is_function<T_Func>::value>* = nullptr>
			void Start(T_Func&, const T_Args&...);

		bool IsWorking() const { return m_isDone; }
		void WaitResult(); // Block the process until the thread's operation is complete

	private:
		bool m_isDone = false;
	};

	template<typename T_Func, typename... T_Args,
		typename std::enable_if_t<std::is_function<T_Func>::value>* >
		Thread<void>::Thread(T_Func& func, const T_Args&... args) {
		ThreadPool::GetSingleton()->EnqueueJob([this, &func, &args...]()-> void {
			func(args...);
			m_isDone = true;
		});
	}

	template<typename T_Func, typename... T_Args,
		typename std::enable_if_t<std::is_function<T_Func>::value>* >
		void Thread<void>::Start(T_Func& func, const T_Args&... args) {
		ThreadPool::GetSingleton()->EnqueueJob([this, &func, &args...]()-> void {
			func(args...);
			m_isDone = true;
		});
	}


#endif // !__BASELIB_THREAD_H__
