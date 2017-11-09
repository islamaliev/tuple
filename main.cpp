#include <iostream>
#include <tuple>
#include "MyTuple.h"
#include <string>
#include <stdexcept>
#include <cassert>

//using namespace std;
using namespace ns; 

int main()
{
	int i = 1;
	auto t = make_tuple(i, 0.5);
	i = get<0>(t);
	auto d = get<1>(t);

	assert(i == 1);
	assert(d == 0.5);
	assert(tuple_size<tuple<int>>::value == 1);
	assert(tuple_size<decltype(t)>::value == 2);

	int li = 5;
	double ld = 3.2;

	tie(li, ld) = make_tuple(4, 2.2);
	assert(li == 4);
	assert(ld == 2.2);

	tie(ignore, ld) = make_tuple(5, 6);
	assert(li == 4);
	assert(ld == 6);

	tie(li, ignore) = make_tuple(7, 8);
	assert(li == 7);
	assert(ld == 6);

	tuple<int, double> tupInitList = { 1, 2.4 };
	assert(get<1>(tupInitList) == 2.4);

	auto te = std::is_same_v<tuple_element<0, tuple<int, float>>::type, int>;
	assert(te);
	te = std::is_same_v<tuple_element<1, tuple<int, float>>::type, float>;
	assert(te);

	return 0;
}
