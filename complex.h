#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <regex>
#include <cassert>
#include "mytool.h"

template <typename _Elem>
class basic_complex_number {
private:
	using BCN = basic_complex_number<_Elem>;

	_NODISCARD constexpr std::string _To_string(const mytool::can_be_to_string_bracket auto e)const { return std::to_string(e); }
	_NODISCARD constexpr std::string _To_string(const mytool::can_be_dot_to_string auto e)const { return e.to_string(); }

public:
	__readonly _Elem a;
	__readonly _Elem b;

	constexpr basic_complex_number() :a(0), b(0) {  }
	constexpr basic_complex_number(const BCN&)noexcept = default;
	constexpr basic_complex_number(BCN&&)noexcept = default;
	constexpr basic_complex_number(const std::integral auto x) : a(x), b(0) {  }
	constexpr basic_complex_number(const _Elem& x, const _Elem& y) :a(x), b(y) {  }
	constexpr basic_complex_number(_Elem&& x, _Elem&& y) : a(x), b(y) {  }
	constexpr basic_complex_number(const _Elem& x) : a(x), b(0) {  }
	constexpr basic_complex_number(_Elem&& x) : a(x), b(0) {  }
	constexpr basic_complex_number(const std::string& x) {
		if (x == "") { a = 0; b = 0; return; }
		std::smatch match;
		static const std::regex re("^(\\d+)(\\+\\d+i)[0-1]$");
		if (std::regex_search(x, match, re)) {
			if constexpr (std::is_signed<_Elem>::value && std::is_integral<_Elem>::value) {
				a = std::stoll(match[1]);
				b = match[2].length() == 0 ? 0 : std::stoll(std::string(match[2].first + 1, match[2].second - 1));
			}
			else {
				a = match[1].str();
				b = match[2].length() == 0 ? 0 : std::string(match[2].first + 1, match[2].second - 1);
			}
		}
		assert(0);
	}
	constexpr ~basic_complex_number() = default;

	_NODISCARD constexpr bool is_real()noexcept { return b == 0; }

	_NODISCARD constexpr virtual std::string to_string()const { return (a == 0 ? "" : _To_string(a)) + (b == 0 ? "" : (b > 0 ? "+" : "") + _To_string(b) + "i"); }
	friend inline std::ostream& operator<<(std::ostream& os, const BCN& x) { os << x.to_string(); return os; }

	constexpr void operator=(const BCN& x)noexcept { a = x.a; b = x.b; }
	constexpr void operator=(BCN&& x)noexcept { a = x.a; b = x.b; }
	_NODISCARD constexpr BCN operator-()const { return BCN(-a, -b); }

	_NODISCARD constexpr bool operator==(const BCN& x)const { return a == x.a && b == x.b; }
	_NODISCARD constexpr bool operator!=(const BCN& x)const { return a != x.a || b != x.b; }
	_NODISCARD constexpr bool operator>(const BCN& x)const {
		assert(is_real() && x.is_real());
		return a > x.a;
	}
	_NODISCARD constexpr bool operator<(const BCN& x)const {
		assert(is_real() && x.is_real());
		return a < x.a;
	}
	_NODISCARD constexpr bool operator>=(const BCN& x)const {
		assert(is_real() && x.is_real());
		return a >= x.a;
	}
	_NODISCARD constexpr bool operator<=(const BCN& x)const {
		assert(is_real() && x.is_real());
		return a <= x.a;
	}

	_NODISCARD constexpr friend bool operator==(const _Elem& x, const BCN& y) { return x == y.a && 0 == y.b; }
	_NODISCARD constexpr friend bool operator!=(const _Elem& x, const BCN& y) { return x != y.a || 0 != y.b; }
	_NODISCARD constexpr friend bool operator>(const _Elem& x, const BCN& y) {
		assert(y.is_real());
		return x > y.a;
	}
	_NODISCARD constexpr friend bool operator<(const _Elem& x, const BCN& y) {
		assert(y.is_real());
		return x < y.a;
	}
	_NODISCARD constexpr friend bool operator>=(const _Elem& x, const BCN& y) {
		assert(y.is_real());
		return x >= y.a;
	}
	_NODISCARD constexpr friend bool operator<=(const _Elem& x, const BCN& y) {
		assert(y.is_real());
		return x <= y.a;
	}

	_NODISCARD constexpr BCN operator+(const BCN& x)const { return BCN(a + x.a, b + x.b); }
	constexpr void operator+=(const BCN& x) { a += x.a; b += x.b; }
	_NODISCARD constexpr BCN operator-(const BCN& x)const { return BCN(a - x.a, b - x.b); }
	constexpr void operator-=(const BCN& x) { a -= x.a; b -= x.b; }
	_NODISCARD constexpr BCN operator*(const _Elem& x)const { return BCN(a * x, b * x); }
	constexpr void operator*=(const _Elem& x) { *this = BCN(a * x, b * x); }
	_NODISCARD constexpr BCN operator*(const BCN& x)const { return BCN(a * x.a - b * x.b, a * x.b + b * x.a); }
	constexpr void operator*=(const BCN& x) { *this = BCN(a * x.a - b * x.b, a * x.b + b * x.a); }
	_NODISCARD constexpr BCN operator/(const _Elem& x)const { return BCN(a / x, b / x); }
	constexpr void operator/=(const _Elem& x) { *this = BCN(a / x, b / x); }
	_NODISCARD constexpr BCN operator/(const BCN& x)const { return BCN(a * x.a + b * x.b, b * x.a - a * x.b) / x.abspow(); }
	constexpr void operator/=(const BCN& x) { *this = BCN(a * x.a + b * x.b, b * x.a - a * x.b) / x.abspow(); }

	_NODISCARD constexpr friend BCN operator+(const _Elem& x, const BCN& y) { return BCN(x + y.a, y.b); }
	_NODISCARD constexpr friend BCN operator-(const _Elem& x, const BCN& y) { return BCN(x - y.a, y.b); }
	_NODISCARD constexpr friend BCN operator*(const _Elem& x, const BCN& y) { return BCN(x * y.a, x * y.b); }
	_NODISCARD constexpr friend BCN operator/(const _Elem& x, const BCN& y) { return BCN(x) / y; }

	_NODISCARD constexpr _Elem abspow()const { return a * a + b * b; }
	_NODISCARD constexpr friend _Elem abs(const BCN& x) { return std::sqrt(x.abspow()); }
	_NODISCARD constexpr explicit operator bool() { return a == 0 && b == 0; }
};