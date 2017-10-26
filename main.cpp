#include <iostream>
#include <tuple>

namespace ns
{ 

template<class... Args>
struct ValueHolder;

template<>
struct ValueHolder<> { };

template<class T, class... Args>
struct ValueHolder<T, Args...> : ValueHolder<Args...> 
{
	ValueHolder(T val1, Args... args)
		: ValueHolder<Args...>(args...)
		, val(val1)
	{}

	typedef T type;
	T val;
};

template<class... Args>
class tuple
{
public:
	tuple(Args... args)
		: m_values(args...)
	{}

	typename ValueHolder<Args...>::type get() const 
	{ 
		return m_values.val; 
	}

	ValueHolder<Args...> m_values;

};

/*template<class T>
class tuple
{
public:
	tuple(T _1)
		: m_1(_1)
	{}

	T get() const { return m_1; }

	T m_1;
};*/

template<bool cond, class T1, class T2>
struct Condition
{
	typedef T2 type;
};

template<class T1, class T2>
struct Condition<true, T1, T2>
{
	typedef T1 type;
};

template<int current, int sought, class... Types>
struct TypesPicker {};

template<int sought, class T, class... Types>
struct TypesPicker<sought, sought, T, Types...>
{
	typedef T type;
};

template<int current, int sought, class T, class... Types>
struct TypesPicker<current, sought, T, Types...> 
{
	typedef typename TypesPicker<current + 1, sought, Types...>::type type;
};

template<int index, class ... Types>
auto get(tuple<Types...>& t) -> typename TypesPicker<0, index, Types...>::type
{
	return 0;
}

}

//using namespace std;
using namespace ns;

int main()
{
	tuple<int, double> t(1, 0.5);
	auto i = get<0>(t);
	auto d = get<1>(t);
	std::cout << i << std::endl;
	std::cout << d << std::endl;
	std::system("pause");
	return 0;
}