#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>

using namespace std;
using namespace std::chrono;

void func(int k, int n)
{
	int i = 0;
	int c = (k * k + 1) % 5 + 1;
	auto e = seconds(c);
	while (i < n)
	{
		this_thread::sleep_for(e);
		i += c;
		cout << "index: " << k << " tick: " << i << endl;
	}
	cout << "index: " << k << " wait:" << n << " complete" << endl;
}

int main(void)
{
	cout << "main start..." << endl;
	for (int i = 1; i < 3; ++i)
	{
		auto t = thread(func, i, ((i + 1) * (i + 2)) + 30);
		t.detach();
	}
	cout << "main exit..." << endl;
	(void)getchar();
	return 0;
}