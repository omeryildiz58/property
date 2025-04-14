#pragma once
#include "checkoperator.h"

template<typename T>
concept NonArithmetic = !is_arithmetic_v<T> && !is_pointer_v<T>;

template<typename T>
concept Arithmetic = is_arithmetic_v<T> || is_pointer_v<T>;

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
	using ValueType = remove_reference<GetterType>::type;
	Property(ClassType& instance) : instance_(instance) {}
	template <auto S = setter, typename enable_if_t < S != nullptr, int > = 0>
	Property(ClassType& instance, const ValueType& value) : instance_(instance) { set(value); }

	inline ValueType& get() {
		return (instance_.*getter)();
	}

	inline ValueType& get() const {
		return (instance_.*getter)();
	}

	template<auto S = setter>
	inline enable_if_t<S != nullptr, void> set(const ValueType& value) const {
		(instance_.*setter)(value);
	}
	template<auto S = setter>
	inline enable_if_t<S != nullptr, Property&> operator=(const ValueType& value) const {
		set(value);
		return *this;
	}

	inline operator ValueType& () {
		return get();
	}

	inline operator const ValueType& () const {
		return get();
	}

	template<NonArithmetic U>
		requires(has_plus_equal<ValueType&&, U&&>::value)
	inline auto operator+=(U&& value) {
		get() += value;
		return *this;
	}

	template<NonArithmetic U>
		requires(has_plus<ValueType&&, U&&>::value && !has_plus_equal<ValueType&&, U&&>::value)
	inline auto operator+=(U&& value) {
		set(get() + value);
		return *this;
	}

	template<NonArithmetic U>
		requires(!has_plus_equal<ValueType&&, U&&>::value && !has_plus<ValueType&&, U&&>::value)
	inline auto operator+=(U&& value) {
		static_assert(has_plus_equal<ValueType&&, U&&>::value || has_plus<ValueType&&, U&&>::value, "Neither operator+= nor operator+ is defined with respect to the right operand or the right operand is not an arithmetic type.");
		return *this;
	}

	template<NonArithmetic U>
		requires(has_minus_equal<ValueType&&, U&&>::value)
	inline auto operator-=(U&& value) {
		get() -= value;
		return *this;
	}
	template<NonArithmetic U>
		requires(!has_minus_equal<ValueType&&, U&&>::value&& has_minus<ValueType&&, U&&>::value)
	inline auto operator-=(U&& value) {
		set(get() - value);
		return *this;
	}
	template<NonArithmetic U>
		requires(!has_minus_equal<ValueType&&, U&&>::value && !has_minus<ValueType&&, U&&>::value)
	inline auto operator-=(U&& value) {
		static_assert(has_minus_equal<ValueType&&, U&&>::value || has_minus<ValueType&&, U&&>::value, "Neither operator-= nor operator- is defined with respect to the right operand or the right operand is not an arithmetic type.");
		return *this;
	}
	template<NonArithmetic U>
		requires(has_multiply_equal<ValueType&&, U&&>::value)
	inline auto operator*=(U&& value) {
		get() *= value;
		return *this;
	}
	template<NonArithmetic U>
		requires(!has_multiply_equal<ValueType&&, U&&>::value&& has_multiply<ValueType&&, U&&>::value)
	inline auto operator*=(U&& value) {
		set(get() * value);
		return *this;
	}
	template<NonArithmetic U>
		requires(!has_multiply_equal<ValueType&&, U&&>::value && !has_multiply<ValueType&&, U&&>::value)
	inline auto operator*=(U&& value) {
		static_assert(has_multiply_equal<ValueType&&, U&&>::value || has_multiply<ValueType&&, U&&>::value, "Neither operator*= nor operator* is defined with respect to the right operand or the right operand is not an arithmetic type.");
		return *this;
	}
	template<NonArithmetic U>
		requires(has_divide_equal<ValueType&&, U&&>::value)
	inline auto operator/=(U&& value) {
		get() /= value;
		return *this;
	}
	template<NonArithmetic U>
		requires(!has_divide_equal<ValueType&&, U&&>::value&& has_divide<ValueType&&, U&&>::value)
	inline auto operator/=(U&& value) {
		set(get() / value);
		return *this;
	}
	template<NonArithmetic U>
		requires(!has_divide_equal<ValueType&&, U&&>::value && !has_divide<ValueType&&, U&&>::value)
	inline auto operator/=(U&& value) {
		static_assert(has_divide_equal<ValueType&&, U&&>::value || has_divide<ValueType&&, U&&>::value, "Neither operator/= nor operator/ is defined with respect to the right operand or the right operand is not an arithmetic type.");
		return *this;
	}
	template<NonArithmetic U>
		requires(has_modulus_equal<ValueType&&, U&&>::value)
	inline auto operator%=(U&& value) {
		get() %= value;
		return *this;
	}
	template<NonArithmetic U>
		requires(!has_modulus_equal<ValueType&&, U&&>::value&& has_modulus<ValueType&&, U&&>::value)
	inline auto operator%=(U&& value) {
		set(get() % value);
		return *this;
	}
	template<NonArithmetic U>
		requires(!has_modulus_equal<ValueType&&, U&&>::value && !has_modulus<ValueType&&, U&&>::value)
	inline auto operator%=(U&& value) {
		static_assert(has_modulus_equal<ValueType&&, U&&>::value || has_modulus<ValueType&&, U&&>::value, "Neither operator%= nor operator% is defined with respect to the right operand or the right operand is not an arithmetic type.");
		return *this;
	}
	template<NonArithmetic U>
		requires(has_bitwise_and_equal<ValueType&&, U&&>::value)
	inline auto operator&=(U&& value) {
		get() &= value;
		return *this;
	}
	template<NonArithmetic U>
		requires(!has_bitwise_and_equal<ValueType&&, U&&>::value&& has_bitwise_and<ValueType&&, U&&>::value)
	inline auto operator&=(U&& value) {
		set(get() & value);
		return *this;
	}
	template<NonArithmetic U>
		requires(!has_bitwise_and_equal<ValueType&&, U&&>::value && !has_bitwise_and<ValueType&&, U&&>::value)
	inline auto operator&=(U&& value) {
		static_assert(has_bitwise_and_equal<ValueType&&, U&&>::value || has_bitwise_and<ValueType&&, U&&>::value, "Neither operator&= nor operator& is defined with respect to the right operand or the right operand is not an arithmetic type.");
		return *this;
	}
	template<NonArithmetic U>
		requires(has_bitwise_or_equal<ValueType&&, U&&>::value)
	inline auto operator|=(U&& value) {
		get() |= value;
		return *this;
	}
	template<NonArithmetic U>
		requires(!has_bitwise_or_equal<ValueType&&, U&&>::value&& has_bitwise_or<ValueType&&, U&&>::value)
	inline auto operator|=(U&& value) {
		set(get() | value);
		return *this;
	}
	template<NonArithmetic U>
		requires(!has_bitwise_or_equal<ValueType&&, U&&>::value && !has_bitwise_or<ValueType&&, U&&>::value)
	inline auto operator|=(U&& value) {
		static_assert(has_bitwise_or_equal<ValueType&&, U&&>::value || has_bitwise_or<ValueType&&, U&&>::value, "Neither operator|= nor operator| is defined with respect to the right operand or the right operand is not an arithmetic type.");
		return *this;
	}
	template<NonArithmetic U>
		requires(has_bitwise_xor_equal<ValueType&&, U&&>::value)
	inline auto operator^=(U&& value) {
		get() ^= value;
		return *this;
	}
	template<NonArithmetic U>
		requires(!has_bitwise_xor_equal<ValueType&&, U&&>::value&& has_bitwise_xor<ValueType&&, U&&>::value)
	inline auto operator^=(U&& value) {
		set(get() ^ value);
		return *this;
	}
	template<NonArithmetic U>
		requires(!has_bitwise_xor_equal<ValueType&&, U&&>::value && !has_bitwise_xor<ValueType&&, U&&>::value)
	inline auto operator^=(U&& value) {
		static_assert(has_bitwise_xor_equal<ValueType&&, U&&>::value || has_bitwise_xor<ValueType&&, U&&>::value, "Neither operator^= nor operator^ is defined with respect to the right operand or the right operand is not an arithmetic type.");
		return *this;
	}
	template<NonArithmetic U>
		requires(has_left_shift_equal<ValueType&&, U&&>::value)
	inline auto operator<<=(U&& value) {
		get() <<= value;
		return *this;
	}
	template<NonArithmetic U>
		requires(!has_left_shift_equal<ValueType&&, U&&>::value&& has_left_shift<ValueType&&, U&&>::value)
	inline auto operator<<=(U&& value) {
		set(get() << value);
		return *this;
	}
	template<NonArithmetic U>
		requires(!has_left_shift_equal<ValueType&&, U&&>::value && !has_left_shift<ValueType&&, U&&>::value)
	inline auto operator<<=(U&& value) {
		static_assert(has_left_shift_equal<ValueType&&, U&&>::value || has_left_shift<ValueType&&, U&&>::value, "Neither operator<<= nor operator<< is defined with respect to the right operand or the right operand is not an arithmetic type.");
		return *this;
	}
	template<NonArithmetic U>
		requires(has_right_shift_equal<ValueType&&, U&&>::value)
	inline auto operator>>=(U&& value) {
		get() >>= value;
		return *this;
	}
	template<NonArithmetic U>
		requires(!has_right_shift_equal<ValueType&&, U&&>::value&& has_right_shift<ValueType&&, U&&>::value)
	inline auto operator>>=(U&& value) {
		set(get() >> value);
		return *this;
	}
	template<NonArithmetic U>
		requires(!has_right_shift_equal<ValueType&&, U&&>::value && !has_right_shift<ValueType&&, U&&>::value)
	inline auto operator>>=(U&& value) {
		static_assert(has_right_shift_equal<ValueType&&, U&&>::value || has_right_shift<ValueType&&, U&&>::value, "Neither operator>>= nor operator>> is defined with respect to the right operand or the right operand is not an arithmetic type.");
		return *this;
	}

	template<NonArithmetic U>
		requires (has_plus<ValueType&&, U&&>::value)
	inline auto operator+(U&& value) {
		return get() + value;
	}

	template<NonArithmetic U>
		requires (has_minus<ValueType&&, U&&>::value)
	inline auto operator-(U&& value) {
		return get() - value;
	}
	template<NonArithmetic U>
		requires (has_multiply<ValueType&&, U&&>::value)
	inline auto operator*(U&& value) {
		return get() * value;
	}
	template<NonArithmetic U>
		requires (has_divide<ValueType&&, U&&>::value)
	inline auto operator/(U&& value) {
		return get() / value;
	}
	template<NonArithmetic U>
		requires (has_modulus<ValueType&&, U&&>::value)
	inline auto operator%(U&& value) {
		return get() % value;
	}
	template<NonArithmetic U>
		requires (has_logical_and<ValueType&&, U&&>::value)
	inline auto operator&(U&& value) {
		return get() & value;
	}
	template<NonArithmetic U>
		requires (has_bitwise_and<ValueType, U>::value)
	inline auto operator|(U&& value) {
		return get() | value;
	}
	template<NonArithmetic U>
		requires (has_bitwise_xor<ValueType, U>::value)
	inline auto operator^(U&& value) {
		return get() ^ value;
	}


	template<NonArithmetic U>
		requires (has_left_shift<ValueType, U>::value)
	inline auto operator<<(U&& value) {
		return get() << value;
	}

	template<NonArithmetic U>
		requires (has_right_shift<ValueType, U>::value)
	inline auto operator>>(U&& value) {
		return get() >> value;
	}

	template<Arithmetic... U>
		requires (
	has_subscript<ValueType, tuple<U&&...>>::value
		)
		inline auto operator[](U&&... values) {
		return get()[forward<U&&>(values)...];
	}

	template<Arithmetic... U>
		requires (
	has_subscript<ValueType, tuple<U&...>>::value
		)
		inline auto operator[](U&... values) {
		return get()[forward<U&>(values)...];
	}


	template<Arithmetic... U>
		requires (
	has_function_call<ValueType, std::tuple<U&&...>>::value
		)
		inline auto operator()(U&&... values) {
		return get()(forward<U&&>(values)...);
	}
	template<Arithmetic... U>
		requires (
	has_function_call<ValueType, std::tuple<U&...>>::value
		)
		inline auto operator()(U&... values) {
		return get()(forward<U&>(values)...);
	}

	template<Arithmetic T = ValueType>
		inline T operator->() {
		return get();
	}


private:
	ClassType& instance_;
};

template<auto	getter, auto setter, NonArithmetic U = Property<getter, setter>, typename T = U::ValueType>
	requires(has_equal<T&&, U&&>::value)
inline auto operator==(Property<getter, setter>& pr, U& value)
{
	return pr.get() == value;
}

template<Arithmetic U, auto getter, auto setter, typename T = Property<getter, setter>::ValueType>
	requires(has_equal<T&&, U&&>::value)
inline auto operator==(Property<getter, setter>& pr, U&& value)
{
	return pr.get() == value;
}

template<auto	getter, auto setter, NonArithmetic U = Property<getter, setter>, typename T = U::ValueType>
	requires(has_not_equal<T&&, U&&>::value)
inline auto operator!=(Property<getter, setter>& pr, U& value)
{
	return pr.get() != value;
}
template<Arithmetic U, auto getter, auto setter, typename T = Property<getter, setter>::ValueType>
	requires(has_not_equal<T&&, U&&>::value)
inline auto operator!=(Property<getter, setter>& pr, U&& value)
{
	return pr.get() != value;
}
template<auto getter, auto setter, NonArithmetic U = Property<getter, setter>, typename T = U::ValueType>
	requires(has_less<T&&, U&&>::value)
inline auto operator<(Property<getter, setter>& pr, U& value)
{
	return pr.get() < value;
}
template<Arithmetic U, auto getter, auto setter, typename T = Property<getter, setter>::ValueType>
	requires(has_less<T&&, U&&>::value)
inline auto operator<(Property<getter, setter>& pr, U&& value)
{
	return pr.get() < value;
}
template<auto getter, auto setter, NonArithmetic U = Property<getter, setter>, typename T = U::ValueType>
	requires(has_greater<T&&, U&&>::value)
inline auto operator>(Property<getter, setter>& pr, U& value)
{
	return pr.get() > value;
}
template<Arithmetic U, auto getter, auto setter, typename T = Property<getter, setter>::ValueType>
	requires(has_greater<T&&, U&&>::value)
inline auto operator>(Property<getter, setter>& pr, U&& value)
{
	return pr.get() > value;
}
template<auto getter, auto setter, NonArithmetic U = Property<getter, setter>, typename T = U::ValueType>
	requires(has_less_equal<T&&, U&&>::value)
inline auto operator<=(Property<getter, setter>& pr, U& value)
{
	return pr.get() <= value;
}
template<Arithmetic U, auto getter, auto setter, typename T = Property<getter, setter>::ValueType>
	requires(has_less_equal<T&&, U&&>::value)
inline auto operator<=(Property<getter, setter>& pr, U&& value)
{
	return pr.get() <= value;
}
template<auto getter, auto setter, NonArithmetic U = Property<getter, setter>, typename T = U::ValueType>
	requires(has_greater_equal<T&&, U&&>::value)
inline auto operator>=(Property<getter, setter>& pr, U& value)
{
	return pr.get() >= value;
}
template<Arithmetic U, auto getter, auto setter, typename T = Property<getter, setter>::ValueType>
	requires(has_greater_equal<T&&, U&&>::value)
inline auto operator>=(Property<getter, setter>& pr, U&& value)
{
	return pr.get() >= value;
}
template<auto getter, auto setter, NonArithmetic U = Property<getter, setter>, typename T = U::ValueType>
	requires(has_logical_and<T&&, U&&>::value)
inline auto operator&&(Property<getter, setter>& pr, U& value)
{
	return pr.get() && value;
}
template<Arithmetic U, auto getter, auto setter, typename T = Property<getter, setter>::ValueType>
	requires(has_logical_and<T&&, U&&>::value)
inline auto operator&&(Property<getter, setter>& pr, U&& value)
{
	return pr.get() && value;
}
template<auto getter, auto setter, NonArithmetic U = Property<getter, setter>, typename T = U::ValueType>
	requires(has_logical_or<T&&, U&&>::value)
inline auto operator||(Property<getter, setter>& pr, U& value)
{
	return pr.get() || value;
}
template<Arithmetic U, auto getter, auto setter, typename T = Property<getter, setter>::ValueType>
	requires(has_logical_or<T&&, U&&>::value)
inline auto operator||(Property<getter, setter>& pr, U&& value)
{
	return pr.get() || value;
}
template<auto getter, auto setter, NonArithmetic U = Property<getter, setter>, typename T = U::ValueType>
	requires(has_bitwise_and<T&&, U&&>::value)
inline auto operator&(Property<getter, setter>& pr, U& value)
{
	return pr.get() & value;
}
template<Arithmetic U, auto getter, auto setter, typename T = Property<getter, setter>::ValueType>
	requires(has_bitwise_and<T&&, U&&>::value)
inline auto operator&(Property<getter, setter>& pr, U&& value)
{
	return pr.get() & value;
}
template<auto getter, auto setter, NonArithmetic U = Property<getter, setter>, typename T = U::ValueType>
	requires(has_bitwise_or<T&&, U&&>::value)
inline auto operator|(Property<getter, setter>& pr, U& value)
{
	return pr.get() | value;
}
template<Arithmetic U, auto getter, auto setter, typename T = Property<getter, setter>::ValueType>
	requires(has_bitwise_or<T&&, U&&>::value)
inline auto operator|(Property<getter, setter>& pr, U&& value)
{
	return pr.get() | value;
}
template<auto getter, auto setter, NonArithmetic U = Property<getter, setter>, typename T = U::ValueType>
	requires(has_bitwise_xor<T&&, U&&>::value)
inline auto operator^(Property<getter, setter>& pr, U& value)
{
	return pr.get() ^ value;
}
template<Arithmetic U, auto getter, auto setter, typename T = Property<getter, setter>::ValueType>
	requires(has_bitwise_xor<T&&, U&&>::value)
inline auto operator^(Property<getter, setter>& pr, U&& value)
{
	return pr.get() ^ value;
}
template<auto getter, auto setter, NonArithmetic U = Property<getter, setter>, typename T = U::ValueType>
	requires(has_left_shift<T&&, U&&>::value)
inline auto operator<<(Property<getter, setter>& pr, U& value)
{
	return pr.get() << value;
}
template<Arithmetic U, auto getter, auto setter, typename T = Property<getter, setter>::ValueType>
	requires(has_left_shift<T&&, U&&>::value)
inline auto operator<<(Property<getter, setter>& pr, U&& value)
{
	return pr.get() << value;
}
template<auto getter, auto setter, NonArithmetic U = Property<getter, setter>, typename T = U::ValueType>
	requires(has_right_shift<T&&, U&&>::value)
inline auto operator>>(Property<getter, setter>& pr, U& value)
{
	return pr.get() >> value;
}
template<Arithmetic U, auto getter, auto setter, typename T = Property<getter, setter>::ValueType>
	requires(has_right_shift<T&&, U&&>::value)
inline auto operator>>(Property<getter, setter>& pr, U&& value)
{
	return pr.get() >> value;
}