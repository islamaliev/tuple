#include <iostream>
#include <tuple>
#include "MyTuple.h"

//using namespace std;
using namespace ns; 

int main()
{
	tuple<int, float> t(1, 0.5);
	auto i = get<0>(t);
	auto d = get<1>(t);
	std::cout << i << std::endl;
	std::cout << d << std::endl;
	std::system("pause");
	return 0;
}
