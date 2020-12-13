#include <stdio.h>
#include "TaskQueue.h"
#include "QueueHandler.h"
#include <string>
#include <iostream>
#include <fstream>
#include <functional>
#include <vector>

using namespace std;
const string FILE_PATH = "TextFile.txt";
typedef vector<string> Vector;

bool ReadFile(const string& fileName, Vector*& fileContent)
{
	ifstream fin(fileName);
	string line;

	if (!fin.good()) {
		return false;
	}

	while (getline(fin, line))
	{
		string tmpLine;
		tmpLine = line + "\n";
		if (tmpLine.size() != 0)
		{
			fileContent->push_back(tmpLine);
		}			
	}
	fin.close();
	return true;
}

bool WriteInFile(Vector sortedFileContent)
{
	ofstream fout("SortedFile.txt");
	if (!fout.good())
	{
		return false;
	}
	for (auto str : sortedFileContent) {
		fout << str.c_str();
	}
	fout.close();
	return true;
}

void DistributeToThreads(int threadCount, vector<Vector*>*& splitedFileContent, Vector*& fileContent, TaskQueue taskQueue)
{
	auto onePartCount = (int)floor((((double)fileContent->size()) / threadCount) + .5);
	for (size_t i = 0; i < fileContent->size(); i += onePartCount) {
		auto* newVector = new vector<string>;
		splitedFileContent->push_back(newVector);
		for (size_t j = i; j < i + onePartCount; j++) {
			if (j < fileContent->size()) {
				string str = (*fileContent)[j];
				newVector->push_back(str);
			}
		}
		taskQueue.AddTask([newVector]() { sort(newVector->begin(), newVector->end()); });
	}
}

Vector Concat(const Vector& firstPart, const Vector& secondPart)
{
	size_t firstPartSize = firstPart.size();
	size_t secondPartSize = secondPart.size();
	Vector result;
	result.reserve(firstPartSize + secondPartSize);
	int i = 0, j = 0;

	while (i < firstPartSize && j < secondPartSize) {

		if (firstPart[i] <= secondPart[j])
		{
			result.push_back(firstPart[i]);
			i++;
		}
		else
		{
			result.push_back(secondPart[j]);
			j++;
		}
	}

	while (i < firstPartSize)
	{
		result.push_back(firstPart[i]);
		i++;
	}

	while (j < secondPartSize)
	{
		result.push_back(secondPart[j]);
		j++;
	}
	return result;
}

Vector ConcatAll(vector<vector<string>*>*& splitedFileContent)
{
	Vector tmpVector;
	if (splitedFileContent->size() > 0) {
		tmpVector = *(*splitedFileContent)[0];
	}
	for (size_t i = 1; i < splitedFileContent->size(); i++) {
		tmpVector = Concat(tmpVector, *(*splitedFileContent)[i]);
	}
	return tmpVector;
}

int main()
{
	auto* fileContent = new Vector();	
	bool readFile = ReadFile(FILE_PATH, fileContent);

	if (!readFile) {
		std::cout << "File not found!\n";
		return -1;
	}

	TaskQueue taskQueue;
	QueueHandler queueHandler(taskQueue);
	int threadCount = (int)fileContent->size();
	auto* splitedFileContent = new vector<Vector*>();
	DistributeToThreads(threadCount, splitedFileContent, fileContent, taskQueue);
	queueHandler.StartTask(threadCount);
	Vector sortedFileContent = ConcatAll(splitedFileContent);
	WriteInFile(sortedFileContent);	
	std::cout << "File was sorted successfully. Check result in file.\n";
	system("pause");
}