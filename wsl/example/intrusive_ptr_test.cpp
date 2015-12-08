
#include <iostream>
#include <set>
#include <map>
#include <vector>
#include "../include/intrusive_ptr.h"
#include "../include/object_counter.h"
#include "../include/ref.h"
#include <assert.h>

using namespace std;
using namespace wsl;

struct TestCounter : public object_counter<TestCounter, size_t, Mutex>
{
	string _s;
	mutable int _ref_count;

	TestCounter(const char* s = "")
		: _s(s)
		, _ref_count(0)
	{}

	friend void intrusive_ptr_add_ref(const TestCounter * p)
	{
		++p->_ref_count;
	}

	 friend void intrusive_ptr_release(const TestCounter * p)
	{
		if(--p->_ref_count <= 0)
			delete p;
	}
	 int get_ref_count()
	 {
		 return _ref_count;
	 }
};

int main(int argc, char ** argv)
{
	{
		intrusive_ptr<TestCounter>	p(new TestCounter("abcde"));
		intrusive_ptr<const TestCounter>	p2 ( p );
		//intrusive_ptr<TestCounter>	p2 = shared_const_cast<TestCounter>(p);
		//cout << p.use_count() << endl;
		cout << "TestCounter count: " << TestCounter::get_object_count()<<"  "<<p->get_ref_count() << endl;
		cout << p << endl;
	}
	cout << "TestCounter count: " << TestCounter::get_object_count() << endl;
	return 0;
}

