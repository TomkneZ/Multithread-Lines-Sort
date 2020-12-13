#include "QueueHandler.h"

QueueHandler::QueueHandler(TaskQueue queue)
{
	_tasksQueue = queue;
}

void DistributeToThreads(TaskQueue queue, int count)
{
	std::vector<std::thread> _threads;
	while (count)
	{
		TTask _task = queue.TakeTask();
		if (_task != NULL)
		{
			std::thread _thread(_task);
			_threads.push_back(std::move(_thread));
			count--;
		}
	}

	for (auto& _task : _threads)
	{
		_task.join();
	}
}

void QueueHandler::StartTask(int _maxThreadCount)
{
	std::thread _thread(DistributeToThreads, _tasksQueue, _maxThreadCount);
	_thread.join();
}