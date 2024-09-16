#pragma once
#include <vector>
#include <cassert>

template<typename _Elem, typename _int = int>
requires mytool::has_nagetive<_Elem>
class basic_matrix {
private:
	using BM = basic_matrix<_Elem, _int>;

	std::vector<_int> pivot_column;
	constexpr bool is_all_zero(std::vector<_Elem> x) {
		for (const auto& i : x) if (i != 0) return false;
		return true;
	}
	constexpr void row_echelon_form() {
		_int i = 0, main_position = 0;
		while (i < sizeh() && main_position < sizev()) {
			if (matrix[i][main_position] == 0) {
				for (_int j = i + 1; j < sizev(); j++)
					if (matrix[j][main_position] != 0) {
						swap(matrix[i], matrix[j]);
						break;
					}
				if (matrix[i][main_position] == 0) {
					main_position += 1;
					continue;
				}
			}
			for (_int j = i + 1; j < sizeh(); j++) {
				_Elem factor = matrix[j][main_position] / matrix[i][main_position];
				for (_int k = main_position; k < sizev(); k++) matrix[j][k] -= factor * matrix[i][k];
			}
			pivot_column.push_back(main_position);
			i += 1;
			main_position += 1;
		}
		BM _matrix;
		for (const auto& i : matrix)
			if (!is_all_zero(i)) _matrix.appendh(i);
		*this = std::move(_matrix);
	}

	constexpr void reduced_row_echelon_form() {
		row_echelon_form();
		for (_int i = 0; i < pivot_column.size(); i++) {
			if (matrix[i][pivot_column[i]] != 1) {
				for (_int j = pivot_column[i] + 1; j < sizev(); j++) matrix[i][j] /= matrix[i][pivot_column[i]];
				matrix[i][pivot_column[i]] = 1;
			}
			for (_int j = 0; j < i; j++) {
				_Elem factor = matrix[j][pivot_column[i]];
				for (_int k = pivot_column[i]; k < sizev(); k++) matrix[j][k] -= factor * matrix[i][k];
			}
		}
	}

public:
	__readonly std::vector<std::vector<_Elem>> matrix;

	constexpr basic_matrix() = default;
	constexpr basic_matrix(const BM&) = default;
	constexpr basic_matrix(BM&&) = default;
	constexpr basic_matrix(const _int& x, const _int& y) { matrix.assign(x, std::vector<_Elem>(y)); }
	constexpr basic_matrix(_int&& x, _int&& y) { matrix.assign(x, std::vector<_Elem>(y)); }
	constexpr basic_matrix(const std::vector<std::vector<_Elem>>& matrix) {
		assert(!matrix.empty());
		for (const auto& i : matrix) assert(i.size() == matrix.front().size());
		this->matrix = matrix;
	}
	constexpr basic_matrix(std::vector<std::vector<_Elem>>&& matrix) {
		assert(!matrix.empty());
		for (const auto& i : matrix) assert(i.size() == matrix.front().size());
		this->matrix = matrix;
	}
	constexpr basic_matrix(const std::initializer_list<std::initializer_list<_Elem>>& matrix) { for (const auto& i : matrix) this->matrix.push_back(std::vector<_Elem>(i)); }
	constexpr basic_matrix(std::initializer_list<std::initializer_list<_Elem>>&& matrix) { for (const auto& i : matrix) this->matrix.push_back(std::vector<_Elem>(i)); }
	constexpr ~basic_matrix() = default;

	constexpr void operator=(const BM& x) { matrix = x.matrix; }
	constexpr void operator=(BM&& x)noexcept { matrix = x.matrix; }
	_NODISCARD constexpr bool operator==(const BM& x)const { return matrix == x.matrix; }
	_NODISCARD constexpr bool operator!=(const BM&& x)const { return matrix != x.matrix; }

	_NODISCARD constexpr _int sizeh()const { return matrix.size(); }
	_NODISCARD constexpr _int sizev()const { return matrix.empty() ? 0 : matrix.front().size(); }
	_NODISCARD constexpr _int rank()const {
		BM _matrix = *this;
		_matrix.row_echelon_form();
		return _matrix.pivot_column.size();
	}
	_NODISCARD constexpr bool empty()const { return matrix.empty(); }
	_NODISCARD constexpr const std::vector<_Elem>& operator[](const _int& x)const { return matrix[x]; }
	_NODISCARD constexpr _Elem& ele(const _int& x, const _int& y) { return matrix[x][y]; }
	_NODISCARD constexpr const std::vector<_Elem>& front()const { return matrix.front(); }
	_NODISCARD constexpr const std::vector<_Elem>& back()const { return matrix.back(); }

	constexpr void appendh(const std::vector<_Elem>& x) {
		assert(empty() || (!x.empty() && (matrix.empty() || x.size() == sizev())));
		matrix.push_back(x);
	}
	constexpr void appendh(std::vector<_Elem>&& x) {
		assert(empty() || (!x.empty() && (matrix.empty() || x.size() == sizev())));
		matrix.push_back(x);
	}
	constexpr void appendh(const std::initializer_list<_Elem>& x) {
		assert(empty() || (x.size() != 0 && (matrix.empty() || x.size() == sizev())));
		matrix.push_back(std::vector<_Elem>(x));
	}
	constexpr void appendh(std::initializer_list<_Elem>&& x) {
		assert(empty() || (x.size() != 0 && (matrix.empty() || x.size() == sizev())));
		matrix.push_back(std::vector<_Elem>(x));
	}
	constexpr void poph() {
		assert(empty());
		matrix.pop_back();
	}

	constexpr void appendv(const std::vector<_Elem>& x) {
		if (empty()) {
			appendh(x);
			*this = this->transpose();
			return;
		}
		assert(!x.empty() && x.size() == sizeh());
		for (_int i = 0; i < sizeh(); i++) { matrix[i].push_back(x[i]); }
	}
	constexpr void appendv(std::vector<_Elem>&& x) {
		if (empty()) {
			appendh(x);
			*this = *this->transpose();
			return;
		}
		assert(!x.empty() && x.size() == sizeh());
		for (_int i = 0; i < sizeh(); i++) { matrix[i].push_back(x[i]); }
	}
	constexpr void appendv(const std::initializer_list<_Elem>& x) {
		if (empty()) {
			appendh(x);
			*this = *this->transpose();
			return;
		}
		assert(x.size() != 0 && x.size() == sizeh());
		for (_int i = 0; i < sizeh(); i++) { matrix[i].push_back(x[i]); }
	}
	constexpr void appendv(std::initializer_list<_Elem>&& x) {
		if (empty()) {
			appendh(x);
			*this = *this->transpose();
			return;
		}
		assert(x.size() != 0 && x.size() == sizeh());
		for (_int i = 0; i < sizeh(); i++) { matrix[i].push_back(x[i]); }
	}
	constexpr void popv() {
		assert(empty());
		for (auto& i : matrix) i.pop_back();
	}

	friend inline std::ostream& operator<<(std::ostream& os, const BM& x) {
		for (const auto& i : x.matrix) {
			for (const _Elem& j : i) {
				os << j << " ";
			}
			os << std::endl;
		}
		return os;
	}

	_NODISCARD constexpr BM transpose()const noexcept {
		std::vector<std::vector<_Elem>> _matrix(sizev(), std::vector<_Elem>(sizeh()));
		for (_int i = 0; i < sizeh(); i++) for (_int j = 0; j < sizev(); j++) _matrix[j][i] = matrix[i][j];
		return _matrix;
	}
	_NODISCARD constexpr BM conjugate_transpose()const noexcept {
		std::vector<std::vector<_Elem>> _matrix(sizev(), std::vector<_Elem>(sizeh()));
		for (_int i = 0; i < sizeh(); i++) for (_int j = 0; j < sizev(); j++) _matrix[j][i] = matrix[i][j].conjugate();
		return _matrix;
	}
	_NODISCARD constexpr _Elem trace()const {
		assert(sizeh() == sizev());
		_Elem answer = 0;
		for (_int i = 0; i < sizeh(); i++) answer += matrix[i][i];
		return answer;
	}
	_NODISCARD constexpr BM inverse()const {
		assert(sizeh() == sizev());
		//
		return *this;
	}
	_NODISCARD constexpr _Elem determinant()const {
		assert(sizeh() == sizev());
		BM _matrix = *this;
		_matrix.row_echelon_form();
		if (_matrix.pivot_column.size() == _matrix.sizev()) {
			_Elem answer = 1;
			for (_int i = 0; i < _matrix.sizev(); i++) answer *= _matrix[i][i];
			return answer;
		}
		else return 0;
	}

	_NODISCARD constexpr BM operator+(const BM& x)const {
		assert(sizeh() == x.sizeh() && sizev() == x.sizev());
		BM _matrix = *this;
		for (_int i = 0; i < sizeh(); i++) for (_int j = 0; j < sizev(); j++) _matrix.ele(i, j) += x[i][j];
		return _matrix;
	}
	constexpr void operator+=(const BM& x) { *this = *this + x; }
	_NODISCARD constexpr BM operator-(const BM& x)const {
		assert(sizeh() == x.sizeh() && sizev() == x.sizev());
		BM _matrix = *this;
		for (_int i = 0; i < sizeh(); i++) for (_int j = 0; j < sizev(); j++) _matrix.ele(i, j) -= x[i][j];
		return _matrix;
	}
	constexpr void operator-=(const BM& x) { *this = *this - x; }
	_NODISCARD constexpr BM operator*(const _Elem& x)const {
		BM _matrix = *this;
		for (_int i = 0; i < sizeh(); i++) for (_int j = 0; j < sizev(); j++) _matrix.ele(i, j) *= x;
		return _matrix;
	}
	_NODISCARD constexpr friend BM operator*(const _Elem& x, const BM& y) {
		BM _matrix = y;
		for (_int i = 0; i < sizeh(); i++) for (_int j = 0; j < sizev(); j++) _matrix.ele(i, j) *= x;
		return _matrix;
	}
	constexpr void operator*=(const BM& x) { *this = *this * x; }
	_NODISCARD constexpr BM operator*(const BM& x)const {
		assert(sizev() == x.sizeh());
		std::vector<std::vector<_Elem>> _matrix(sizeh(), std::vector<_Elem>(x.sizev()));
		for (_int i = 0; i < sizeh(); i++) for (_int j = 0; j < x.sizev(); j++) for (_int k = 0; k < sizev(); k++)
			_matrix[i][j] += matrix[i][k] * x[k][j];
		return _matrix;
	}
	_NODISCARD constexpr _Elem operator^(const BM& x)const {
		assert(sizeh() == x.sizeh() && sizev() == x.sizev());
		_Elem answer = 0;
		for (_int i = 0; i < sizeh(); i++) for (_int j = 0; j < sizev(); j++) answer += matrix[i][j] * x[i][j];
		return answer;
	}

	_NODISCARD constexpr BM null_space()const {
		BM _matrix = *this;
		_matrix.reduced_row_echelon_form();
		BM _null_space;
		std::vector<_int> free_column;
		_int j = 0;
		for (const auto& i : _matrix.pivot_column) {
			while (j < i) {
				free_column.push_back(j);
				j++;
			}
			j++;
		}
		for (_int i = _matrix.pivot_column.back() + 1; i < _matrix.sizev(); i++) free_column.push_back(i);
		for (_int i = 0; i < free_column.size(); i++) {
			std::vector<_Elem> list_cache;
			for (const auto& k : _matrix.matrix) list_cache.push_back(k[free_column[i]]);
			for (_int k = 0; k < i; k++) list_cache.push_back(0);
			list_cache.push_back(-1);
			for (_int k = 0; k < (free_column.size() - i - 1); k++) list_cache.push_back(0);
			_null_space.appendv(list_cache);
		}
		return _null_space;
	}
};