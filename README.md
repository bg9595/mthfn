A C++ class that recovers the syntax of pure math, making it quick and easy to write and use mathematical expressions.


## Sample Usage
### Variables 

Define a variable

    var x("x");

### Functions
To define a polynomial

    mathFn f = 3*(x^2) + x;         
    
  Parentheses must be used with ^ since this operator has low precedence. Functions can be combined using +, -, /, *, ^, and ()
  
    mathFn g = sin(cos(x))

  The x is optional here; `sin(cos)` would also work. Some other examples
  
    mathFn h = -g(f + g / f) - 3;

    g = 2*g;

    mathFn k = x^(x^2)

  To plug numbers into these functions

    cout <<     f(2)                  << endl << 
                g(.5)                 << endl <<
                h(7)                 << endl <<
                h(f+g(h) / f)(3)      << endl;

  To print a function
  
    cout << g.print() << endl;      // prints "sin(cos(x))"





  ### Multivariable functions
  Define more variables
  
    var y("y");
    var z("z");
    var t("t");
    var s("s");

  Then
  
    f = cos(2*x + 2*y) / ln((z^2) * t);

  To define which arguments get passed to which variables, and in which order

      setEvaluator(x, y, z, t);    

  Now to plug numbers into this function
  
      cout << f(-2.5, 3, 4, 65);


  ### Custom functions
  Define any C++ function you like that takes a number and returns a number, for example

    long double step(long double x){       return  x < 0  ?  0  :  1;}
    
    long double sign(long double x){       if(x < 0){          return -1;}
                                           else if(x > 0){     return 1;}
                                           return 0;}

  The outputs of these functions could for example depend on other parts of the program. Then mathFn's can be defined

    mathFn stp(step, "step");
    mathFn sgn(sign, "sgn"); 

  And now `stp` and `sgn` can be used as above.    

    f = sin(stp(x)) + sgn(cos(x));

    cout << f(3);
    


  ## Notes
  There are #define lines that hijack the names of many of the standard cmath functions (sin, cos, exp, etc.). These lines can be deleted to recover access to these functions.

  ## Future versions
  Future versions will allow for functions R^n -> R^m, and derivatives.
  



