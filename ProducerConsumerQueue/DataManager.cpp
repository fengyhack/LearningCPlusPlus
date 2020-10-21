#include "DataManager.h"

DataManager::DataManager() :
	index(0), residue(0), 
	finished(false), consumerThreadActive(false)
{
	consumerThreadActive = true;
	consumerThread = thread([this]() 
		{
			_ConsumerThreadFunc();
		});
	consumerThread.detach();
}

DataManager::~DataManager()
{
	WaitForConsumption();
}

void DataManager::OnDataProduced(DataObject object)
{
	{
		lock_guard<mutex> lck(mtx);
		objectQueue.push(object);
		++residue;
	}
	cout << "Append, Residue: " << residue << endl;
}

void DataManager::FinishProduction()
{
	finished = true;
}

void DataManager::WaitForConsumption()
{
	auto pend = [&]() 
	{
		lock_guard<mutex> lck(mtx);
		return !finished || residue > 0;
	};
	while (pend())
	{
		this_thread::sleep_for(chrono::milliseconds(100));
	}
	consumerThreadActive = false;
}

void DataManager::_ConsumerThreadFunc()
{
	while (consumerThreadActive)
	{
		if (residue == 0)
		{
			this_thread::sleep_for(chrono::milliseconds(100));
		}
		else
		{
			{
				mtx.lock();
				auto& object = objectQueue.front();
				mtx.unlock();

				FILE* fp = nullptr;
				char sz[16] = { 0 };
				sprintf_s(sz, 16, "%05d.txt", ++index);
				fopen_s(&fp, sz, "w");
				if (fp)
				{
					fwrite(object.data, object.size, 1, fp);
					fclose(fp);
					delete[] object.data;
				}

				mtx.lock();
				--residue;
				objectQueue.pop();
				mtx.unlock();

				this_thread::sleep_for(chrono::milliseconds(500));
			}
			cout << "Save: " << index << ", Residue: " << residue << endl;
		}
	}
}