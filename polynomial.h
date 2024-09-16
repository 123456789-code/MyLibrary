#pragma once
#include <string>
#include <iostream>
#include <vector>
#include "mytool.h"

template <typename _Elem, typename _int = int>
requires mytool::has_nagetive<_Elem>
class basic_polynomial {
private:
	using BP = basic_polynomial<_Elem, _int>;

	_NODISCARD constexpr std::string _To_string(const mytool::can_be_to_string_bracket auto e)const { return std::to_string(e); }
	_NODISCARD constexpr std::string _To_string(const mytool::can_be_dot_to_string auto e)const { return e.to_string(); }

	constexpr void simplify()noexcept {
		while (!series.empty() && series.back() == 0) series.pop_back();
		if (series.empty()) series.push_back(0);
	}

	template<typename T, typename ... others>
	constexpr void build_recursion(std::vector<_Elem> *series_recursion, const T& t, const others& ... o) {
		series_recursion->push_back(t);
		if constexpr (sizeof...(o) != 0) { build_recursion(series_recursion, o...); }
	}

public:
	__readonly std::vector<_Elem> series;
	constexpr basic_polynomial(const BP&)noexcept = default;
	constexpr basic_polynomial(BP&&)noexcept = default;
	constexpr explicit basic_polynomial(const _Elem& x = 0) { series.push_back(x); }
	constexpr explicit basic_polynomial(_Elem&& x = 0) { series.push_back(x); }
	constexpr basic_polynomial(const std::vector<_Elem>& x) : series(x) { simplify(); }
	constexpr basic_polynomial(std::vector<_Elem>&& x) : series(x) { simplify(); }
	constexpr basic_polynomial(const std::initializer_list<_Elem>& x) : series(x) { simplify(); }
	constexpr basic_polynomial(std::initializer_list<_Elem>&& x) : series(x) { simplify(); }
	constexpr basic_polynomial(const _Elem* x) : series(x, x + sizeof(x)) { simplify(); }
	template<typename ... others>
	constexpr basic_polynomial<_Elem, _int>(const others& ... o) { build_recursion(&series, o...); simplify();}
	constexpr ~basic_polynomial() {  }

	template<typename T>
	constexpr T culculate(const T& x)const {
		T cache = 0, power = 1;
		for (_Elem i : series) {
			cache += T(i) * power;
			power *= x;
		}
		return cache;
	}

	constexpr friend BP abs(const BP& r) { return r > 0 ? r : -r; }
	constexpr std::string to_string()const {
		if (series.size() == 1)
			return _To_string(series.front());
		else {
			std::string cache = series.front() != 0 ? _To_string(series.front()) : "";
			if (series.size() > 1 && series[1] != 0)
				cache += (series[1] > 0 ? "+" : "-") + (abs(series[1]) == 1 ? "" : _To_string(abs(series[1]))) + "x";
			for (_int i = 2; i < series.size(); i++)
				if (series[i] != 0)
					cache += (series[i] > 0 ? "+" : "-") + (abs(series[i]) == 1 ? "" : _To_string(abs(series[i]))) + "x^" + _To_string(i);
			return cache;
		}
	}
	constexpr friend std::string to_string(const BP& x) {
		return x.to_string();
	}
	friend inline std::ostream& operator<<(std::ostream& os, const BP& x) { os << x.to_string(); return os; }

	constexpr void operator=(const BP& x) { series = x.series; }
	constexpr void operator=(BP&& x) { series = x.series; }
	constexpr BP operator-()const {
		std::vector<_Elem> cache;
		for (const _Elem& i : series) cache.push_back(-i);
		return cache;
	}

	constexpr bool operator==(const BP& x)const { return series == x.series; }
	constexpr bool operator!=(const BP& x)const { return series != x.series; }
	constexpr bool operator>(const BP& x)const { return series.front() < x.series.front(); }
	constexpr bool operator<(const BP& x)const { return series.front() > x.series.front(); }
	constexpr bool operator>=(const BP& x)const { return !(*this < x); }
	constexpr bool operator<=(const BP& x)const { return !(*this > x); }

	constexpr BP operator+(const BP& x)const {
		std::vector<_Elem> cache(std::max(series.size(), x.series.size()));
		_int i = 0;
		for (; i < std::min(series.size(), x.series.size()); i++) cache[i] = series[i] + x.series[i];
		if (series.size() > x.series.size()) for (; i < cache.size(); i++) cache[i] = series[i];
		else for (; i < cache.size(); i++) cache[i] = x.series[i];
		while (cache.back() == 0) cache.pop_back();
		return BP(cache);
	}
	constexpr void operator+=(const BP& x) { *this = *this + x; }
	constexpr BP operator-(const BP& x)const {
		std::vector<_Elem> cache(std::max(series.size(), x.series.size()));
		_int i = 0;
		for (; i < std::min(series.size(), x.series.size()); i++) cache[i] = series[i] - x.series[i];
		if (series.size() > x.series.size()) for (; i < cache.size(); i++) cache[i] = series[i];
		else for (; i < cache.size(); i++) cache[i] = -x.series[i];
		while (cache.back() == 0) cache.pop_back();
		return BP(cache);
	}
	constexpr void operator-=(const BP& x) { *this = *this - x; }
	constexpr BP operator*(const BP& x)const {
		std::vector<_Elem> cache(series.size() + x.series.size() - 1);
		for (_int i = 0; i < cache.size(); i++)
			for (_int j = std::max(0, _int(i - x.series.size() + 1)); j < std::min(i + 1, _int(series.size())); j++)
				cache[i] += series[j] * x.series[i - j];
		return BP(cache);
	}
	constexpr void operator*=(const BP& x) { *this = *this * x; }
	constexpr std::pair<BP, BP> divmod(const BP& x)const {
		if (series.size() < x.series.size()) return std::pair<BP, BP>(BP(0), *this);
		else {
			std::vector<_Elem> cache = this->series;
			std::vector<_Elem> answer(series.size() - x.series.size() + 1);
			_Elem n;
			for (_int i = series.size() - 1, p = -1; i >= x.series.size() - 1; i--, p--) {
				n = cache[i] / x.series.back();
				answer[answer.size() + p] = n;
				for (_int j = -1; j > -_int(x.series.size()); j--) {
					cache[i + j] -= n * x.series[x.series.size() - 1 + j];
				}
				cache.pop_back();
			}
			return std::make_pair(BP(answer), BP(cache));
		}
	}
	constexpr BP operator/(const BP& x)const { return divmod(x).first; }
	constexpr void operator/=(const BP& x) { *this = divmod(x).first; }
	constexpr BP operator%(const BP& x)const { return divmod(x).second; }
	constexpr void operator%=(const BP& x) { *this = divmod(x).second; }
};