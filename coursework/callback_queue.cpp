#include "callback_queue.h"

CallbackQueue *CallbackQueue::instance = nullptr;

CallbackQueue::CallbackQueue() {}

void CallbackQueue::Push(std::function<void()> value)
{
	{
		std::unique_lock<std::mutex> lock(this->mutex);
		this->queue.push_front(value);
	}
	this->condition.notify_one();
}

std::function<void()> CallbackQueue::Pop()
{
	std::unique_lock<std::mutex> lock(this->mutex);
	this->condition.wait(lock, [this] { return !this->queue.empty(); });
	std::function<void()> result = this->queue.back();
	this->queue.pop_back();
	return result;
}

CallbackQueue *CallbackQueue::GetInstance()
{
	if (CallbackQueue::instance == nullptr)
	{
		CallbackQueue::instance = new CallbackQueue();

		auto fn = [](void *) {
			while (true) CallbackQueue::instance->Pop()();

			return DWORD(0);
		};

		CreateThread(NULL, NULL, fn, NULL, NULL, NULL);
	}

	return CallbackQueue::instance;
}
