#pragma once

#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
using namespace std;

struct DataObject
{
	int   size;
	char* data;
};

class DataManager
{
public:
	DataManager();
	~DataManager();

	void OnDataProduced(DataObject object);
	void FinishProduction();
	void WaitForConsumption();

private:
	void _ConsumerThreadFunc();

private:
	queue<DataObject> objectQueue;

	bool finished;
	bool consumerThreadActive;
	thread consumerThread;
	int index;
	int residue;
	mutex mtx;
};

