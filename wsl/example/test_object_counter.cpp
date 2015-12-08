
#include <map>
#include <iostream>
#include "../include/object_counter.h"

using namespace std;
using namespace wsl;

// TestReporter �����д����󽫱���¼�ڰ�
struct TestReporter : public object_reporter<TestReporter, size_t>
{
	string _s;

	TestReporter(const char* s = "")
		: _s(s)
	{}
};

ostream& operator<<(ostream& ost, const TestReporter& o)
{
	ost << o._s;
	return ost;
}

struct print_reporter
{
	void operator()(const TestReporter& r)
	{
		cout << r << endl;
	}
};

// TestCounter �Ĵ�����������¼�����̰߳�ȫ
struct TestCounter : public object_counter<TestCounter, size_t, Mutex>
{
	string _s;

	TestCounter(const char* s = "")
		: _s(s)
	{}
};

int main(int argc, char ** argv)
{
	// object_reporter
	std::map<int, TestReporter> m;
	m[1] = "aa";
	m[2] = "bb";
	m[1] = "cc";
	// ���TestReporter�Ĵ�������
	cout << "TestReporter count: " << TestReporter::get_object_count() << endl;
	// �����ǰ���д���TestReporter����
	TestReporter::for_each_object(print_reporter());

	// object_counter
	TestCounter * p = new TestCounter();
	TestCounter aa;
	aa = *p;
	// ���TestCounter�Ĵ�������
	cout << "TestCounter count: " << TestCounter::get_object_count() << endl;

	return 0;
}

