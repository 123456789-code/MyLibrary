#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <regex>
#include <cassert>
#include "mytool.h"

template <typename _Elem>
class basic_rational_number {
private:
	using BRN = basic_rational_number<_Elem>;

	_NODISCARD constexpr _Elem gcd(_Elem _Ax, _Elem _Bx)const noexcept {
		if (_Ax == 0 && _Bx == 0) { return 1; }
		while (_Bx != 0) {
			const _Elem _Ax2 = std::move(_Ax);
			_Ax = std::move(_Bx);
			_Bx = _Ax2 % _Bx;
		}
		return _Ax;
	}

	constexpr void simplify()noexcept {
		assert(q != 0);
		if (p == 0) { q = 1; return; }
		const _Elem x = gcd(p, q);
		p /= x;
		q /= x;
		if (q < 0) { p = -p; q = -q; }
	}

	_NODISCARD constexpr std::string _To_string(const mytool::can_be_to_string_bracket auto e)const { return std::to_string(e); }
	_NODISCARD constexpr std::string _To_string(const mytool::can_be_dot_to_string auto e)const { return e.to_string(); }

public:
	__readonly _Elem p;
	__readonly _Elem q;

	constexpr basic_rational_number() :p(0), q(1) {  }
	constexpr basic_rational_number(const BRN&)noexcept = default;
	constexpr basic_rational_number(BRN&&)noexcept = default;
	constexpr basic_rational_number(const _Elem& x, const _Elem& y) :p(x), q(y) { simplify(); }
	constexpr basic_rational_number(_Elem&& x, _Elem&& y) : p(x), q(y) { simplify(); }
	constexpr basic_rational_number(const _Elem& x) : p(x), q(1) {  }
	constexpr basic_rational_number(_Elem&& x) : p(x), q(1) {  }
	constexpr basic_rational_number(const std::string& x) {
		if (x == "") { p = 0; q = 1; return; }
		std::smatch match;
		static const std::regex re("^(\\d+)(/\\d+)[0-1]$");
		if (std::regex_search(x, match, re)) {
			if constexpr (std::is_signed<_Elem>::value && std::is_integral<_Elem>::value) {
				p = std::stoll(match[1]);
				q = match[2].length() == 0 ? 1 : std::stoll(std::string(match[2].first + 1, match[2].second));
			}
			else {
				p = match[1].str();
				q = match[2].length() == 0 ? 1 : std::string(match[2].first + 1, match[2].second);
			}
		}
		assert(0);
	}
	constexpr ~basic_rational_number() = default;

	_NODISCARD constexpr virtual std::string to_string()const { return _To_string(p) + (q == 1 ? "" : "/" + _To_string(q)); }
	friend inline std::ostream& operator<<(std::ostream& os, const BRN& x) { os << x.to_string(); return os; }
	
	constexpr void operator=(const BRN& x)noexcept { q = x.q; p = x.p; }
	constexpr void operator=(BRN&& x)noexcept { q = x.q; p = x.p; }
	_NODISCARD constexpr BRN operator-()const { return BRN(-p, q); }
	
	_NODISCARD constexpr bool operator==(const BRN& x)const { return p == x.p && q == x.q; }
	_NODISCARD constexpr bool operator!=(const BRN& x)const { return p != x.p || q != x.q; }
	_NODISCARD constexpr bool operator>(const BRN& x)const { return p * x.q > x.p * q; }
	_NODISCARD constexpr bool operator<(const BRN& x)const { return p * x.q < x.p * q; }
	_NODISCARD constexpr bool operator>=(const BRN& x)const { return !(*this < x); }
	_NODISCARD constexpr bool operator<=(const BRN& x)const { return !(*this > x); }

	_NODISCARD constexpr friend bool operator==(const _Elem& x, const BRN& y) { return x == y.p && 1 == y.q; }
	_NODISCARD constexpr friend bool operator!=(const _Elem& x, const BRN& y) { return x != y.p || 1 != y.q; }
	_NODISCARD constexpr friend bool operator>(const _Elem& x, const BRN& y) { return x * y.q > y.p; }
	_NODISCARD constexpr friend bool operator<(const _Elem& x, const BRN& y) { return x * y.q < y.p; }
	_NODISCARD constexpr friend bool operator>=(const _Elem& x, const BRN& y) { return !(x < y); }
	_NODISCARD constexpr friend bool operator<=(const _Elem& x, const BRN& y) { return !(x > y); }

	_NODISCARD constexpr BRN operator+(const BRN& x)const { return BRN(p * x.q + x.p * q, q * x.q); }
	constexpr void operator+=(const BRN& x) { *this = BRN(p * x.q + x.p * q, q * x.q); }
	_NODISCARD constexpr BRN operator-(const BRN& x)const { return BRN(p * x.q - x.p * q, q * x.q); }
	constexpr void operator-=(const BRN& x) { *this = BRN(p * x.q - x.p * q, q * x.q); }
	_NODISCARD constexpr BRN operator*(const BRN& x)const { return BRN(x.p * p, x.q * q); }
	constexpr void operator*=(const BRN& x) { *this = BRN(x.p * p, x.q * q); }
	_NODISCARD constexpr BRN operator/(const BRN& x)const { return BRN(x.q * p, x.p * q); }
	constexpr void operator/=(const BRN& x) { *this = BRN(x.q * p, x.p * q); }

	_NODISCARD constexpr friend BRN operator+(const _Elem& x, const BRN& y) { return BRN(x * y.q + y.p, y.q); }
	_NODISCARD constexpr friend BRN operator-(const _Elem& x, const BRN& y) { return BRN(x * y.q - y.p, y.q); }
	_NODISCARD constexpr friend BRN operator*(const _Elem& x, const BRN& y) { return BRN(x * y.p, y.q); }
	_NODISCARD constexpr friend BRN operator/(const _Elem& x, const BRN& y) { return BRN(x * y.q, y.p); }

	_NODISCARD constexpr friend BRN abs(const BRN& x) { return BRN(abs(x.p), x.q); }
	_NODISCARD constexpr explicit operator double() { return double(p) / double(q); }
	_NODISCARD constexpr explicit operator float() { return float(p) / float(q); }
	_NODISCARD constexpr explicit operator int() { assert(q == 1); return int(p); }
	_NODISCARD constexpr explicit operator long() { assert(q == 1); return long(p); }
	_NODISCARD constexpr explicit operator short() { assert(q == 1); return short(p); }
	_NODISCARD constexpr explicit operator bool() { return bool(p); }
};
	
constexpr static const basic_rational_number<intmax_t> atto(1, 1000000000000000000LL);
constexpr static const basic_rational_number<intmax_t> femto(1, 1000000000000000LL);
constexpr static const basic_rational_number<intmax_t> pico(1, 1000000000000LL);
constexpr static const basic_rational_number<intmax_t> nano(1, 1000000000);
constexpr static const basic_rational_number<intmax_t> micro(1, 1000000);
constexpr static const basic_rational_number<intmax_t> milli(1, 1000);
constexpr static const basic_rational_number<intmax_t> centi(1, 100);
constexpr static const basic_rational_number<intmax_t> deci(1, 10);
constexpr static const basic_rational_number<intmax_t> deca(10, 1);
constexpr static const basic_rational_number<intmax_t> hecto(100, 1);
constexpr static const basic_rational_number<intmax_t> kilo(1000, 1);
constexpr static const basic_rational_number<intmax_t> mega(1000000, 1);
constexpr static const basic_rational_number<intmax_t> giga(1000000000, 1);
constexpr static const basic_rational_number<intmax_t> tera(1000000000000LL, 1);
constexpr static const basic_rational_number<intmax_t> peta(1000000000000000LL, 1);
constexpr static const basic_rational_number<intmax_t> exa(1000000000000000000LL, 1);