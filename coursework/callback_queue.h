#pragma once

#include <windows.h>
#include <mutex>
#include <condition_variable>
#include <deque>
#include <functional>

class CallbackQueue
{
public:
	void Push(std::function<void()>);
	std::function<void()> Pop();

	static CallbackQueue *GetInstance();

private:
	CallbackQueue();

	std::mutex mutex;
	std::condition_variable condition;
	std::deque<std::function<void()>> queue;

	static CallbackQueue *instance;
};
