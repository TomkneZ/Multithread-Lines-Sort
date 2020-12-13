#pragma once
#include <queue>
#include <functional>

typedef std::function<void()> TTask;

class TaskQueue
{
public:
	TaskQueue();
	std::queue<TTask>* _tasksQueue = new std::queue<TTask>;
	int AddTask(const TTask& task);
	TTask TakeTask();
};

