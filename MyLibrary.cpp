#include <iostream>
#include "print.h"
#include "rational.h"
#include "polynomial.h"
using namespace std;
using namespace MyNamespace;
using rat = basic_rational_number<int>;
using bp = basic_polynomial<rat, int>;

int main() {
    vector<rat> a;
    a.push_back(1);
    int p = 1;
    for (int i = 2; i < 10; i++) {
        a.push_back(rat(1, p));
        p *= i;
    }
    bp b(a);
    print("e^x = ", b, "\n");
    for (double i = 0.1; i < 2.1; i += 0.1)
        print("x=", i, ", e^x=", b.culculate(i), "\n");
    system("pause");
}