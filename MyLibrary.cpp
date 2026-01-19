#include <iostream>
#include "mytool.h"
#include "rational.h"
#include "complex.h"
#include "matrix.h"
#include "fsm.h"
#include "polynomial.h"
using namespace std;
using namespace mytool;
using rat = basic_rational_number<intmax_t>;
using complex = basic_complex_number<rat>;

int main() {
	for (int i=0;i<100;i++)
	printa();
	return 0;
}