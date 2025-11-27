# MathFunction
<p align = "left"><img src="header.png" alt="mathfnsample" width=1000>

MathFunction is written with ease of use in mind, and the few lines above explain most of what is needed. Download **mathFunction.h** to your working directory and use `#include "mathFunction.cpp"`, and if desired `using namespace mathFunction`. The file **examples.cpp** has some examples and can be compiled.

Requires C++20. To compile, use eg.

    g++ -std=c++20 examples.cpp




    

## Usage Guide

First define a variable

    var x("x");


### Functions
To define a polynomial

    mathFn f = 3*(x^2) + x;         
    
  Parentheses typically must be used with `^` since this operator has low precedence. Functions can be combined using `+`, `-`, `/`, `*`, `^`, and `()`
  
    mathFn g = sin(cos(x));

  The `(x)` is optional since this is a single variable function; `sin(cos)` would also work. Some other examples
  
    mathFn h = -g(sin(x) + g/f) - 3;

    g = 2*g;

    mathFn k = x^(x^2)

  To plug numbers into these functions

    cout <<     f(2)                  << endl << 
                g(.5)                 << endl <<
                h(7)                  << endl;
These return a `long double`. The class `var` derives from `mathFn`.

  ### Multivariable functions

To define a multivariable function

    var y("y"), var z("z"), var t("t");
    
    f(x,y,z,t) = cos(2*x + 2*y) / ln((z^2) * t);

Calling `f(x,y,z,t)` sets the variables of $f$. This could also be done in two lines

    f = cos(2*x + 2*y) / ln((z^2) * t);
    f(x,y,z,t);
    
 Plugging numbers into this function again returns a `long double`
  
    cout << f(-2.5, 3, 4, 65);

Any combination of functions/numbers/variables can be plugged into functions
    
    mathFn g1 = z^2;
    mathFn g2 = 3*z - cos(y);

    h(x,y,z) = f(g1, g2, z, 4);

which returns a `mathFn` unless all the arguments are numbers. Plugging things into a function $f$ of a single variable can be done without first setting its variable (although setting its variable may be needed if for example $f$ is going to be used to construct another function).

 

###  Functions $\mathbb{R}^n\to\mathbb{R}^m$

Functions with any numbers of components can be defined

    vecMathFn F, G, H, I;
        
    F(x,y,z) = {cos(x * y), z - y, x*y*z};
    G(u,v,w,t) = {u*v, exp(u - v) * t, v^2 / w};
    H(x,y,z) = {3, x*y, ln(z)};
    I(u,v) = {u^2, v^2, u + v, u - v};

Plugging in numbers returns an object of class `vecNum`, which derives from `vector <long double>`, and can be used with `cout`

    cout << G(3.4, 2, -4, 10);

Various operators can be used.

    vecMathFn J, K, L, N;

    J = F(G);

    K = F + H;

    L = F - H;
    
    mathFn m = F * H;        // dot product

    N = F % H;               // cross product

    mathFn P = cos(t) * F;   // multiplies each component of F by cos(t)

    mathFn Q = F / cos(t);   // divides each component of F by cos(t)


 For any of these operators, the dimension and number variables of each of the two functions should make mathematical sense. For the composition, `J` automatically has its variables set to those of `G`, but the others still need their variables set (this could have been done in one line for each function, eg. `m(x,y,z) = F*H`). As with mathFn's, any combination of numbers/functions/variables can be plugged into a vecMathFn. 

    mathFn p1 = z^2 / 4
    mathFn p2 = x*y*z - t;

    vecMathFn R = G(p1, p2, w, 4);

 
The class `vecMathFn` derives from `vector <mathFn>`, so component functions can be accessed with `[]`

    F[0];  // returns cos(x * y)








### Printing

Printing works as follows

    f = cos(sin(x));
    G = {cos(x), sin(x), x};
    
    f.print();        // returns the string "cos(sin(x))"
    G.print();        // returns the string "(cos(x), sin(x), x)"

Functions can be used with `cout`

    cout << f;         // prints "3 * x^2 + x"   
    cout << G;         // prints "(cos(x), sin(x), x)"




    




### `setEvaluator()`

There is a global "default" variable list. Suppose you are working with many functions of the variables $x,y,z$. Rather then setting all their variables individually, call `setEvaluator(x,y,z)`; any function without its variables set will automatically use $x$, $y$, and $z$. Any function that has its variables set can still be called as usual

    mathFn f, g, h;
    f = x*y*z;
    g = 3x^2 + cos(y);
    h = 2*z;   

    mathFn k;
    k(s,t) = cos(2*t) * s;

    setEvaluator(x,y,z);
    
    cout << f(2.3, 4, 7)     << endl <<
            g(3, -4, -10)    << endl <<
            h(0, 10, -5)     << endl <<
            k(4, 2);

If $f$ has already had its variables set, calling `f()` will clear its variables and allow it to use the default variables. A function/variable/number can always be plugged into a function of single variable, whether or not its variable has been set, and whether or not the global variables have been set.
  


### Custom functions

  Define any C++ function you like that takes a number and returns a number, for example

    long double step(long double x){       return  x < 0  ?  0  :  1;}
    
    long double sign(long double x){       if(x < 0){          return -1;}
                                           else if(x > 0){     return 1;}
                                           return 0;}

  The outputs of these functions could for example depend on other parts of the program. Then define

    cppMathFn stp(step, "step");
    cppMathFn sgn(sign, "sgn"); 

  `cppMathFn` derives from `mathFn`. Now `stp` and `sgn` can be used as above.    

    f = sin(stp(x)) + sgn(cos(x));

    cout << f(3);
    
Many of the `<cmath>` functions have been used like this

    mathFn arccos(acos, "arccos");
    f = sin(arccos(x));






### Ambiguous Expressions

If $x,y,z$ are variables, then mathematically, the expression $f(x,y,z)$ could represent two different things

- the statement that $f$ is a function of $x, y,$ and $z$, or
- the composition of $f$ with the functions/variables $x, y,$ and $z$

    
(For example, in the second case, $f$ might be a function of $u,v,w$; plugging in $x,y,z$ amounts to replacing one set of variables with the other.) By default, mathFunction interprets `f(x,y,z)` in the first way, as in the numerous examples above. To accomplish the second, cast at least one of the variables to `mathFn`


    f(u,v,w) = cos(u * v) + w;
    
    g(x,y,z) = f(x, (mathFn) y, z);    // the left side sets the variables of g to x,y,z; the right side plugs x,y,z into f
                                       // the result is that g = cos(x * y) + z, and g has its variables set to x,y,z


### Namespaces

All classes and variables are contained in the `mathFunction` namespace. The following names conflict with the `std` namespace: `sin`, `cos`, `tan`, `exp`, `sqrt`, `abs`.


## Future versions
Plans for future versions include `.differentiate()` functions, and various data types for numbers, including arbitrary precision.




