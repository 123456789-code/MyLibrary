#pragma once
#include <string>
#include <iostream>
#include <vector>

namespace MyNamespace {
	template<typename _Elem, typename _int = int>
	class basic_polynomial {
		//static_assert(std::is_signed<_Elem>::value&& std::is_integral<_Elem>::value, "_Elem must be a signed integer");

	private:
		using BP = basic_polynomial<_Elem, _int>;

		constexpr std::string _To_string(_Elem e)const {
			if constexpr (std::is_signed<_Elem>::value && std::is_integral<_Elem>::value) return std::to_string(e);
			else return e.to_string();
		}
		constexpr std::wstring _To_wstring(_Elem e)const {
			if constexpr (std::is_signed<_Elem>::value && std::is_integral<_Elem>::value) return std::to_wstring(e);
			else return e.to_wstring();
		}

		constexpr void simplify()noexcept {
			while (!series.empty() && series.back() == 0) series.pop_back();
			if (series.empty()) series.push_back(0);
		}

		template<typename T, typename ... others>
		constexpr void build_recursion(std::vector<_Elem> *series_recursion, const T& t, const others& ... o) {
			series_recursion->push_back(t);
			/*
			若t不能转换为_Elem，则视为一个列表，转换为vector
			std::vector<_Elem> t_ = static_cast<std::vector<_Elem>>(t);
			series_recursion->insert(series_recursion->end(), t_.begin(), t_.end());
			*/
			if constexpr (sizeof...(o) != 0) { build_recursion(series_recursion, o...); }
		}

	public:
		__readonly std::vector<_Elem> series;
		constexpr basic_polynomial<_Elem, _int>(const BP& x)noexcept = default;
		constexpr basic_polynomial<_Elem, _int>(const _Elem& x = 0) { series.push_back(x); }
		constexpr basic_polynomial<_Elem, _int>(const std::vector<_Elem>& x) : series(x) { simplify(); }
		constexpr basic_polynomial<_Elem, _int>(const std::initializer_list<_Elem>& x) : series(x) { simplify(); }
		constexpr basic_polynomial<_Elem, _int>(const _Elem* x) : series(x, x + sizeof(x)) { simplify(); }
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
		constexpr std::wstring to_wstring()const {
			if (series.size() == 1)
				return _To_wstring(series.front());
			else {
				std::wstring cache = series.front() != 0 ? _To_wstring(series.front()) : L"";
				if (series.size() > 1 && series[1] != 0)
					cache += (series[1] > 0 ? L"+" : L"-") + (abs(series[1]) == 1 ? L"" : _To_wstring(abs(series[1]))) + L"x";
				for (_int i = 2; i < series.size(); i++)
					if (series[i] != 0)
						cache += (series[i] > 0 ? L"+" : L"-") + (abs(series[i]) == 1 ? L"" : _To_wstring(abs(series[i]))) + L"x^" + _To_wstring(i);
				return cache;
			}
		}

		constexpr void operator=(const BP& x) { series = x.series; }
		constexpr BP operator-()const {
			std::vector<_Elem> cache;
			for (const _Elem& i : series) cache.push_back(-i);
			return cache;
		}
		friend std::ostream& operator<<(std::ostream& os, const BP& x) { os << x.to_string(); return os; }
		friend std::wostream& operator<<(std::wostream& os, const BP& x) { os << x.to_wstring(); return os; }

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
		constexpr void operator+=(const BP& x) { *this = std::move(*this + x); }
		constexpr BP operator-(const BP& x)const {
			std::vector<_Elem> cache(std::max(series.size(), x.series.size()));
			_int i = 0;
			for (; i < std::min(series.size(), x.series.size()); i++) cache[i] = series[i] - x.series[i];
			if (series.size() > x.series.size()) for (; i < cache.size(); i++) cache[i] = series[i];
			else for (; i < cache.size(); i++) cache[i] = -x.series[i];
			while (cache.back() == 0) cache.pop_back();
			return BP(cache);
		}
		constexpr void operator-=(const BP& x) { *this = std::move(*this - x); }
		constexpr BP operator*(const BP& x)const {
			std::vector<_Elem> cache(series.size() + x.series.size() - 1);
			for (_int i = 0; i < cache.size(); i++)
				for (_int j = std::max(0, _int(i - x.series.size() + 1)); j < std::min(i + 1, _int(series.size())); j++)
					cache[i] += series[j] * x.series[i - j];
			return BP(cache);
		}
		constexpr void operator*=(const BP& x) { *this = std::move(*this * x); }
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
		constexpr void operator/=(const BP& x) { *this = std::move(divmod(x).first); }
		constexpr BP operator%(const BP& x)const { return divmod(x).second; }
		constexpr void operator%=(const BP& x) { *this = std::move(divmod(x).second); }
	};
}