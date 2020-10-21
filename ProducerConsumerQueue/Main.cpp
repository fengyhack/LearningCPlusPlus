#include "DataManager.h"

int main()
{
	const int N = 1024 * 1024;
	const int M = 10;

	auto mgr = make_shared<DataManager>();
	auto producer = thread([&]() 
		{
			for (int i = 1; i <= M; ++i)
			{
				DataObject object{ N, new char[N] };
				memset(object.data, 0x0D, N);
				sprintf_s(object.data, N, "%05d", i);
				mgr->OnDataProduced(object);
				this_thread::sleep_for(chrono::milliseconds(200));
			}
			mgr->FinishProduction();
		});
	producer.detach();

	mgr->WaitForConsumption();

	this_thread::sleep_for(chrono::milliseconds(500));
	cout << "Done";
	cin.get();

	return 0;
}