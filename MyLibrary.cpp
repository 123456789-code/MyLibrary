#include <iostream>
#include "mytool.h"
#include "rational.h"
#include "complex.h"
#include "matrix.h"
#include "fsm.h"
#include "polynomial.h"
using namespace std;
using namespace mytool;
using rat = basic_rational_number<int>;
using complex = basic_complex_number<rat>;
constexpr static const basic_complex_number<rat> I(0, 1);
using matrix = basic_matrix<rat, size_t>;

int main() {
	complex a(1, 2);
	cout << (a + 6 - 9 * I) / (1 + I);
	return 0;
}