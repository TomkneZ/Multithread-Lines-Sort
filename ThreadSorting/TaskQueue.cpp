#include "TaskQueue.h"
#include <mutex>

std::mutex _mutex;

TaskQueue::TaskQueue() { }

int TaskQueue::AddTask(const TTask& task)
{
	_mutex.lock();
	_tasksQueue->push(task);
	_mutex.unlock();
	return 1;
}

TTask TaskQueue::TakeTask()
{
	TTask _task;
	_mutex.lock();
	if (_tasksQueue->empty())
	{
		_task = NULL;
	}
	else
	{
		_task = _tasksQueue->front();
		_tasksQueue->pop();
	}
	_mutex.unlock();
	return _task;
}
