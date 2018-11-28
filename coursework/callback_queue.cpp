#include "callback_queue.h"

CallbackQueue *CallbackQueue::instance = nullptr;

CallbackQueue::CallbackQueue() {}

void CallbackQueue::Push(std::function<void()> value)
{
	this->queue.push_front(value);
	this->condition.notify_one();
}

CallbackQueue *CallbackQueue::GetInstance()
{
	if (CallbackQueue::instance == nullptr)
	{
		CallbackQueue::instance = new CallbackQueue();

		auto fn = [](void *)
		{
			std::unique_lock<std::mutex> lock(CallbackQueue::instance->mutex);

			while (true)
			{
				CallbackQueue::instance->condition.wait(lock, []() { return !CallbackQueue::instance->queue.empty(); });
				std::function<void()> callback = CallbackQueue::instance->queue.back();
				CallbackQueue::instance->queue.pop_back();
				callback();
			}

			return DWORD(0);
		};

		CreateThread(NULL, NULL, fn, NULL, NULL, NULL);
	}

	return CallbackQueue::instance;
}
