#include <iostream>
#include <tuple>
#include "MyTuple.h"
#include <string>
#include <stdexcept>

//using namespace std;
using namespace ns; 

int main()
{
	auto t = make_tuple(1, 0.5);
	auto i = get<0>(t);
	std::cout << i << std::endl;
	auto d = get<1>(t);
	std::cout << d << std::endl;
	std::cout << tuple_size<decltype(t)>::value << std::endl;
	std::system("pause");
	return 0;
}
