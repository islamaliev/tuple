namespace ns
{ 

template<class... Types>
struct ValueHolder;

template<class T>
struct ValueHolder<T> 
{ 
	constexpr ValueHolder(T v) : val(v) {} 

	constexpr ValueHolder() = default;

	T val;
};

template<class T>
struct ValueHolder<T&>
{
	constexpr ValueHolder(T& v) : val(v) {}

	T& val;
};

template<class T, class... Types>
struct ValueHolder<T, Types...> 
{
	constexpr ValueHolder(T val1, Types... args)
		: val(val1)
		, other(args...)
	{}

	constexpr ValueHolder() = default;

	T val;
	ValueHolder<Types...> other;
};

template<class T, class... Types>
struct ValueHolder<T&, Types&...>
{
	constexpr ValueHolder(T& val1, Types&... args)
		: val(val1)
		, other(args...)
	{}

	T& val;
	ValueHolder<Types&...> other;
};

struct _Ignore 
{
	template<class T>
	constexpr void operator=(const T& other) const {}
};

constexpr _Ignore ignore{};

template<class T1, class T2>
constexpr void assignValueHolder(ValueHolder<T1&>& left, const ValueHolder<T2>& right)
{
	left.val = right.val;
}

template<class T1, class T2, class... Types1, class... Types2>
constexpr void assignValueHolder(ValueHolder<T1&, Types1&...>& left, const ValueHolder<T2, Types2...>& right)
{
	left.val = right.val;
	assignValueHolder(left.other, right.other);
}

template<class... Types>
struct tuple
{
	template<class... Types2>
	constexpr tuple(Types2&&... args)
		: values(std::forward<Types2>(args)...)
	{}

	constexpr tuple() = default;

	ValueHolder<Types...> values;
};

template<class... Types>
struct tuple<Types&...>
{
	constexpr tuple(Types&... args)
		: values(args...)
	{ }

	template<class... Types2>
	constexpr tuple<Types&...>& operator=(tuple<Types2...>&& other)
	{
		assignValueHolder(values, std::forward<tuple<Types2...>>(other).values);
		return *this;
	}

	constexpr tuple& operator=(const tuple<Types...>& other)
	{
		assignValueHolder(values, other.values);
		return *this;
	}

	ValueHolder<Types&...> values;
};

template<int current, int sought, class... Types>
struct TupleIndexIterator {};

template<int sought, class T, class... Types>
struct TupleIndexIterator<sought, sought, T, Types...>
{
	typedef T type;
	static constexpr type& get(ValueHolder<T, Types...>& vh) { return vh.val; }
};

template<int current, int sought, class T, class... Types>
struct TupleIndexIterator<current, sought, T, Types...>
{
	typedef typename TupleIndexIterator<current + 1, sought, Types...>::type type;
	static constexpr type& get(ValueHolder<T, Types...>& vh) { return TupleIndexIterator<current + 1, sought, Types...>::get(vh.other); }
};

template<class... Types>
struct TupleTypeIterator {};

template<class T, class... Types>
struct TupleTypeIterator<T, T, Types...>
{
	typedef T type;
	static constexpr type& get(ValueHolder<T, Types...>& vh) { return vh.val; }
};

template<class T, class Head, class... Tail>
struct TupleTypeIterator<T, Head, Tail...>
{
	typedef typename TupleTypeIterator<T, Tail...>::type type;
	static constexpr type& get(ValueHolder<Head, Tail...>& vh) { return TupleTypeIterator<T, Tail...>::get(vh.other); }
};

template<int index, class ... Types>
constexpr auto get(tuple<Types...>& t) noexcept -> typename TupleIndexIterator<0, index, Types...>::type& 
{
	return TupleIndexIterator<0, index, Types...>::get(t.values);
}

template<class T, class... Types>
constexpr auto get(tuple<Types...>& t) noexcept -> typename TupleTypeIterator<T, Types...>::type&
{
	return TupleTypeIterator<T, Types...>::get(t.values);
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

template<int index, class T>
struct tuple_element {};

template<int index, class... Types>
struct tuple_element<index, tuple<Types...>>
{
	typedef typename TupleIndexIterator<0, index, Types...>::type type;
};

}
