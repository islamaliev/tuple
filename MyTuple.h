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
struct tuple
{
	tuple(Args... args)
		: values(args...)
	{}

	ValueHolder<Args...> values;
};

template<int current, int sought, class... Types>
struct TupleIterator {};

template<int sought, class T, class... Types>
struct TupleIterator<sought, sought, T, Types...>
{
	typedef T type;
	static auto& get(ValueHolder<T, Types...>& vh) { return vh.val; }
};

template<int current, int sought, class T, class... Types>
struct TupleIterator<current, sought, T, Types...> 
{
	typedef typename TupleIterator<current + 1, sought, Types...>::type type;
	static auto& get(ValueHolder<T, Types...>& vh) { return TupleIterator<current + 1, sought, Types...>::get(vh.other); }
};

template<int index, class ... Types>
auto get(tuple<Types...>& t) -> typename TupleIterator<0, index, Types...>::type&
{
	auto& val = TupleIterator<0, index, Types...>::get(t.values);
	return val;
}

template<class... Types>
tuple<Types...> make_tuple(Types... args)
{
	return tuple<Types...>(args...);
}

}
