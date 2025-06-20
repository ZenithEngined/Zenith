#pragma once

#include <string>
#include <thread>

namespace Zenith {

	class Thread
	{
	public:
		Thread(const std::string& name);

		template<typename Fn, typename... Args>
		void Dispatch(Fn&& func, Args&&... args)
		{
			m_Thread = std::thread(func, std::forward<Args>(args)...);
			SetName(m_Name);
		}

		void SetName(const std::string& name);

		void Join();

		std::thread::id GetID() const;
	private:
		std::string m_Name;
		std::thread m_Thread;
	};

	class ThreadSignal
	{
	public:
		ThreadSignal(const std::string& name, bool manualReset = false);
		~ThreadSignal();

		void Wait();
		void Signal();
		void Reset();
	private:
		void* m_SignalHandle = nullptr;
	};

}