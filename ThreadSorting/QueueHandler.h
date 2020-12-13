#pragma once
#include "TaskQueue.h"
#include <thread>
class QueueHandler
{
public:
	TaskQueue _tasksQueue;
	explicit QueueHandler(TaskQueue _tasksQueue);
	void StartTask(int _maxThreadCount);
};

