#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <regex>
#include <cassert>

namespace MyNamespace {
	template<typename _Elem>
	class basic_rational_number {
	private:
		using BRN = basic_rational_number<_Elem>;

		_NODISCARD constexpr _Elem gcd(_Elem _Ax, _Elem _Bx)const noexcept {
			if (_Ax == 0 && _Bx == 0) { return 1; }
			while (_Bx != 0) {
				const _Elem _Ax2 = std::move(_Ax);
				_Ax = std::move(_Bx);
				_Bx = std::move(_Ax2 % _Bx);
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

		_NODISCARD constexpr std::string _To_string(_Elem e)const {
			if constexpr (std::is_signed<_Elem>::value && std::is_integral<_Elem>::value) return std::to_string(e);
			else return e.to_string();
		}
		_NODISCARD constexpr std::wstring _To_wstring(_Elem e)const {
			if constexpr (std::is_signed<_Elem>::value && std::is_integral<_Elem>::value) return std::to_wstring(e);
			else return e.to_wstring();
		}

	public:
		__readonly _Elem p;
		__readonly _Elem q;

		constexpr basic_rational_number<_Elem>(const BRN& x)noexcept = default;
		constexpr basic_rational_number<_Elem>(const _Elem& x, const _Elem& y) :p(x), q(y) { simplify(); }
		constexpr basic_rational_number<_Elem>(const _Elem& x = 0) : p(x), q(1) {  }
		constexpr basic_rational_number<_Elem>(const std::string& x) {
			if (x == "") { p = 0; q = 1; return; }
			std::smatch match;
			static const std::regex re("^(\\d+)/?(\\d*)$");
			if (std::regex_search(x, match, re)) {
				if constexpr (std::is_signed<_Elem>::value && std::is_integral<_Elem>::value) {
					p = std::stoll(match[1].str());
					q = std::stoll(match[2].str());
				}
				else {
					p = match[1].str();
					q = match[2].str();
				}
			}
			else assert(0);
		}
		constexpr ~basic_rational_number() {  }

		_NODISCARD constexpr virtual std::string to_string()const {
			if constexpr (std::is_signed<_Elem>::value && std::is_integral<_Elem>::value) return _To_string(p) + (q == 1 ? "" : "/" + _To_string(q));
			else return "(" + _To_string(p) + (q == 1 ? ")" : ")/(" + _To_string(q) + ")");
		}
		_NODISCARD constexpr virtual std::wstring to_wstring()const {
			if constexpr (std::is_signed<_Elem>::value && std::is_integral<_Elem>::value) return _To_wstring(p) + (q == 1 ? L"" : L"/" + _To_wstring(q));
			else return L"(" + _To_wstring(p) + (q == 1 ? L")" : L")/(" + _To_wstring(q) + L")");
		}

		constexpr void operator=(const BRN& x) { q = x.q; p = x.p; }
		_NODISCARD constexpr BRN operator-()const { return BRN(-p, q); }
		friend std::ostream& operator<<(std::ostream& os, const BRN& x) { os << x.to_string(); return os; }
		friend std::wostream& operator<<(std::wostream& os, const BRN& x) { os << x.to_wstring(); return os; }

		_NODISCARD constexpr bool operator==(const BRN& x)const { return q == x.q && p == x.p; }
		_NODISCARD constexpr bool operator!=(const BRN& x)const { return q != x.q || p != x.p; }
		_NODISCARD constexpr bool operator>(const BRN& x)const { return p * x.q > x.p * q; }
		_NODISCARD constexpr bool operator<(const BRN& x)const { return p * x.q < x.p * q; }
		_NODISCARD constexpr bool operator>=(const BRN& x)const { return !(*this < x); }
		_NODISCARD constexpr bool operator<=(const BRN& x)const { return !(*this > x); }

		_NODISCARD constexpr BRN operator+(const BRN& x)const { return BRN(p * x.q + x.p * q, q * x.q); }
		constexpr void operator+=(const BRN& x) { *this = std::move(BRN(p * x.q + x.p * q, q * x.q)); }
		_NODISCARD constexpr BRN operator-(const BRN& x)const { return BRN(p * x.q - x.p * q, q * x.q); }
		constexpr void operator-=(const BRN& x) { *this = std::move(BRN(p * x.q - x.p * q, q * x.q)); }
		_NODISCARD constexpr BRN operator*(const BRN& x)const { return BRN(x.p * p, x.q * q); }
		constexpr void operator*=(const BRN& x) { *this = std::move(BRN(x.p * p, x.q * q)); }
		_NODISCARD constexpr BRN operator/(const BRN& x)const { return BRN(x.q * p, x.p * q); }
		constexpr void operator/=(const BRN& x) { *this = std::move(BRN(x.q * p, x.p * q)); }

		_NODISCARD constexpr friend BRN abs(const BRN& r) { return BRN(abs(r.p), r.q); }
		_NODISCARD constexpr explicit operator double() { return double(p) / double(q); }
		_NODISCARD constexpr explicit operator float() { return float(p) / float(q); }
		_NODISCARD constexpr explicit operator int() { assert(q == 1); return int(p); }
		_NODISCARD constexpr explicit operator long() { assert(q == 1); return long(p); }
		_NODISCARD constexpr explicit operator short() { assert(q == 1); return short(p); }
		_NODISCARD constexpr explicit operator bool() { return bool(p); }
	};
}