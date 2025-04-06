#pragma once
#include "checkoperator.h"

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

	template<typename U, typename T = ValueType>
	inline enable_if_t<!is_arithmetic<U>::value && has_plus_equal<T&, const U&>::value, Property&> operator+=(const U& value) {
		get() += value;
		return *this;
	}

	template<typename U, typename T = ValueType>
	inline enable_if_t<!is_arithmetic<U>::value && !has_plus_equal<T&, const U&>::value&& has_plus<T&, const U&>::value, Property&> operator+=(const U& value) {
		set(get() + value);
		return *this;
	}

	template<typename U, typename T = ValueType>
	inline enable_if_t<!is_arithmetic<U>::value && !has_plus_equal<T&, const U&>::value && !has_plus<T&, const U&>::value, Property&> operator+=(const U& value) {
		static_assert(has_plus_equal<T&, const U&>::value || has_plus<T&, const U&>::value, "Neither operator+= nor operator+ is defined with respect to the right operand or the right operand is not an arithmetic type.");
		return *this;
	}

	template<typename U, typename T = ValueType>
	inline enable_if_t<!is_arithmetic<U>::value&& has_minus_equal<T&, const U&>::value, Property&> operator-=(const U& value) {
		get() -= value;
		return *this;
	}
	template<typename U, typename T = ValueType>
	inline enable_if_t<!is_arithmetic<U>::value && !has_minus_equal<T&, const U&>::value&& has_minus<T&, const U&>::value, Property&> operator-=(const U& value) {
		set(get() - value);
		return *this;
	}
	template<typename U, typename T = ValueType>
	inline enable_if_t<!is_arithmetic<U>::value && !has_minus_equal<T&, const U&>::value && !has_minus<T&, const U&>::value, Property&> operator-=(const U& value) {
		static_assert(has_minus_equal<T&, const U&>::value || has_minus<T&, const U&>::value, "Neither operator-= nor operator- is defined with respect to the right operand or the right operand is not an arithmetic type.");
		return *this;
	}
	template<typename U, typename T = ValueType>
	inline enable_if_t<!is_arithmetic<U>::value&& has_multiply_equal<T&, const U&>::value, Property&> operator*=(const U& value) {
		get() *= value;
		return *this;
	}
	template<typename U, typename T = ValueType>
	inline enable_if_t<!is_arithmetic<U>::value && !has_multiply_equal<T&, const U&>::value&& has_multiply<T&, const U&>::value, Property&> operator*=(const U& value) {
		set(get() * value);
		return *this;
	}
	template<typename U, typename T = ValueType>
	inline enable_if_t<!is_arithmetic<U>::value && !has_multiply_equal<T&, const U&>::value && !has_multiply<T&, const U&>::value, Property&> operator*=(const U& value) {
		static_assert(has_multiply_equal<T&, const U&>::value || has_multiply<T&, const U&>::value, "Neither operator*= nor operator* is defined with respect to the right operand or the right operand is not an arithmetic type.");
		return *this;
	}
	template<typename U, typename T = ValueType>
	inline enable_if_t<!is_arithmetic<U>::value&& has_divide_equal<T&, const U&>::value, Property&> operator/=(const U& value) {
		get() /= value;
		return *this;
	}
	template<typename U, typename T = ValueType>
	inline enable_if_t<!is_arithmetic<U>::value && !has_divide_equal<T&, const U&>::value&& has_divide<T&, const U&>::value, Property&> operator/=(const U& value) {
		set(get() / value);
		return *this;
	}
	template<typename U, typename T = ValueType>
	inline enable_if_t<!is_arithmetic<U>::value && !has_divide_equal<T&, const U&>::value && !has_divide<T&, const U&>::value, Property&> operator/=(const U& value) {
		static_assert(has_divide_equal<T&, const U&>::value || has_divide<T&, const U&>::value, "Neither operator/= nor operator/ is defined with respect to the right operand or the right operand is not an arithmetic type.");
		return *this;
	}
	template<typename U, typename T = ValueType>
	inline enable_if_t<!is_arithmetic<U>::value&& has_modulus_equal<T&, const U&>::value, Property&> operator%=(const U& value) {
		get() %= value;
		return *this;
	}
	template<typename U, typename T = ValueType>
	inline enable_if_t<!is_arithmetic<U>::value && !has_modulus_equal<T&, const U&>::value&& has_modulus<T&, const U&>::value, Property&> operator%=(const U& value) {
		set(get() % value);
		return *this;
	}
	template<typename U, typename T = ValueType>
	inline enable_if_t<!is_arithmetic<U>::value && !has_modulus_equal<T&, const U&>::value && !has_modulus<T&, const U&>::value, Property&> operator%=(const U& value) {
		static_assert(has_modulus_equal<T&, const U&>::value || has_modulus<T&, const U&>::value, "Neither operator%= nor operator% is defined with respect to the right operand or the right operand is not an arithmetic type.");
		return *this;
	}
	template<typename U, typename T = ValueType>
	inline enable_if_t<!is_arithmetic<U>::value&& has_bitwise_and_equal<T&, const U&>::value, Property&> operator&=(const U& value) {
		get() &= value;
		return *this;
	}
	template<typename U, typename T = ValueType>
	inline enable_if_t<!is_arithmetic<U>::value && !has_bitwise_and_equal<T&, const U&>::value&& has_bitwise_and<T&, const U&>::value, Property&> operator&=(const U& value) {
		set(get() & value);
		return *this;
	}
	template<typename U, typename T = ValueType>
	inline enable_if_t<!is_arithmetic<U>::value && !has_bitwise_and_equal<T&, const U&>::value && !has_bitwise_and<T&, const U&>::value, Property&> operator&=(const U& value) {
		static_assert(has_bitwise_and_equal<T&, const U&>::value || has_bitwise_and<T&, const U&>::value, "Neither operator&= nor operator& is defined with respect to the right operand or the right operand is not an arithmetic type.");
		return *this;
	}
	template<typename U, typename T = ValueType>
	inline enable_if_t<!is_arithmetic<U>::value&& has_bitwise_or_equal<T&, const U&>::value, Property&> operator|=(const U& value) {
		get() |= value;
		return *this;
	}
	template<typename U, typename T = ValueType>
	inline enable_if_t<!is_arithmetic<U>::value && !has_bitwise_or_equal<T&, const U&>::value&& has_bitwise_or<T&, const U&>::value, Property&> operator|=(const U& value) {
		set(get() | value);
		return *this;
	}
	template<typename U, typename T = ValueType>
	inline enable_if_t<!is_arithmetic<U>::value && !has_bitwise_or_equal<T&, const U&>::value && !has_bitwise_or<T&, const U&>::value, Property&> operator|=(const U& value) {
		static_assert(has_bitwise_or_equal<T&, const U&>::value || has_bitwise_or<T&, const U&>::value, "Neither operator|= nor operator| is defined with respect to the right operand or the right operand is not an arithmetic type.");
		return *this;
	}
	template<typename U, typename T = ValueType>
	inline enable_if_t<!is_arithmetic<U>::value&& has_bitwise_xor_equal<T&, const U&>::value, Property&> operator^=(const U& value) {
		get() ^= value;
		return *this;
	}
	template<typename U, typename T = ValueType>
	inline enable_if_t<!is_arithmetic<U>::value && !has_bitwise_xor_equal<T&, const U&>::value&& has_bitwise_xor<T&, const U&>::value, Property&> operator^=(const U& value) {
		set(get() ^ value);
		return *this;
	}
	template<typename U, typename T = ValueType>
	inline enable_if_t<!is_arithmetic<U>::value && !has_bitwise_xor_equal<T&, const U&>::value && !has_bitwise_xor<T&, const U&>::value, Property&> operator^=(const U& value) {
		static_assert(has_bitwise_xor_equal<T&, const U&>::value || has_bitwise_xor<T&, const U&>::value, "Neither operator^= nor operator^ is defined with respect to the right operand or the right operand is not an arithmetic type.");
		return *this;
	}
	template<typename U, typename T = ValueType>
	inline enable_if_t<!is_arithmetic<U>::value&& has_left_shift_equal<T&, const U&>::value, Property&> operator<<=(const U& value) {
		get() <<= value;
		return *this;
	}
	template<typename U, typename T = ValueType>
	inline enable_if_t<!is_arithmetic<U>::value && !has_left_shift_equal<T&, const U&>::value&& has_left_shift<T&, const U&>::value, Property&> operator<<=(const U& value) {
		set(get() << value);
		return *this;
	}
	template<typename U, typename T = ValueType>
	inline enable_if_t<!is_arithmetic<U>::value && !has_left_shift_equal<T&, const U&>::value && !has_left_shift<T&, const U&>::value, Property&> operator<<=(const U& value) {
		static_assert(has_left_shift_equal<T&, const U&>::value || has_left_shift<T&, const U&>::value, "Neither operator<<= nor operator<< is defined with respect to the right operand or the right operand is not an arithmetic type.");
		return *this;
	}
	template<typename U, typename T = ValueType>
	inline enable_if_t<!is_arithmetic<U>::value&& has_right_shift_equal<T&, const U&>::value, Property&> operator>>=(const U& value) {
		get() >>= value;
		return *this;
	}
	template<typename U, typename T = ValueType>
	inline enable_if_t<!is_arithmetic<U>::value && !has_right_shift_equal<T&, const U&>::value&& has_right_shift<T&, const U&>::value, Property&> operator>>=(const U& value) {
		set(get() >> value);
		return *this;
	}
	template<typename U, typename T = ValueType>
	inline enable_if_t<!is_arithmetic<U>::value && !has_right_shift_equal<T&, const U&>::value && !has_right_shift<T&, const U&>::value, Property&> operator>>=(const U& value) {
		static_assert(has_right_shift_equal<T&, const U&>::value || has_right_shift<T&, const U&>::value, "Neither operator>>= nor operator>> is defined with respect to the right operand or the right operand is not an arithmetic type.");
		return *this;
	}

	template<typename U, typename T = ValueType>
	inline enable_if_t<!is_arithmetic<U>::value&& has_plus<T&, const U&>::value, decltype(declval<T&>() + declval<U&>())> operator+(const U& value) const{
		return get() + value;
	}

	template<typename U, typename T = ValueType>
	inline enable_if_t<!is_arithmetic<U>::value&& has_minus<T&, const U&>::value, decltype(declval<T&>() - declval<U&>())> operator-(const U& value) {
		return get() - value;
	}
	template<typename U, typename T = ValueType>
	inline enable_if_t<!is_arithmetic<U>::value&& has_multiply<T&, const U&>::value, decltype(declval<T&>() * declval<U&>())> operator*(const U& value) {
		return get() * value;
	}
	template<typename U, typename T = ValueType>
	inline enable_if_t<!is_arithmetic<U>::value&& has_divide<T&, const U&>::value, decltype(declval<T&>() / declval<U&>())> operator/(const U& value) {
		return get() / value;
	}
	template<typename U, typename T = ValueType>
	inline enable_if_t<!is_arithmetic<U>::value&& has_modulus<T&, const U&>::value, decltype(declval<T&>() % declval<U&>())> operator%(const U& value) {
		return get() % value;
	}
	template<typename U, typename T = ValueType>
	inline enable_if_t<!is_arithmetic<U>::value&& has_bitwise_and<T&, const U&>::value, decltype(declval<T&>() & declval<U&>())> operator&(const U& value) {
		return get() & value;
	}
	template<typename U, typename T = ValueType>
	inline enable_if_t<!is_arithmetic<U>::value&& has_bitwise_or<T&, const U&>::value, decltype(declval<T&>() | declval<U&>())> operator|(const U& value) {
		return get() | value;
	}
	template<typename U, typename T = ValueType>
	inline enable_if_t<!is_arithmetic<U>::value&& has_bitwise_xor<T&, const U&>::value, decltype(declval<T&>() ^ declval<U&>())> operator^(const U& value) {
		return get() ^ value;
	}
	template<typename U, typename T = ValueType>
	inline enable_if_t<!is_arithmetic<U>::value&& has_left_shift<T&, const U&>::value, decltype(declval<T&>() << declval<U&>())> operator<<(const U& value) {
		return get() << value;
	}
	template<typename U, typename T = ValueType>
	inline enable_if_t<!is_arithmetic<U>::value&& has_right_shift<T&, const U&>::value, decltype(declval<T&>() >> declval<U&>())> operator>>(const U& value) {
		return get() >> value;
	}

	template<typename U, typename T = ValueType>
	inline enable_if_t<!is_arithmetic<U>::value&& has_subscript<T&, const U&>::value, decltype(declval<T&>()[declval<U&>()])> operator[](const U& value) {
		return get()[value];
	}

	template<typename... Args, typename T = ValueType>
	inline enable_if_t<!is_arithmetic<T>::value&& has_function_call<T&, Args...>::value, decltype(std::declval<T&>()(std::declval<Args>()...))> operator()(Args&&... args) {
		return get()(std::forward<Args>(args)...);
	}


private:
	ClassType& instance_;
};

template<auto getter, auto setter, typename U = Property<getter, setter>, typename T = U::ValueType>
inline enable_if_t<!is_arithmetic<U>::value && has_equal<T&, const U&>::value, decltype(declval<T&>() == declval<const U&>())> operator==(const Property<getter, setter>&pr, const U& value)
{
	return pr.get() == value;
}

template<typename U, auto getter, auto setter, typename T = Property<getter, setter>::ValueType>
inline enable_if_t<is_arithmetic<U>::value && has_equal<T&, const U&>::value, decltype(declval<T&>() == declval<const U&>())> operator==(const Property<getter, setter>& pr, const U& value)
{
	return pr.get() == value;
}

template<auto	getter, auto setter, typename U = Property<getter, setter>, typename T = U::ValueType>
inline enable_if_t<!is_arithmetic<U>::value&& has_not_equal<T&, const U&>::value, decltype(declval<T&>() != declval<const U&>())> operator!=(const Property<getter, setter>& pr, const U& value)
{
	return pr.get() != value;
}
template<typename U, auto getter, auto setter, typename T = Property<getter, setter>::ValueType>
inline enable_if_t<is_arithmetic<U>::value&& has_not_equal<T&, const U&>::value, decltype(declval<T&>() != declval<const U&>())> operator!=(const Property<getter, setter>& pr, const U& value)
{
	return pr.get() != value;
}
template<auto getter, auto setter, typename U = Property<getter, setter>, typename T = U::ValueType>
inline enable_if_t<!is_arithmetic<U>::value&& has_less<T&, const U&>::value, decltype(declval<T&>() < declval<const U&>())> operator<(const Property<getter, setter>& pr, const U& value)
{
	return pr.get() < value;
}
template<typename U, auto getter, auto setter, typename T = Property<getter, setter>::ValueType>
inline enable_if_t<is_arithmetic<U>::value&& has_less<T&, const U&>::value, decltype(declval<T&>() < declval<const U&>())> operator<(const Property<getter, setter>& pr, const U& value)
{
	return pr.get() < value;
}
template<auto getter, auto setter, typename U = Property<getter, setter>, typename T = U::ValueType>
inline enable_if_t<!is_arithmetic<U>::value&& has_greater<T&, const U&>::value, decltype(declval<T&>() > declval<const U&>())> operator>(const Property<getter, setter>& pr, const U& value)
{
	return pr.get() > value;
}
template<typename U, auto getter, auto setter, typename T = Property<getter, setter>::ValueType>
inline enable_if_t<is_arithmetic<U>::value&& has_greater<T&, const U&>::value, decltype(declval<T&>() > declval<const U&>())> operator>(const Property<getter, setter>& pr, const U& value)
{
	return pr.get() > value;
}
template<auto getter, auto setter, typename U = Property<getter, setter>, typename T = U::ValueType>
inline enable_if_t<!is_arithmetic<U>::value&& has_less_equal<T&, const U&>::value, decltype(declval<T&>() <= declval<const U&>())> operator<=(const Property<getter, setter>& pr, const U& value)
{
	return pr.get() <= value;
}
template<typename U, auto getter, auto setter, typename T = Property<getter, setter>::ValueType>
inline enable_if_t<is_arithmetic<U>::value&& has_less_equal<T&, const U&>::value, decltype(declval<T&>() <= declval<const U&>())> operator<=(const Property<getter, setter>& pr, const U& value)
{
	return pr.get() <= value;
}
template<auto getter, auto setter, typename U = Property<getter, setter>, typename T = U::ValueType>
inline enable_if_t<!is_arithmetic<U>::value&& has_greater_equal<T&, const U&>::value, decltype(declval<T&>() >= declval<const U&>())> operator>=(const Property<getter, setter>& pr, const U& value)
{
	return pr.get() >= value;
}
template<typename U, auto getter, auto setter, typename T = Property<getter, setter>::ValueType>
inline enable_if_t<is_arithmetic<U>::value&& has_greater_equal<T&, const U&>::value, decltype(declval<T&>() >= declval<const U&>())> operator>=(const Property<getter, setter>& pr, const U& value)
{
	return pr.get() >= value;
}
template<auto getter, auto setter, typename U = Property<getter, setter>, typename T = U::ValueType>
inline enable_if_t<!is_arithmetic<U>::value&& has_logical_and<T&, const U&>::value, decltype(declval<T&>() && declval<const U&>())> operator&&(const Property<getter, setter>& pr, const U& value)
{
	return pr.get() && value;
}
template<typename U, auto getter, auto setter, typename T = Property<getter, setter>::ValueType>
inline enable_if_t<is_arithmetic<U>::value&& has_logical_and<T&, const U&>::value, decltype(declval<T&>() && declval<const U&>())> operator&&(const Property<getter, setter>& pr, const U& value)
{
	return pr.get() && value;
}
template<auto getter, auto setter, typename U = Property<getter, setter>, typename T = U::ValueType>
inline enable_if_t<!is_arithmetic<U>::value&& has_logical_or<T&, const U&>::value, decltype(declval<T&>() || declval<const U&>())> operator||(const Property<getter, setter>& pr, const U& value)
{
	return pr.get() || value;
}
template<typename U, auto getter, auto setter, typename T = Property<getter, setter>::ValueType>
inline enable_if_t<is_arithmetic<U>::value&& has_logical_or<T&, const U&>::value, decltype(declval<T&>() || declval<const U&>())> operator||(const Property<getter, setter>& pr, const U& value)
{
	return pr.get() || value;
}
template<auto getter, auto setter, typename U = Property<getter, setter>, typename T = U::ValueType>
inline enable_if_t<!is_arithmetic<U>::value&& has_bitwise_and<T&, const U&>::value, decltype(declval<T&>()& declval<const U&>())> operator&(const Property<getter, setter>& pr, const U& value)
{
	return pr.get() & value;
}
template<typename U, auto getter, auto setter, typename T = Property<getter, setter>::ValueType>
inline enable_if_t<is_arithmetic<U>::value&& has_bitwise_and<T&, const U&>::value, decltype(declval<T&>()& declval<const U&>())> operator&(const Property<getter, setter>& pr, const U& value)
{
	return pr.get() & value;
}
template<auto getter, auto setter, typename U = Property<getter, setter>, typename T = U::ValueType>
inline enable_if_t<!is_arithmetic<U>::value&& has_bitwise_or<T&, const U&>::value, decltype(declval<T&>() | declval<const U&>())> operator|(const Property<getter, setter>& pr, const U& value)
{
	return pr.get() | value;
}
template<typename U, auto getter, auto setter, typename T = Property<getter, setter>::ValueType>
inline enable_if_t<is_arithmetic<U>::value&& has_bitwise_or<T&, const U&>::value, decltype(declval<T&>() | declval<const U&>())> operator|(const Property<getter, setter>& pr, const U& value)
{
	return pr.get() | value;
}
template<auto getter, auto setter, typename U = Property<getter, setter>, typename T = U::ValueType>
inline enable_if_t<!is_arithmetic<U>::value&& has_bitwise_xor<T&, const U&>::value, decltype(declval<T&>() ^ declval<const U&>())> operator^(const Property<getter, setter>& pr, const U& value)
{
	return pr.get() ^ value;
}
template<typename U, auto getter, auto setter, typename T = Property<getter, setter>::ValueType>
inline enable_if_t<is_arithmetic<U>::value&& has_bitwise_xor<T&, const U&>::value, decltype(declval<T&>() ^ declval<const U&>())> operator^(const Property<getter, setter>& pr, const U& value)
{
	return pr.get() ^ value;
}
template<auto getter, auto setter, typename U = Property<getter, setter>, typename T = U::ValueType>
inline enable_if_t<!is_arithmetic<U>::value&& has_left_shift<T&, const U&>::value, decltype(declval<T&>() << declval<const U&>())> operator<<(const Property<getter, setter>& pr, const U& value)
{
	return pr.get() << value;
}
template<typename U, auto getter, auto setter, typename T = Property<getter, setter>::ValueType>
inline enable_if_t<is_arithmetic<U>::value&& has_left_shift<T&, const U&>::value, decltype(declval<T&>() << declval<const U&>())> operator<<(const Property<getter, setter>& pr, const U& value)
{
	return pr.get() << value;
}
template<auto getter, auto setter, typename U = Property<getter, setter>, typename T = U::ValueType>
inline enable_if_t<!is_arithmetic<U>::value&& has_right_shift<T&, const U&>::value, decltype(declval<T&>() >> declval<const U&>())> operator>>(const Property<getter, setter>& pr, const U& value)
{
	return pr.get() >> value;
}
template<typename U, auto getter, auto setter, typename T = Property<getter, setter>::ValueType>
inline enable_if_t<is_arithmetic<U>::value&& has_right_shift<T&, const U&>::value, decltype(declval<T&>() >> declval<const U&>())> operator>>(const Property<getter, setter>& pr, const U& value)
{
	return pr.get() >> value;
}