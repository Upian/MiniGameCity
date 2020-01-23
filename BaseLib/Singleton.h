#pragma once
#ifndef BASELIB_SINGLETON_H
#define BASELIB_SINGLETON_H

/*
*	NOTE
*	Singleton<class name> 상속 후
*	DECLARE_SINGLETON(class name) 을 private로 선언 및 생성자 정의
*	DECLARE_SINGLETON는 생성자, 소멸자

*	 CreateInstance()		singleton 클래스 생성 또는 반환
*	GetInstance()			singleton 클래스 반환
*	DistorySingleton()		singleton 클래스 소멸
*/
namespace Util {
	template<typename T_Class>
	class Singleton {
	public:
		typedef T_Class SingletonClass;

		static SingletonClass* CreateInstance();
		template <typename ...T_Args> static SingletonClass* CreateInstance(const T_Args& ...args);
		static void DestroyInstance();
		static SingletonClass* GetInstance();

	protected:
		Singleton() {}
		virtual ~Singleton() { }

	private:
		static SingletonClass* m_instance;
	};

	/*
	*	Initialize
	*/
	template <typename T_Class>
	typename Singleton<T_Class>::SingletonClass* Singleton<T_Class>::m_instance = nullptr;

	/*
	*	Create Singleton
	*/
	template <typename T_Class>
	typename Singleton<T_Class>::SingletonClass* Singleton<T_Class>::CreateInstance() {
		if (nullptr == m_instance)
			m_instance = new SingletonClass();

		return m_instance;
	}
	template <typename T_Class>
	template <typename ...T_Args>
	typename Singleton<T_Class>::SingletonClass* Singleton<T_Class>::CreateInstance(const T_Args& ...args) {
		if (nullptr == m_instance)
			m_instance = new SingletonClass(args...);

		return m_instance;
	}
	/*
	*	Get Singleton
	*/
	template <typename T_Class>
	typename Singleton<T_Class>::SingletonClass* Singleton<T_Class>::GetInstance() {
		return m_instance;
	}

	/*
	*	Destory Singleton
	*/
	template<typename T_Class>
	void Singleton<T_Class>::DestroyInstance() {
		delete m_instance;
		m_instance = nullptr;
	}
	//#Singleton End
}//namespace Util

/*
*   DECLARE_SINGLETON
*   Make class to singlton
*   must define a constructor and destroyer
*/
#ifndef DECLARE_SINGLETON
#define DECLARE_SINGLETON( ClassName ) \
        friend class Util::Singleton< ClassName >; ClassName(); ~ClassName();
#endif // !DECLARE_SINGLETON

#endif // !BASELIB_SINGLETON_H

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
