namespace ns
{ 

template<class... Args>
struct ValueHolder;

template<class T>
struct ValueHolder<T> 
{ 
	ValueHolder(T v)
		: val(v)
	{}

	T val;
};

template<class T, class... Args>
struct ValueHolder<T, Args...> 
{
	ValueHolder(T val1, Args... args)
		: val(val1)
		, other(args...)
	{}

	T val;
	ValueHolder<Args...> other;
};

template<class... Args>
class tuple
{
public:
	tuple(Args... args)
		: m_values(args...)
	{}

	ValueHolder<Args...> m_values;
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

template<int current, int sought, class... Types>
struct ValuePicker {};

template<int sought, class T, class... Types>
struct ValuePicker<sought, sought, T, Types...>
{
	static auto& get(ValueHolder<T, Types...>& vh) { return vh.val; }
};

template<int current, int sought, class T, class... Types>
struct ValuePicker<current, sought, T, Types...>
{
	static auto& get(ValueHolder<T, Types...>& vh) { return ValuePicker<current + 1, sought, Types...>::get(vh.other); }
};

template<int index, class ... Types>
auto get(tuple<Types...>& t) -> typename TypesPicker<0, index, Types...>::type
{
	auto& val = ValuePicker<0, index, Types...>::get(t.m_values);
	return val;
}

}
