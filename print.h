#pragma once
#include <iostream>

namespace MyNamespace {
	template<typename T, typename ... others>
	void print(const T& t, const others& ... o) {
		std::cout << t;
		if constexpr (sizeof...(o) != 0) { print(o...); }
	}
	template<typename S1, typename S2, typename T, typename ... others>
	void prints(const S1& sep, const S2& end, const T& t, const others& ... o) {
		if constexpr (sizeof...(o) == 0) { std::cout << t << end; }
		else {
			std::cout << t << sep;
			prints(sep, end, o...);
		}
	}
	template<typename T, typename ... others>
	void printw(const T& t, const others& ... o) {
		std::wcout << t;
		if constexpr (sizeof...(o) != 0) { printw(o...); }
	}
	template<typename S1, typename S2, typename T, typename ... others>
	void printws(const S1& sep, const S2& end, const T& t, const others& ... o) {
		if constexpr (sizeof...(o) == 0) { std::wcout << t << end; }
		else {
			std::wcout << t << sep;
			printws(sep, end, o...);
		}
	}
}