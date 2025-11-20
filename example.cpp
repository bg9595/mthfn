#include <iostream>
#include "mathfn13.cpp"

using namespace std;
using namespace mathFunction;

#define sin mathFunction::sin
#define cos mathFunction::cos
#define tan mathFunction::tan

#define exp mathFunction::exp
#define abs mathFunction::abs
#define sqrt mathFunction::sqrt

int main(){

    var x("x"), y("y"), z("z"), s("s"), t("t"), u("u"), v("v"), w("w");

    mathFn f, g, h;
    f = cos(sin(x));
    g = (x^2) + 3*x;
    h(x,y) = -f(g) + y;

    cout << "f = " << f << endl;
    cout << "f(2) = " << f(2) << "\n\n";

    cout << "h = " << h << endl;
    cout << "h(2,3) = " << h(2,3) << "\n\n---\n\n";

    vecMathFn U, V, W, T;
    U(x,y,z) = {cos(x+y), exp(x+z), (y^2) + z};
    V(s,t) = {(s^2) + (t^2), s/t, sin(t)};
    W = U(V);

    T = U(f, h, z);
    T(x,y,z);

    cout << "T = " << T << endl;
    cout << T(1, 2, 3) << "\n\n";
    
    cout << "U = " << U << endl;
    cout << U(-5, 2, 7.3) << endl;
    

    return 0;


}