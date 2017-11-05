namespace ns
{ 

template<class... Types>
struct ValueHolder;

template<class T>
struct ValueHolder<T> 
{ 
	ValueHolder(T v)
		: val(v)
	{}

	T val;
};

template<class T>
struct ValueHolder<T&>
{
	ValueHolder(T& v)
		: val(v)
	{}

	T& val;
};

template<class T, class... Types>
struct ValueHolder<T, Types...> 
{
	ValueHolder(T val1, Types... args)
		: val(val1)
		, other(args...)
	{}

	T val;
	ValueHolder<Types...> other;
};

template<class T, class... Types>
struct ValueHolder<T&, Types&...>
{
	ValueHolder(T& val1, Types&... args)
		: val(val1)
		, other(args...)
	{}

	T& val;
	ValueHolder<Types&...> other;
};

template<class T>
void assignValueHolder(ValueHolder<T&>& left, const ValueHolder<T>& right)
{
	left.val = right.val;
}

template<class T, class... Types>
void assignValueHolder(ValueHolder<T&, Types&...>& left, const ValueHolder<T, Types...>& right)
{
	left.val = right.val;
	assignValueHolder(left.other, right.other);
}

template<class... Types>
struct tuple
{
	template<class... Types2>
	tuple(Types2&&... args)
		: values(std::forward<Types2>(args)...)
	{}

	ValueHolder<Types...> values;
};

template<class... Types>
struct tuple<Types&...>
{
	tuple(Types&... args)
		: values(args...)
	{ }

	template<class... Types2>
	tuple<Types&...>& operator=(tuple<Types2...>&& other)
	{
		assignValueHolder(values, other.values);
		return *this;
	}

	ValueHolder<Types&...> values;
};

template<int current, int sought, class... Types>
struct TupleIterator {};

template<int sought, class T, class... Types>
struct TupleIterator<sought, sought, T, Types...>
{
	typedef T type;
	static type& get(ValueHolder<T, Types...>& vh) { return vh.val; }
};

template<int current, int sought, class T, class... Types>
struct TupleIterator<current, sought, T, Types...> 
{
	typedef typename TupleIterator<current + 1, sought, Types...>::type type;
	static type& get(ValueHolder<T, Types...>& vh) { return TupleIterator<current + 1, sought, Types...>::get(vh.other); }
};

template<int index, class ... Types>
constexpr auto get(tuple<Types...>& t) noexcept -> typename TupleIterator<0, index, Types...>::type& 
{
	return TupleIterator<0, index, Types...>::get(t.values);
}

template<int index, class ... Types>
constexpr auto get(const tuple<Types...>& t) noexcept -> const typename TupleIterator<0, index, Types...>::type& 
{
	return TupleIterator<0, index, Types...>::get(t.values);
}

template<class... Types>
struct TypesCounter 
{
	enum { value = 0 };
};

template<class T, class... Types>
struct TypesCounter<T, Types...>
{
	enum { value = TypesCounter<Types...>::value + 1 };
};

template<class... Types>
struct tuple_size {};

template<class... Types>
struct tuple_size<tuple<Types...>>
{
	enum { value = TypesCounter<Types...>::value};
};

template<class... Types>
constexpr tuple<Types...> make_tuple(Types&&... args)
{
	return tuple<Types...>(std::forward<Types>(args)...);
}

template<class... Types>
constexpr tuple<Types&...> tie(Types&... args) noexcept
{
	return make_tuple(args...);
}

}
