#pragma once
#ifndef PROPERTY_H_
#define PROPERTY_H_

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4626)
#pragma warning(disable : 5027)
#pragma warning(disable : 4172)
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#pragma GCC diagnostic ignored "-Wdeprecated-copy"
#pragma GCC diagnostic ignored "-Wreturn-local-addr"
#elif defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#pragma clang diagnostic ignored "-Wdeprecated-copy"
#pragma clang diagnostic ignored "-Wreturn-local-addr"
#endif
#include <musCheckOperator.h>
#include <type_traits>
using	namespace std;

template<typename T>
concept NonArithmetic = !is_arithmetic_v<T> && !is_pointer_v<T>;

template<typename T>
concept Arithmetic = is_arithmetic_v<T> || is_pointer_v<T>;

template<typename T>
concept Moveable = is_rvalue_reference_v<T&&> && !is_array_v<remove_reference_t<T>>;

template<typename T, typename ClassType>
concept IsThisPointer = is_same_v<remove_pointer_t<remove_cvref_t<T>>, ClassType> && is_pointer_v<T>;
//concept IsThisPointer = is_same_v<remove_cvref_t<T>, ClassType> && is_pointer_v<T>;

template<auto Getter, auto Setter = nullptr, auto MoveSetter=Setter>
	requires(is_member_function_pointer_v<decltype(Getter)> && (Setter == nullptr || is_member_function_pointer_v<decltype(Setter)>))
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
	Property(const Property&) = delete;
	Property(Property&&) = delete;

	using ClassType = GetterInfo<decltype(Getter)>::ClassType;
	using GetterType = GetterInfo<decltype(Getter)>::ReturnType;
	using ValueType = remove_reference<GetterType>::type;
	friend ClassType;
private:

	Property(ClassType * instance) : instance_(*instance) {}

public:

	template <auto S = Setter>
		requires(S != nullptr)
	Property(ClassType& instance, const ValueType& value) : instance_(instance) { set(value); }

	inline ValueType& get() & {
		return (instance_.*Getter)();
	}

	inline ValueType& get() const & {
		return (instance_.*Getter)();
	}

	template<typename T, auto S = Setter>
		requires(S != nullptr and !Moveable<T>)
	inline void set(const T& value) {
		(instance_.*Setter)(value);
	}

	template<typename T, auto S = MoveSetter>
		requires(S != nullptr && Moveable<T>)
	inline void set(T&& value) {
		(instance_.*MoveSetter)(move(value));
	}

	template<typename T>
		requires(!is_same_v<T, ValueType>&& has_implicit_conversion_operator<ValueType&&, T&&>::value)
	inline operator T&&() {
		return move(static_cast<T>(get()));
	}

	template<typename T, auto S = MoveSetter>
		requires(S != nullptr && Moveable<T>)
	inline auto&& operator=(T&& value) {
		set(move(value));
		return move(*this);
	}

	template<typename T,auto S = Setter>
		requires(S != nullptr && !Moveable<T>)
	inline auto&& operator=(const T& value) {
		set(value);
		return *this;
	}

	template<typename T,auto S = Setter>
		requires(S != nullptr && !is_same_v<T, ValueType> && has_assignment<ValueType&&, T&&>::value && !is_same_v<remove_reference_t<T>, Property>)
	inline auto&& operator=(T&& value) {
		return move(get() = move(value));
	}

	template<typename T, auto S = Setter>
		requires(S != nullptr && !is_same_v<T, ValueType>&& has_assignment <ValueType&&, decltype(declval<T&&>().operator ValueType& ())> ::value&& is_same_v<remove_reference_t<T>, Property>)
	inline auto&& operator=(T&& value) {
		return move(get() = move(value.operator ValueType&()));
	}



	inline operator ValueType& () {
		return get();
	}

	inline operator const ValueType& () const {
		return get();
	}

	template<NonArithmetic U>
	inline operator ValueType&& () {
		return move(get());
	}

	template<NonArithmetic U>
	inline operator const ValueType&& () const {
		return move(get());
	}

	template<NonArithmetic U>
		requires(has_plus_equal<ValueType&&, U&&>::value)
	inline auto&& operator+=(U&& value) {
		return move(get() += value);
	}

	template<NonArithmetic U>
		requires(has_plus<ValueType&&, U&&>::value && !has_plus_equal<ValueType&&, U&&>::value)
	inline auto&& operator+=(U&& value) {
		set(get() + value);
		return move(get());
	}

	template<NonArithmetic U>
		requires(!has_plus_equal<ValueType&&, U&&>::value && !has_plus<ValueType&&, U&&>::value)
	inline auto&& operator+=(U&& value) {
		static_assert(has_plus_equal<ValueType&&, U&&>::value || has_plus<ValueType&&, U&&>::value, "Neither operator+= nor operator+ is defined with respect to the right operand or the right operand is not an arithmetic type.");
		return move(*this);
	}

	template<NonArithmetic U>
		requires(has_minus_equal<ValueType&&, U&&>::value)
	inline auto&& operator-=(U&& value) {
		return move(get() -= value);
	}
	template<NonArithmetic U>
		requires(!has_minus_equal<ValueType&&, U&&>::value&& has_minus<ValueType&&, U&&>::value)
	inline auto&& operator-=(U&& value) {
		set(get() - value);
		return move(move(get()));
	}
	template<NonArithmetic U>
		requires(!has_minus_equal<ValueType&&, U&&>::value && !has_minus<ValueType&&, U&&>::value)
	inline auto&& operator-=(U&& value) {
		static_assert(has_minus_equal<ValueType&&, U&&>::value || has_minus<ValueType&&, U&&>::value, "Neither operator-= nor operator- is defined with respect to the right operand or the right operand is not an arithmetic type.");
		return move(*this);
	}
	template<NonArithmetic U>
		requires(has_multiply_equal<ValueType&&, U&&>::value)
	inline auto&& operator*=(U&& value) {
		return move(get() *= value);
	}
	template<NonArithmetic U>
		requires(!has_multiply_equal<ValueType&&, U&&>::value&& has_multiply<ValueType&&, U&&>::value)
	inline auto&& operator*=(U&& value) {
		set(get() * value);
		return move(get());
	}
	template<NonArithmetic U>
		requires(!has_multiply_equal<ValueType&&, U&&>::value && !has_multiply<ValueType&&, U&&>::value)
	inline auto&& operator*=(U&& value) {
		static_assert(has_multiply_equal<ValueType&&, U&&>::value || has_multiply<ValueType&&, U&&>::value, "Neither operator*= nor operator* is defined with respect to the right operand or the right operand is not an arithmetic type.");
		return move(*this);
	}
	template<NonArithmetic U>
		requires(has_divide_equal<ValueType&&, U&&>::value)
	inline auto&& operator/=(U&& value) {
		return move(get() /= value);
	}
	template<NonArithmetic U>
		requires(!has_divide_equal<ValueType&&, U&&>::value&& has_divide<ValueType&&, U&&>::value)
	inline auto&& operator/=(U&& value) {
		set(get() / value);
		return move(get());
	}
	template<NonArithmetic U>
		requires(!has_divide_equal<ValueType&&, U&&>::value && !has_divide<ValueType&&, U&&>::value)
	inline auto&& operator/=(U&& value) {
		static_assert(has_divide_equal<ValueType&&, U&&>::value || has_divide<ValueType&&, U&&>::value, "Neither operator/= nor operator/ is defined with respect to the right operand or the right operand is not an arithmetic type.");
		return move(*this);
	}
	template<NonArithmetic U>
		requires(has_modulus_equal<ValueType&&, U&&>::value)
	inline auto&& operator%=(U&& value) {
		return move(get() %= value);
	}
	template<NonArithmetic U>
		requires(!has_modulus_equal<ValueType&&, U&&>::value&& has_modulus<ValueType&&, U&&>::value)
	inline auto&& operator%=(U&& value) {
		set(get() % value);
		return move(get());
	}
	template<NonArithmetic U>
		requires(!has_modulus_equal<ValueType&&, U&&>::value && !has_modulus<ValueType&&, U&&>::value)
	inline auto&& operator%=(U&& value) {
		static_assert(has_modulus_equal<ValueType&&, U&&>::value || has_modulus<ValueType&&, U&&>::value, "Neither operator%= nor operator% is defined with respect to the right operand or the right operand is not an arithmetic type.");
		return move(*this);
	}
	template<NonArithmetic U>
		requires(has_bitwise_and_equal<ValueType&&, U&&>::value)
	inline auto&& operator&=(U&& value) {
		return move(get() &= value);
	}
	template<NonArithmetic U>
		requires(!has_bitwise_and_equal<ValueType&&, U&&>::value&& has_bitwise_and<ValueType&&, U&&>::value)
	inline auto&& operator&=(U&& value) {
		set(get() & value);
		return move(get());
	}
	template<NonArithmetic U>
		requires(!has_bitwise_and_equal<ValueType&&, U&&>::value && !has_bitwise_and<ValueType&&, U&&>::value)
	inline auto&& operator&=(U&& value) {
		static_assert(has_bitwise_and_equal<ValueType&&, U&&>::value || has_bitwise_and<ValueType&&, U&&>::value, "Neither operator&= nor operator& is defined with respect to the right operand or the right operand is not an arithmetic type.");
		return move(*this);
	}
	template<NonArithmetic U>
		requires(has_bitwise_or_equal<ValueType&&, U&&>::value)
	inline auto&& operator|=(U&& value) {
		return move(get() |= value);
	}
	template<NonArithmetic U>
		requires(!has_bitwise_or_equal<ValueType&&, U&&>::value&& has_bitwise_or<ValueType&&, U&&>::value)
	inline auto&& operator|=(U&& value) {
		set(get() | value);
		return move(get());
	}
	template<NonArithmetic U>
		requires(!has_bitwise_or_equal<ValueType&&, U&&>::value && !has_bitwise_or<ValueType&&, U&&>::value)
	inline auto&& operator|=(U&& value) {
		static_assert(has_bitwise_or_equal<ValueType&&, U&&>::value || has_bitwise_or<ValueType&&, U&&>::value, "Neither operator|= nor operator| is defined with respect to the right operand or the right operand is not an arithmetic type.");
		return move(*this);
	}
	template<NonArithmetic U>
		requires(has_bitwise_xor_equal<ValueType&&, U&&>::value)
	inline auto&& operator^=(U&& value) {
		return move(get() ^= value);
	}
	template<NonArithmetic U>
		requires(!has_bitwise_xor_equal<ValueType&&, U&&>::value&& has_bitwise_xor<ValueType&&, U&&>::value)
	inline auto&& operator^=(U&& value) {
		set(get() ^ value);
		return move(get());
	}
	template<NonArithmetic U>
		requires(!has_bitwise_xor_equal<ValueType&&, U&&>::value && !has_bitwise_xor<ValueType&&, U&&>::value)
	inline auto&& operator^=(U&& value) {
		static_assert(has_bitwise_xor_equal<ValueType&&, U&&>::value || has_bitwise_xor<ValueType&&, U&&>::value, "Neither operator^= nor operator^ is defined with respect to the right operand or the right operand is not an arithmetic type.");
		return move(*this);
	}
	template<NonArithmetic U>
		requires(has_left_shift_equal<ValueType&&, U&&>::value)
	inline auto&& operator<<=(U&& value) {
		return move(get() <<= value);
	}
	template<NonArithmetic U>
		requires(!has_left_shift_equal<ValueType&&, U&&>::value&& has_left_shift<ValueType&&, U&&>::value)
	inline auto&& operator<<=(U&& value) {
		set(get() << value);
		return move(get());
	}
	template<NonArithmetic U>
		requires(!has_left_shift_equal<ValueType&&, U&&>::value && !has_left_shift<ValueType&&, U&&>::value)
	inline auto&& operator<<=(U&& value) {
		static_assert(has_left_shift_equal<ValueType&&, U&&>::value || has_left_shift<ValueType&&, U&&>::value, "Neither operator<<= nor operator<< is defined with respect to the right operand or the right operand is not an arithmetic type.");
		return move(*this);
	}
	template<NonArithmetic U>
		requires(has_right_shift_equal<ValueType&&, U&&>::value)
	inline auto&& operator>>=(U&& value) {
		return move(get() >>= value);
	}
	template<NonArithmetic U>
		requires(!has_right_shift_equal<ValueType&&, U&&>::value&& has_right_shift<ValueType&&, U&&>::value)
	inline auto&& operator>>=(U&& value) {
		set(get() >> value);
		return move(value);
	}
	template<NonArithmetic U>
		requires(!has_right_shift_equal<ValueType&&, U&&>::value && !has_right_shift<ValueType&&, U&&>::value)
	inline auto&& operator>>=(U&& value) {
		static_assert(has_right_shift_equal<ValueType&&, U&&>::value || has_right_shift<ValueType&&, U&&>::value, "Neither operator>>= nor operator>> is defined with respect to the right operand or the right operand is not an arithmetic type.");
		return move(*this);
	}

	template<NonArithmetic U>
		requires (has_plus<ValueType&&, U&&>::value)
	inline auto&& operator+(U&& value) {
		return move(get() + value);
	}

	template<NonArithmetic U>
		requires (has_minus<ValueType&&, U&&>::value)
	inline auto&& operator-(U&& value) {
		return move(get() - value);
	}
	template<NonArithmetic U>
		requires (has_multiply<ValueType&&, U&&>::value)
	inline auto&& operator*(U&& value) {
		return move(get() * value);
	}
	template<NonArithmetic U>
		requires (has_divide<ValueType&&, U&&>::value)
	inline auto&& operator/(U&& value) {
		return move(get() / value);
	}
	template<NonArithmetic U>
		requires (has_modulus<ValueType&&, U&&>::value)
	inline auto&& operator%(U&& value) {
		return move(get() % value);
	}
	template<NonArithmetic U>
		requires (has_logical_and<ValueType&&, U&&>::value)
	inline auto&& operator&(U&& value) {
		return move(get() & value);
	}
	template<NonArithmetic U>
		requires (has_bitwise_and<ValueType, U>::value)
	inline auto&& operator|(U&& value) {
		return move(get() | value);
	}
	template<NonArithmetic U>
		requires (has_bitwise_xor<ValueType, U>::value)
	inline auto&& operator^(U&& value) {
		return move(get() ^ value);
	}


	template<NonArithmetic U>
		requires (has_left_shift<ValueType, U>::value)
	inline auto&& operator<<(U&& value) {
		return move(get() << value);
	}

	template<NonArithmetic U>
		requires (has_right_shift<ValueType, U>::value)
	inline auto&& operator>>(U&& value) {
		return move(get() >> value);
	}

	template<Arithmetic... U>
		requires (
	has_subscript<ValueType, tuple<U&&...>>::value
		)
		inline auto&& operator[](U&&... values) {
		return move(get()[forward<U&&>(values)...]);
	}

	template<Arithmetic... U>
		requires (
	has_subscript<ValueType, tuple<U&...>>::value
		)
		inline auto&& operator[](U&... values) {
		return move(get()[forward<U&>(values)...]);
	}


	template<Arithmetic... U>
		requires (
	has_function_call<ValueType, std::tuple<U&&...>>::value
		)
		inline auto&& operator()(U&&... values) {
		return move(get()(forward<U&&>(values)...));
	}
	template<Arithmetic... U>
		requires (
	has_function_call<ValueType, std::tuple<U&...>>::value
		)
		inline auto&& operator()(U&... values) {
		return move(get()(forward<U&>(values)...));
	}

	template<Arithmetic T = ValueType>
		inline T operator->() {
		return move(get());
	}


private:
	ClassType& instance_;
};

template<auto Getter, auto Setter, auto MoveSetter, NonArithmetic U = Property<Getter, Setter, MoveSetter>, typename T = U::ValueType>
	requires(has_equal<T&&, U&&>::value)
inline auto&& operator==(Property<Getter, Setter, MoveSetter>& pr, U& value)
{
	return move(pr.get() == value);
}

template<Arithmetic U, auto Getter, auto Setter, auto MoveSetter, typename T = Property<Getter, Setter, MoveSetter>::ValueType>
	requires(has_equal<T&&, U&&>::value)
inline auto&& operator==(Property<Getter, Setter>& pr, U&& value)
{
	return move(pr.get() == value);
}

template<auto Getter, auto Setter, auto MoveSetter, NonArithmetic U = Property<Getter, Setter, MoveSetter>, typename T = U::ValueType>
	requires(has_not_equal<T&&, U&&>::value)
inline auto&& operator!=(Property<Getter, Setter, MoveSetter>& pr, U& value)
{
	return move(pr.get() != value);
}
template<Arithmetic U, auto Getter, auto Setter, auto MoveSetter, typename T = Property<Getter, Setter, MoveSetter>::ValueType>
	requires(has_not_equal<T&&, U&&>::value)
inline auto&& operator!=(Property<Getter, Setter>& pr, U&& value)
{
	return move(pr.get() != value);
}
template<auto Getter, auto Setter, auto MoveSetter, NonArithmetic U = Property<Getter, Setter, MoveSetter>, typename T = U::ValueType>
	requires(has_less<T&&, U&&>::value)
inline auto&& operator<(Property<Getter, Setter, MoveSetter>& pr, U& value)
{
	return move(pr.get() < value);
}
template<Arithmetic U, auto Getter, auto Setter, auto MoveSetter, typename T = Property<Getter, Setter, MoveSetter>::ValueType>
	requires(has_less<T&&, U&&>::value)
inline auto&& operator<(Property<Getter, Setter, MoveSetter>& pr, U&& value)
{
	return move(pr.get() < value);
}
template<auto Getter, auto Setter, auto MoveSetter, NonArithmetic U = Property<Getter, Setter, MoveSetter>, typename T = U::ValueType>
	requires(has_greater<T&&, U&&>::value)
inline auto&& operator>(Property<Getter, Setter, MoveSetter>& pr, U& value)
{
	return move(pr.get() > value);
}
template<Arithmetic U, auto Getter, auto Setter, auto MoveSetter, typename T = Property<Getter, Setter, MoveSetter>::ValueType>
	requires(has_greater<T&&, U&&>::value)
inline auto&& operator>(Property<Getter, Setter>& pr, U&& value)
{
	return move(pr.get() > value);
}
template<auto Getter, auto Setter, auto MoveSetter, NonArithmetic U = Property<Getter, Setter, MoveSetter>, typename T = U::ValueType>
	requires(has_less_equal<T&&, U&&>::value)
inline auto&& operator<=(Property<Getter, Setter, MoveSetter>& pr, U& value)
{
	return move(pr.get() <= value);
}
template<Arithmetic U, auto Getter, auto Setter, auto MoveSetter, typename T = Property<Getter, Setter, MoveSetter>::ValueType>
	requires(has_less_equal<T&&, U&&>::value)
inline auto&& operator<=(Property<Getter, Setter>& pr, U&& value)
{
	return move(pr.get() <= value);
}
template<auto Getter, auto Setter, auto MoveSetter, NonArithmetic U = Property<Getter, Setter, MoveSetter>, typename T = U::ValueType>
	requires(has_greater_equal<T&&, U&&>::value)
inline auto&& operator>=(Property<Getter, Setter, MoveSetter>& pr, U& value)
{
	return move(pr.get() >= value);
}
template<Arithmetic U, auto Getter, auto Setter, auto MoveSetter, typename T = Property<Getter, Setter, MoveSetter>::ValueType>
	requires(has_greater_equal<T&&, U&&>::value)
inline auto&& operator>=(Property<Getter, Setter>& pr, U&& value)
{
	return move(pr.get() >= value);
}
template<auto Getter, auto Setter, auto MoveSetter, NonArithmetic U = Property<Getter, Setter, MoveSetter>, typename T = U::ValueType>
	requires(has_logical_and<T&&, U&&>::value)
inline auto&& operator&&(Property<Getter, Setter, MoveSetter>& pr, U& value)
{
	return move(pr.get() && value);
}
template<Arithmetic U, auto Getter, auto Setter, auto MoveSetter, typename T = Property<Getter, Setter, MoveSetter>::ValueType>
	requires(has_logical_and<T&&, U&&>::value)
inline auto&& operator&&(Property<Getter, Setter>& pr, U&& value)
{
	return move(pr.get() && value);
}
template<auto Getter, auto Setter, auto MoveSetter, NonArithmetic U = Property<Getter, Setter, MoveSetter>, typename T = U::ValueType>
	requires(has_logical_or<T&&, U&&>::value)
inline auto&& operator||(Property<Getter, Setter, MoveSetter>& pr, U& value)
{
	return move(pr.get() || value);
}
template<Arithmetic U, auto Getter, auto Setter, auto MoveSetter, typename T = Property<Getter, Setter, MoveSetter>::ValueType>
	requires(has_logical_or<T&&, U&&>::value)
inline auto&& operator||(Property<Getter, Setter>& pr, U&& value)
{
	return move(pr.get() || value);
}
template<auto Getter, auto Setter, auto MoveSetter, NonArithmetic U = Property<Getter, Setter, MoveSetter>, typename T = U::ValueType>
	requires(has_bitwise_and<T&&, U&&>::value)
inline auto&& operator&(Property<Getter, Setter, MoveSetter>& pr, U& value)
{
	return move(pr.get() & value);
}
template<Arithmetic U, auto Getter, auto Setter, auto MoveSetter, typename T = Property<Getter, Setter, MoveSetter>::ValueType>
	requires(has_bitwise_and<T&&, U&&>::value)
inline auto&& operator&(Property<Getter, Setter>& pr, U&& value)
{
	return move(pr.get() & value);
}
template<auto Getter, auto Setter, auto MoveSetter, NonArithmetic U = Property<Getter, Setter, MoveSetter>, typename T = U::ValueType>
	requires(has_bitwise_or<T&&, U&&>::value)
inline auto&& operator|(Property<Getter, Setter, MoveSetter>& pr, U& value)
{
	return move(pr.get() | value);
}
template<Arithmetic U, auto Getter, auto Setter, auto MoveSetter, typename T = Property<Getter, Setter, MoveSetter>::ValueType>
	requires(has_bitwise_or<T&&, U&&>::value)
inline auto&& operator|(Property<Getter, Setter>& pr, U&& value)
{
	return move(pr.get() | value);
}
template<auto Getter, auto Setter, auto MoveSetter, NonArithmetic U = Property<Getter, Setter, MoveSetter>, typename T = U::ValueType>
	requires(has_bitwise_xor<T&&, U&&>::value)
inline auto&& operator^(Property<Getter, Setter, MoveSetter>& pr, U& value)
{
	return move(pr.get() ^ value);
}
template<Arithmetic U, auto Getter, auto Setter, auto MoveSetter, typename T = Property<Getter, Setter, MoveSetter>::ValueType>
	requires(has_bitwise_xor<T&&, U&&>::value)
inline auto&& operator^(Property<Getter, Setter>& pr, U&& value)
{
	return move(pr.get() ^ value);
}
template<auto Getter, auto Setter, auto MoveSetter, NonArithmetic U = Property<Getter, Setter, MoveSetter>, typename T = Property<Getter, Setter, MoveSetter>::ValueType>
	requires(has_left_shift<T&&, U&&>::value)
inline auto&& operator<<(Property<Getter, Setter, MoveSetter>& pr, U& value)
{
	return move(pr.get() << value);
}
template<Arithmetic U, auto Getter, auto Setter, auto MoveSetter, typename T = Property<Getter, Setter, MoveSetter>::ValueType>
	requires(has_left_shift<T&&, U&&>::value)
inline auto&& operator<<(Property<Getter, Setter>& pr, U&& value)
{
	return move(pr.get() << value);
}

template<Arithmetic U, auto Getter, auto Setter, auto MoveSetter, typename T = Property<Getter, Setter, MoveSetter>::ValueType>
	requires(has_left_shift<U&&, T&&>::value)
inline auto&& operator<<(U&& value, Property<Getter, Setter>& pr)
{
	return move(value << pr.get());
}

template<auto Getter, auto Setter, auto MoveSetter, NonArithmetic U = Property<Getter, Setter, MoveSetter>, typename T = Property<Getter, Setter, MoveSetter>::ValueType>
	requires(has_left_shift<U&&, T&&>::value)
inline auto&& operator<<(U&& value, const Property<Getter, Setter, MoveSetter>& pr)
{
	return move(value << pr.get());
}

template<auto Getter, auto Setter, auto MoveSetter, NonArithmetic U = Property<Getter, Setter, MoveSetter>, typename T = U::ValueType>
	requires(has_right_shift<T&&, U&&>::value)
inline auto&& operator>>(Property<Getter, Setter, MoveSetter>& pr, U& value)
{
	return move(pr.get() >> value);
}
template<Arithmetic U, auto Getter, auto Setter, auto MoveSetter, typename T = Property<Getter, Setter, MoveSetter>::ValueType>
	requires(has_right_shift<T&&, U&&>::value)
inline auto&& operator>>(Property<Getter, Setter>& pr, U&& value)
{
	return move(pr.get() >> value);
}

template<Arithmetic U, auto Getter, auto Setter, auto MoveSetter, typename T = Property<Getter, Setter, MoveSetter>::ValueType>
	requires(has_right_shift<U&&, T&&>::value)
inline auto&& operator>>(U&& value, Property<Getter, Setter>& pr)
{
	return move(value >> pr.get());
}
template<auto Getter, auto Setter, auto MoveSetter, NonArithmetic U = Property<Getter, Setter, MoveSetter>, typename T = Property<Getter, Setter, MoveSetter>::ValueType>
	requires(has_right_shift<U&&, T&&>::value)
inline auto&& operator>>(U&& value, const Property<Getter, Setter, MoveSetter>& pr)
{
	return move(value >> pr.get());
}

#ifdef _MSC_VER
#pragma warning(pop)
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#elif defined(__clang__)
#pragma clang diagnostic pop
#endif

#endif	// PROPERTY_H_