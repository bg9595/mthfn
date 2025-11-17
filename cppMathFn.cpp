#include "mathFunction.h"

using namespace std;

namespace mathFunction{

cppMathFn::cppMathFn(long double (*fn)(long double), string name) : mathFn(fn, name){}

mathFn cppMathFn::operator()(var& x){      return ((mathFn)(*this))((mathFn&)(x));}

// =================


long double identity(long double x){                          return x;}
long double stp(long double x){                        return  x < 0  ?  0  :  1;}
long double sign(long double x){     if(x < 0){          return -1;}
                                    else if(x > 0){     return 1;}
                                    return 0;}

// ===

cppMathFn sin(std::sin, "sin");
cppMathFn cos(std::cos, "cos");
cppMathFn tan(std::tan, "tan");

cppMathFn arcsin(std::asin, "arcsin");
cppMathFn arccos(std::acos, "arccos");
cppMathFn arctan(std::atan, "arctan");

cppMathFn exp(std::exp, "exp");
cppMathFn ln(std::log, "ln");
cppMathFn sqrt(std::sqrt, "sqrt");

cppMathFn id(identity, "id");
cppMathFn step(stp, "step");
cppMathFn abs(std::abs, "abs");
cppMathFn sgn(sign, "sgn");

}