#pragma once
#include <type_traits>
using namespace std;

// Bir s�n�f�n implicit d�n���m operat�r�ne sahip olup olmad���n� kontrol eden yard�mc� �ablon
template<typename From, typename To, typename = void>
class has_implicit_conversion_operator : public std::false_type {};

template<typename From, typename To>
class has_implicit_conversion_operator<From, To, std::void_t<decltype(static_cast<To>(std::declval<From>()))>> : public std::true_type {};

// Bir s�n�f�n operator='e sahip olup olmad���n� kontrol eden yard�mc� �ablon
template<typename T, typename U, typename = void>
class has_assignment : public false_type {};

template<typename T, typename U>
class has_assignment<T, U, void_t<decltype(declval<T&&>() = declval<U&&>())>> : public true_type {};

// Bir s�n�f�n operator+'e sahip olup olmad���n� kontrol eden yard�mc� �ablon
template<typename, typename, typename = void>
class has_plus : public false_type {};

template<typename T, typename U>
class has_plus<T, U, void_t<decltype(declval<T&&>() + declval<U>())>> : public true_type {
public:
	using return_type = decltype(declval<T&>() + declval<U>());
};

// Bir s�n�f�n operator+='e sahip olup olmad���n� kontrol eden yard�mc� �ablon
template<typename, typename, typename = void>
class has_plus_equal : public false_type {};

template<typename T, typename U>
class has_plus_equal<T, U, void_t<decltype(declval<T&&>() += declval<U&&>())>> : public true_type {};

// bir	s�n�f�n operator-'e sahip olup olmad���n� kontrol eden yard�mc� �ablon
template<typename, typename, typename = void>
class has_minus : public false_type {};
template<typename T, typename U>
class has_minus<T, U, void_t<decltype(declval<T&&>() - declval<U>())>> : public true_type {};
// Bir s�n�f�n operator*'e sahip olup olmad���n� kontrol eden yard�mc� �ablon
template<typename, typename, typename = void>
class has_multiply : public false_type {};
template<typename T, typename U>
class has_multiply<T, U, void_t<decltype(declval<T&&>()* declval<U>())>> : public true_type {};
// Bir s�n�f�n operator/'e sahip olup olmad���n� kontrol eden yard�mc� �ablon
template<typename, typename, typename = void>
class has_divide : public false_type {};
template<typename T, typename U>
class has_divide<T, U, void_t<decltype(declval<T&&>() / declval<U>())>> : public true_type {};
// Bir s�n�f�n operator%'e sahip olup olmad���n� kontrol eden yard�mc� �ablon
template<typename, typename, typename = void>
class has_modulus : public false_type {};
template<typename T, typename U>
class has_modulus<T, U, void_t<decltype(declval<T&&>() % declval<U>())>> : public true_type {};
// Bir s�n�f�n operator&'e sahip olup olmad���n� kontrol eden yard�mc� �ablon
template<typename, typename, typename = void>
class has_bitwise_and : public false_type {};
template<typename T, typename U>
class has_bitwise_and<T, U, void_t<decltype(declval<T&&>()& declval<U>())>> : public true_type {};
// Bir s�n�f�n operator|'e sahip olup olmad���n� kontrol eden yard�mc� �ablon
template<typename, typename, typename = void>
class has_bitwise_or : public false_type {};
template<typename T, typename U>
class has_bitwise_or<T, U, void_t<decltype(declval<T&&>() | declval<U>())>> : public true_type {};
// Bir s�n�f�n operator^'e sahip olup olmad���n� kontrol eden yard�mc� �ablon
template<typename, typename, typename = void>
class has_bitwise_xor : public false_type {};
template<typename T, typename U>
class has_bitwise_xor<T, U, void_t<decltype(declval<T&&>() ^ declval<U>())>> : public true_type {};
// Bir s�n�f�n operator<<'e sahip olup olmad���n� kontrol eden yard�mc� �ablon
template<typename, typename, typename = void>
class has_left_shift : public false_type {};
template<typename T, typename U>
class has_left_shift<T, U, void_t<decltype(declval<T&&>() << declval<U>())>> : public true_type {};
// Bir s�n�f�n operator>>'e sahip olup olmad���n� kontrol eden yard�mc� �ablon
template<typename, typename, typename = void>
class has_right_shift : public false_type {};
template<typename T, typename U>
class has_right_shift<T, U, void_t<decltype(declval<T&&>() >> declval<U>())>> : public true_type {};
// Bir s�n�f�n operator<'e sahip olup olmad���n� kontrol eden yard�mc� �ablon
// Bir s�n�f�n operator<='e sahip olup olmad���n� kontrol eden yard�mc� �ablon
template<typename, typename, typename = void>
class has_less_than_equal : public false_type {};
template<typename T, typename U>
class has_less_than_equal<T, U, void_t<decltype(declval<T&&>() <= declval<U&&>())>> : public true_type {};
// Bir s�n�f�n operator>='e sahip olup olmad���n� kontrol eden yard�mc� �ablon
template<typename, typename, typename = void>
class has_greater_than_equal : public false_type {};
template<typename T, typename U>
class has_greater_than_equal<T, U, void_t<decltype(declval<T&&>() >= declval<U&&>())>> : public true_type {};
// Bir s�n�f�n operator=='e sahip olup olmad���n� kontrol eden yard�mc� �ablon
template<typename, typename, typename = void>
class has_equal : public false_type {};
template<typename T, typename U>
class has_equal<T, U, void_t<decltype(declval<T&&>() == declval<U&&>())>> : public true_type {};
// Bir s�n�f�n operator!='e sahip olup olmad���n� kontrol eden yard�mc� �ablon
template<typename, typename, typename = void>
class has_not_equal : public false_type {};
template<typename T, typename U>
class has_not_equal<T, U, void_t<decltype(declval<T&&>() != declval<U&&>())>> : public true_type {};
// Bir s�n�f�n operator<'e sahip olup olmad���n� kontrol eden yard�mc� �ablon
template<typename, typename, typename = void>
class has_less : public false_type {};
template<typename T, typename U>
class has_less<T, U, void_t<decltype(declval<T&&>() < declval<U&&>())>> : public true_type {};
// Bir s�n�f�n operator>'e sahip olup olmad���n� kontrol eden yard�mc� �ablon
template<typename, typename, typename = void>
class has_greater : public false_type {};
template<typename T, typename U>
class has_greater<T, U, void_t<decltype(declval<T&&>() > declval<U&&>())>> : public true_type {
};
// Bir s�n�f�n operator<='e sahip olup olmad���n� kontrol eden yard�mc� �ablon
template<typename, typename, typename = void>
class has_less_equal : public false_type {};
template<typename T, typename U>
class has_less_equal<T, U, void_t<decltype(declval<T&&>() <= declval<U&&>())>> : public true_type {};
// Bir s�n�f�n operator>='e sahip olup olmad���n� kontrol eden yard�mc� �ablon
template<typename, typename, typename = void>
class has_greater_equal : public false_type {};
template<typename T, typename U>
class has_greater_equal<T, U, void_t<decltype(declval<T&&>() >= declval<U&&>())>> : public true_type {};



// Bir s�n�f�n operator[]'e sahip olup olmad���n� kontrol eden yard�mc� �ablon
template<typename, typename, typename = void>
class has_subscript : public false_type {};
template<typename T, typename... Args>
class has_subscript<T, tuple<Args...>, void_t<decltype(declval<T&&>()[declval<Args&&>()...])>> : public true_type {};

// Bir s�n�f�n operator()'e sahip olup olmad���n� kontrol eden yard�mc� �ablon
template<typename, typename, typename = void>
class has_function_call : public false_type {};
template<typename T, typename... Args>
class has_function_call<T, tuple<Args...>, void_t<decltype(apply(declval<T&>(), declval<tuple<Args...>>()))>> : public true_type {};

// Bir s�n�f�n operator->'e sahip olup olmad���n� kontrol eden yard�mc� �ablon
template<typename, typename, typename = void>
class has_arrow : public false_type {};
template<typename T, typename U>
class has_arrow<T, U, void_t<decltype(declval<T&&>().operator->())>> : public true_type {};
// Bir s�n�f�n operator->*'e sahip olup olmad���n� kontrol eden yard�mc� �ablon
template<typename, typename, typename = void>
class has_arrow_star : public false_type {};
template<typename T, typename U>
class has_arrow_star<T, U, void_t<decltype(declval<T&&>().operator->*())>> : public true_type {};
// Bir s�n�f�n operator&&'e sahip olup olmad���n� kontrol eden yard�mc� �ablon
template<typename, typename, typename = void>
class has_logical_and : public false_type {};
template<typename T, typename U>
class has_logical_and<T, U, void_t<decltype(declval<T&&>() && declval<U>())>> : public true_type {};
// Bir s�n�f�n operator||'e sahip olup olmad���n� kontrol eden yard�mc� �ablon
template<typename, typename, typename = void>
class has_logical_or : public false_type {};
template<typename T, typename U>
class has_logical_or<T, U, void_t<decltype(declval<T&&>() || declval<U>())>> : public true_type {};
// Bir s�n�f�n operator&='e sahip olup olmad���n� kontrol eden yard�mc� �ablon
template<typename, typename, typename = void>
class has_bitwise_and_equal : public false_type {};
template<typename T, typename U>
class has_bitwise_and_equal<T, U, void_t<decltype(declval<T&&>() &= declval<U>())>> : public true_type {};
// Bir s�n�f�n operator|'e sahip olup olmad���n� kontrol eden yard�mc� �ablon
template<typename, typename, typename = void>
class has_bitwise_or_equal : public false_type {};
template<typename T, typename U>
class has_bitwise_or_equal<T, U, void_t<decltype(declval<T&&>() |= declval<U>())>> : public true_type {};
// Bir s�n�f�n operator^'e sahip olup olmad���n� kontrol eden yard�mc� �ablon
template<typename, typename, typename = void>
class has_bitwise_xor_equal : public false_type {};
template<typename T, typename U>
class has_bitwise_xor_equal<T, U, void_t<decltype(declval<T&&>() ^= declval<U>())>> : public true_type {};
// Bir s�n�f�n operator<<='e sahip olup olmad���n� kontrol eden yard�mc� �ablon
template<typename, typename, typename = void>
class has_left_shift_equal : public false_type {};
template<typename T, typename U>
class has_left_shift_equal<T, U, void_t<decltype(declval<T&&>() <<= declval<U>())>> : public true_type {};
// Bir s�n�f�n operator>>='e sahip olup olmad���n� kontrol eden yard�mc� �ablon
template<typename, typename, typename = void>
class has_right_shift_equal : public false_type {};
template<typename T, typename U>
class has_right_shift_equal<T, U, void_t<decltype(declval<T&&>() >>= declval<U>())>> : public true_type {};
// Bir s�n�f�n operator%='e sahip olup olmad���n� kontrol eden yard�mc� �ablon
template<typename, typename, typename = void>
class has_modulus_equal : public false_type {};
template<typename T, typename U>
class has_modulus_equal<T, U, void_t<decltype(declval<T&&>() %= declval<U>())>> : public true_type {};
// Bir s�n�f�n operator-='e sahip olup olmad���n� kontrol eden yard�mc� �ablon
template<typename, typename, typename = void>
class has_minus_equal : public false_type {};
template<typename T, typename U>
class has_minus_equal<T, U, void_t<decltype(declval<T&&>() -= declval<U>())>> : public true_type {};
// Bir s�n�f�n operator*='e sahip olup olmad���n� kontrol eden yard�mc� �ablon
template<typename, typename, typename = void>
class has_multiply_equal : public false_type {};
template<typename T, typename U>
class has_multiply_equal<T, U, void_t<decltype(declval<T&&>() *= declval<U>())>> : public true_type {};
// Bir s�n�f�n operator/='e sahip olup olmad���n� kontrol eden yard�mc� �ablon
template<typename, typename, typename = void>
class has_divide_equal : public false_type {};
template<typename T, typename U>
class has_divide_equal<T, U, void_t<decltype(declval<T&&>() /= declval<U>())>> : public true_type {};