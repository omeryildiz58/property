#pragma once

template<auto getter, auto setter = nullptr>
class Property {
	template<class T>
	class GetterInfo;

	template<class C, class VT>
	class GetterInfo<VT(C::*)()> //GetterInfo pointer
	{
	public:
		typedef C ClassType;
		typedef VT ReturnType;
	};

	template<class C, class VT>
	class GetterInfo<VT(C::*)() const> : public GetterInfo<VT(C::*)()> {}; //const GetterInfo pointer

	template<class C, class VT>
	class GetterInfo<VT(C::*)() volatile> : public GetterInfo<VT(C::*)()> {}; //volatile GetterInfo pointer

public:
	using ClassType = GetterInfo<decltype(getter)>::ClassType;
	using GetterType = GetterInfo<decltype(getter)>::ReturnType;
	using ValueType = std::remove_reference<GetterType>::type;
	Property(ClassType& instance) : instance_(instance) {}
	template <auto S = setter, typename std::enable_if_t < S != nullptr, int > = 0>
	Property(ClassType& instance, const ValueType& value) : instance_(instance) { set(value); }

	inline ValueType& get() {
		return (instance_.*getter)();
	}

	template<auto S = setter>
	inline std::enable_if_t<S != nullptr, void> set(const ValueType& value) const {
		(instance_.*setter)(value);
	}
	template<auto S = setter>
	inline std::enable_if_t<S != nullptr, Property&> operator=(const ValueType& value) const {
		set(value);
		return *this;
	}

	inline operator ValueType& () {
		return get();
	}

	// Bir sýnýfýn operator+'e sahip olup olmadýðýný kontrol eden yardýmcý þablon
	template<typename, typename, typename = void>
	class has_plus : public std::false_type {};

	template<typename T, typename U>
	class has_plus<T, U, std::void_t<decltype(std::declval<T&>() + std::declval<U>())>> : public std::true_type {};

	// Bir sýnýfýn operator+='e sahip olup olmadýðýný kontrol eden yardýmcý þablon
	template<typename, typename, typename = void>
	class has_plus_equal : public std::false_type {};

	template<typename T, typename U>
	class has_plus_equal<T, U, std::void_t<decltype(std::declval<T&>() += std::declval<U>())>> : public std::true_type {};

	// bir	sýnýfýn operator-'e sahip olup olmadýðýný kontrol eden yardýmcý þablon
	template<typename, typename, typename = void>
	class has_minus : public std::false_type {};
	template<typename T, typename U>
	class has_minus<T, U, std::void_t<decltype(std::declval<T&>() - std::declval<U>())>> : public std::true_type {};
	// Bir sýnýfýn operator*'e sahip olup olmadýðýný kontrol eden yardýmcý þablon
	template<typename, typename, typename = void>
	class has_multiply : public std::false_type {};
	template<typename T, typename U>
	class has_multiply<T, U, std::void_t<decltype(std::declval<T&>()* std::declval<U>())>> : public std::true_type {};
	// Bir sýnýfýn operator/'e sahip olup olmadýðýný kontrol eden yardýmcý þablon
	template<typename, typename, typename = void>
	class has_divide : public std::false_type {};
	template<typename T, typename U>
	class has_divide<T, U, std::void_t<decltype(std::declval<T&>() / std::declval<U>())>> : public std::true_type {};
	// Bir sýnýfýn operator%'e sahip olup olmadýðýný kontrol eden yardýmcý þablon
	template<typename, typename, typename = void>
	class has_modulus : public std::false_type {};
	template<typename T, typename U>
	class has_modulus<T, U, std::void_t<decltype(std::declval<T&>() % std::declval<U>())>> : public std::true_type {};
	// Bir sýnýfýn operator&'e sahip olup olmadýðýný kontrol eden yardýmcý þablon
	template<typename, typename, typename = void>
	class has_bitwise_and : public std::false_type {};
	template<typename T, typename U>
	class has_bitwise_and<T, U, std::void_t<decltype(std::declval<T&>()& std::declval<U>())>> : public std::true_type {};
	// Bir sýnýfýn operator|'e sahip olup olmadýðýný kontrol eden yardýmcý þablon
	template<typename, typename, typename = void>
	class has_bitwise_or : public std::false_type {};
	template<typename T, typename U>
	class has_bitwise_or<T, U, std::void_t<decltype(std::declval<T&>() | std::declval<U>())>> : public std::true_type {};
	// Bir sýnýfýn operator^'e sahip olup olmadýðýný kontrol eden yardýmcý þablon
	template<typename, typename, typename = void>
	class has_bitwise_xor : public std::false_type {};
	template<typename T, typename U>
	class has_bitwise_xor<T, U, std::void_t<decltype(std::declval<T&>() ^ std::declval<U>())>> : public std::true_type {};
	// Bir sýnýfýn operator<<'e sahip olup olmadýðýný kontrol eden yardýmcý þablon
	template<typename, typename, typename = void>
	class has_left_shift : public std::false_type {};
	template<typename T, typename U>
	class has_left_shift<T, U, std::void_t<decltype(std::declval<T&>() << std::declval<U>())>> : public std::true_type {};
	// Bir sýnýfýn operator>>'e sahip olup olmadýðýný kontrol eden yardýmcý þablon
	template<typename, typename, typename = void>
	class has_right_shift : public std::false_type {};
	template<typename T, typename U>
	class has_right_shift<T, U, std::void_t<decltype(std::declval<T&>() >> std::declval<U>())>> : public std::true_type {};
	// Bir sýnýfýn operator<'e sahip olup olmadýðýný kontrol eden yardýmcý þablon
	template<typename, typename, typename = void>
	class has_less_than : public std::false_type {};
	template<typename T, typename U>
	class has_less_than<T, U, std::void_t<decltype(std::declval<T&>() < std::declval<U>())>> : public std::true_type {};
	// Bir sýnýfýn operator>'e sahip olup olmadýðýný kontrol eden yardýmcý þablon
	template<typename, typename, typename = void>
	class has_greater_than : public std::false_type {};
	template<typename T, typename U>
	class has_greater_than<T, U, std::void_t<decltype(std::declval<T&>() > std::declval<U>())>> : public std::true_type {};
	// Bir sýnýfýn operator<='e sahip olup olmadýðýný kontrol eden yardýmcý þablon
	template<typename, typename, typename = void>
	class has_less_than_equal : public std::false_type {};
	template<typename T, typename U>
	class has_less_than_equal<T, U, std::void_t<decltype(std::declval<T&>() <= std::declval<U>())>> : public std::true_type {};
	// Bir sýnýfýn operator>='e sahip olup olmadýðýný kontrol eden yardýmcý þablon
	template<typename, typename, typename = void>
	class has_greater_than_equal : public std::false_type {};
	template<typename T, typename U>
	class has_greater_than_equal<T, U, std::void_t<decltype(std::declval<T&>() >= std::declval<U>())>> : public std::true_type {};
	// Bir sýnýfýn operator=='e sahip olup olmadýðýný kontrol eden yardýmcý þablon
	template<typename, typename, typename = void>
	class has_equal : public std::false_type {};
	template<typename T, typename U>
	class has_equal<T, U, std::void_t<decltype(std::declval<T&>() == std::declval<U>())>> : public std::true_type {};
	// Bir sýnýfýn operator!='e sahip olup olmadýðýný kontrol eden yardýmcý þablon
	template<typename, typename, typename = void>
	class has_not_equal : public std::false_type {};
	template<typename T, typename U>
	class has_not_equal<T, U, std::void_t<decltype(std::declval<T&>() != std::declval<U>())>> : public std::true_type {};
	// Bir sýnýfýn operator[]'e sahip olup olmadýðýný kontrol eden yardýmcý þablon
	template<typename, typename, typename = void>
	class has_subscript : public std::false_type {};
	template<typename T, typename U>
	class has_subscript<T, U, std::void_t<decltype(std::declval<T&>()[std::declval<U>()])>> : public std::true_type {};
	// Bir sýnýfýn operator()'e sahip olup olmadýðýný kontrol eden yardýmcý þablon
	template<typename, typename, typename = void>
	class has_function_call : public std::false_type {};
	template<typename T, typename U>
	class has_function_call<T, U, std::void_t<decltype(std::declval<T&>()(std::declval<U>()))>> : public std::true_type{};
	// Bir sýnýfýn operator->'e sahip olup olmadýðýný kontrol eden yardýmcý þablon
	template<typename, typename, typename = void>
	class has_arrow : public std::false_type {};
	template<typename T, typename U>
	class has_arrow<T, U, std::void_t<decltype(std::declval<T&>().operator->())>> : public std::true_type {};
	// Bir sýnýfýn operator->*'e sahip olup olmadýðýný kontrol eden yardýmcý þablon
	template<typename, typename, typename = void>
	class has_arrow_star : public std::false_type {};
	template<typename T, typename U>
	class has_arrow_star<T, U, std::void_t<decltype(std::declval<T&>().operator->*())>> : public std::true_type {};
	// Bir sýnýfýn operator&&'e sahip olup olmadýðýný kontrol eden yardýmcý þablon
	template<typename, typename, typename = void>
	class has_logical_and : public std::false_type {};
	template<typename T, typename U>
	class has_logical_and<T, U, std::void_t<decltype(std::declval<T&>() && std::declval<U>())>> : public std::true_type {};
	// Bir sýnýfýn operator||'e sahip olup olmadýðýný kontrol eden yardýmcý þablon
	template<typename, typename, typename = void>
	class has_logical_or : public std::false_type {};
	template<typename T, typename U>
	class has_logical_or<T, U, std::void_t<decltype(std::declval<T&>() || std::declval<U>())>> : public std::true_type {};
	// Bir sýnýfýn operator&='e sahip olup olmadýðýný kontrol eden yardýmcý þablon
	template<typename, typename, typename = void>
	class has_bitwise_and_equal : public std::false_type {};
	template<typename T, typename U>
	class has_bitwise_and_equal<T, U, std::void_t<decltype(std::declval<T&>() &= std::declval<U>())>> : public std::true_type {};
	// Bir sýnýfýn operator|'e sahip olup olmadýðýný kontrol eden yardýmcý þablon
	template<typename, typename, typename = void>
	class has_bitwise_or_equal : public std::false_type {};
	template<typename T, typename U>
	class has_bitwise_or_equal<T, U, std::void_t<decltype(std::declval<T&>() |= std::declval<U>())>> : public std::true_type {};
	// Bir sýnýfýn operator^'e sahip olup olmadýðýný kontrol eden yardýmcý þablon
	template<typename, typename, typename = void>
	class has_bitwise_xor_equal : public std::false_type {};
	template<typename T, typename U>
	class has_bitwise_xor_equal<T, U, std::void_t<decltype(std::declval<T&>() ^= std::declval<U>())>> : public std::true_type {};
	// Bir sýnýfýn operator<<='e sahip olup olmadýðýný kontrol eden yardýmcý þablon
	template<typename, typename, typename = void>
	class has_left_shift_equal : public std::false_type {};
	template<typename T, typename U>
	class has_left_shift_equal<T, U, std::void_t<decltype(std::declval<T&>() <<= std::declval<U>())>> : public std::true_type {};
	// Bir sýnýfýn operator>>='e sahip olup olmadýðýný kontrol eden yardýmcý þablon
	template<typename, typename, typename = void>
	class has_right_shift_equal : public std::false_type {};
	template<typename T, typename U>
	class has_right_shift_equal<T, U, std::void_t<decltype(std::declval<T&>() >>= std::declval<U>())>> : public std::true_type {};
	// Bir sýnýfýn operator%='e sahip olup olmadýðýný kontrol eden yardýmcý þablon
	template<typename, typename, typename = void>
	class has_modulus_equal : public std::false_type {};
	template<typename T, typename U>
	class has_modulus_equal<T, U, std::void_t<decltype(std::declval<T&>() %= std::declval<U>())>> : public std::true_type {};
	// Bir sýnýfýn operator-='e sahip olup olmadýðýný kontrol eden yardýmcý þablon
	template<typename, typename, typename = void>
	class has_minus_equal : public std::false_type {};
	template<typename T, typename U>
	class has_minus_equal<T, U, std::void_t<decltype(std::declval<T&>() -= std::declval<U>())>> : public std::true_type {};
	// Bir sýnýfýn operator*='e sahip olup olmadýðýný kontrol eden yardýmcý þablon
	template<typename, typename, typename = void>
	class has_multiply_equal : public std::false_type {};
	template<typename T, typename U>
	class has_multiply_equal<T, U, std::void_t<decltype(std::declval<T&>() *= std::declval<U>())>> : public std::true_type {};
	// Bir sýnýfýn operator/='e sahip olup olmadýðýný kontrol eden yardýmcý þablon
	template<typename, typename, typename = void>
	class has_divide_equal : public std::false_type {};
	template<typename T, typename U>
	class has_divide_equal<T, U, std::void_t<decltype(std::declval<T&>() /= std::declval<U>())>> : public std::true_type {};
	// Bir sýnýfýn operator%='e sahip olup olmadýðýný kontrol eden yardýmcý þablon
	template<typename, typename, typename = void>
	class has_modulus_equal : public std::false_type {};
	template<typename T, typename U>
	class has_modulus_equal<T, U, std::void_t<decltype(std::declval<T&>() %= std::declval<U>())>> : public std::true_type {};
	// Bir sýnýfýn operator<<='e sahip olup olmadýðýný kontrol eden yardýmcý þablon
	template<typename, typename, typename = void>
	class has_left_shift_equal : public std::false_type {};
	template<typename T, typename U>
	class has_left_shift_equal<T, U, std::void_t<decltype(std::declval<T&>() <<= std::declval<U>())>> : public std::true_type {};
	// Bir sýnýfýn operator>>='e sahip olup olmadýðýný kontrol eden yardýmcý þablon
	template<typename, typename, typename = void>
	class has_right_shift_equal : public std::false_type {};
	template<typename T, typename U>
	class has_right_shift_equal<T, U, std::void_t<decltype(std::declval<T&>() >>= std::declval<U>())>> : public std::true_type {};
	// Bir sýnýfýn operator&='e sahip olup olmadýðýný kontrol eden yardýmcý þablon
	template<typename, typename, typename = void>
	class has_bitwise_and_equal : public std::false_type {};
	template<typename T, typename U>
	class has_bitwise_and_equal<T, U, std::void_t<decltype(std::declval<T&>() &= std::declval<U>())>> : public std::true_type {};
	// Bir sýnýfýn operator|'e sahip olup olmadýðýný kontrol eden yardýmcý þablon
	template<typename, typename, typename = void>
	class has_bitwise_or_equal : public std::false_type {};
	template<typename T, typename U>
	class has_bitwise_or_equal<T, U, std::void_t<decltype(std::declval<T&>() |= std::declval<U>())>> : public std::true_type {};
	// Bir sýnýfýn operator^'e sahip olup olmadýðýný kontrol eden yardýmcý þablon
	template<typename, typename, typename = void>
	class has_bitwise_xor_equal : public std::false_type {};
	template<typename T, typename U>
	class has_bitwise_xor_equal<T, U, std::void_t<decltype(std::declval<T&>() ^= std::declval<U>())>> : public std::true_type {};
	// Bir sýnýfýn operator<<'e sahip olup olmadýðýný kontrol eden yardýmcý þablon


	template<typename U,typename T = ValueType>
	inline std::enable_if_t<has_plus_equal<T&, const U&>::value, Property&> operator+=(const U& value) {
		get() += value;
		return *this;
	}

	template<typename U, typename T = ValueType>
	inline std::enable_if_t<!has_plus_equal<T&, const U&>::value && has_plus<T&, const U&>::value, Property&> operator+=(const U& value) {
		set(get() + value);
		return *this;
	}

	template<typename U, typename T = ValueType>
	inline std::enable_if_t<!has_plus_equal<T&, const U&>::value && !has_plus<T&, const U&>::value, Property&> operator+=(const U& value) {
		static_assert(has_plus_equal<T&, const U&>::value || has_plus<T&, const U&>::value, "Neither operator+= nor operator+ is defined for type U.");
		return *this;
	}

private:
	ClassType& instance_;
};