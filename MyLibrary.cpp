#include <iostream>
#include "mytool.h"
#include "rational.h"
#include "matrix.h"
#include "fsm.h"
#include "polynomial.h"
using namespace std;
using namespace mytool;

using matrix = basic_matrix<rat, size_t>;

int main() {
	matrix a = { {0,0,1},{0,1,0},{1,0,0} };
	matrix x = (a - matrix({ {1,0,0},{0,1,0},{0,0,1} })).null_space();
	printa(x, a*x);
	return 0;
}