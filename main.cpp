#include <iostream>
#include <tuple>
#include "MyTuple.h"
#include <string>
#include <stdexcept>
#include <cassert>

//using namespace std;
using namespace ns; 

constexpr void test_get()
{
	int i = 1;
	auto t = make_tuple(i, 0.5);
	i = get<0>(t);
	auto d = get<1>(t);
	assert(i == 1);
	assert(d == 0.5);
}

constexpr void test_tuple_size()
{
	auto t = make_tuple(8, 0.5);
	assert(tuple_size<tuple<int>>::value == 1);
	assert(tuple_size<decltype(t)>::value == 2);
}

constexpr void test_get_assign()
{
	auto t = make_tuple(8, 0.5);
	get<int>(t) = 5;
	get<double>(t) = 4.4;
	assert(get<0>(t) == 5);
	assert(get<1>(t) == 4.4);
}

constexpr void test_tie()
{
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

	auto copyTuple = make_tuple(2, 3.3);
	tie(li, ld) = copyTuple;
	assert(li == 2);
	assert(ld == 3.3);

	auto moveTuple = make_tuple(5, 4.4);
	tie(li, ld) = std::move(moveTuple);
	assert(li == 5);
	assert(ld == 4.4);
}

constexpr void test_copy_assign()
{
	auto copyTuple = make_tuple(2, 3.3);
	auto t2 = make_tuple(1, 0.3);
	copyTuple = t2;
	assert(get<0>(copyTuple) == 1);
	assert(get<1>(copyTuple) == 0.3);
}

constexpr void test_initialization_list()
{
	tuple<int, double> tupInitList = { 1, 2.4 };
	assert(get<1>(tupInitList) == 2.4);
}

constexpr void test_tuple_element()
{
	auto te = std::is_same_v<tuple_element<0, tuple<int, float>>::type, int>;
	assert(te);
	te = std::is_same_v<tuple_element<1, tuple<int, float>>::type, float>;
	assert(te);
}

int main()
{
	test_get();
	test_tuple_size();
	test_get_assign();
	test_tie();
	test_copy_assign();
	test_initialization_list();
	test_tuple_element();
	test_tuple_cat();

	return 0;
}
