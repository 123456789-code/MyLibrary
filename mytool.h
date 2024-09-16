#pragma once
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

namespace mytool {
	template <typename T, typename ... others>
	constexpr void print(const T& t, const others& ... o) {
		std::cout << t;
		if constexpr (sizeof...(o) != 0) { print(o...); }
	}

	template <typename S1, typename S2, typename T, typename ... others>
	constexpr void prints(const S1& sep, const S2& end, const T& t, const others& ... o) {
		if constexpr (sizeof...(o) == 0) { std::cout << t << end; }
		else {
			std::cout << t << sep;
			prints(sep, end, o...);
		}
	}

	template <typename T, typename ... others>
	constexpr void printw(const T& t, const others& ... o) {
		std::wcout << t;
		if constexpr (sizeof...(o) != 0) { printw(o...); }
	}

	template <typename ... others>
	constexpr void printa(const others& ... o) {
		if constexpr (sizeof...(o) == 0) { std::cout << std::endl; }
		else { prints(" ", "\n", o...); }
		std::cout.flush();
	}

	template <typename _T>
	_NODISCARD constexpr _T pow(const _T m, const size_t n, const _T p = 1) {
		_T res = 1;
		if constexpr (p == 1) {
			while (n) {
				if (n & 1) res = res * m;
				m = m * m;
				n >>= 1;
			}
			return res;
		}
		else {
			while (n) {
				if (n & 1) res = res * m % p;
				m = m * m % p;
				n >>= 1;
			}
		}
		return res;
	}

	template<typename _int, typename T>
	_NODISCARD constexpr std::vector<T> base(const _int n, T k) {
		static_assert(std::is_integral<_int>::value, "_int must be a integer");
		std::vector<_int> cache;
		while (k != 0) {
			cache.push_back(k % n);
			k /= n;
		}
		std::reverse(cache.begin(), cache.end());
		return cache;
	}

	template <typename T>
	concept can_be_to_string_bracket = requires { std::to_string(T()); };
	template <typename T>
	concept can_be_dot_to_string = requires { T().to_string(); };
	template <typename T>
	concept has_mod = requires { T() % T(); };
	template <typename T>
	concept has_nagetive = requires { -T(); };
}