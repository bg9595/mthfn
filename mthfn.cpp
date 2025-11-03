#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

class mathFn;
long double id(long double);

vector <mathFn*> evaluator;

template <class... args>
mathFn retMathFn(args... theArgs);

class mathFn{

public:
    long double (*fn)(long double);

    long double C;      // constant

    mathFn* first;      // first and second are used for operations 2 and higher
    mathFn* second;

    int op;             // operation: 0 constant, 1 function pointer, 2+, 3-, 4*, 5/, 6^, 7()

    string name;

    mathFn* derivative;

    
    mathFn(){

        op = 0;
        C = 1;

        fn = NULL;
        first = second = NULL;   

    }

    mathFn(int C) : mathFn((long double) C){

        if(C == 0){     name = "zero";}
        if(C == 1){     name = "one";}

    }

    mathFn(unsigned int C) : mathFn((long double) C){}

    mathFn(double C) : mathFn((long double) C){}

    mathFn(long double C){
    
        op = 0;
        this->C = C;

        fn = NULL;
        first = second = NULL;

    }

    mathFn(long double (*fn)(long double), string name){

        op = 1;
        C = 1;
        this->fn = fn;

        first = second = NULL;

        this->name = name;
        
    }

    mathFn(const mathFn& f, const mathFn& g, int op){

        this->op = op;

        C = 1;

        fn = NULL;
        
        first = new mathFn; 
        second = new mathFn;

        *first = f;
        *second = g;

    }

    ~mathFn(){

        deleteOut();

    }

    void deleteOut(){

        if(first){
    
            delete first;
            first = NULL;

        }
        if(second){

            delete second;
            second = NULL;

        }

    }

    void copy(const mathFn& f){

        op = f.op;
        C = f.C;
        fn = f.fn;
        name = f.name;
        
        if(f.first && f.second){

            first = new mathFn;
            second = new mathFn;

            *first = *(f.first);
            *second = *(f.second);

        }
        else{

            first = second = NULL;

        }

    }

    void move(mathFn& f){

        fn = f.fn;
        C = f.C;
        first = f.first;
        second = f.second;
        op = f.op;
        name = f.name;

        f.first = f.second = NULL;

    }

    mathFn(const mathFn& f){

        copy(f);

    }

    mathFn(mathFn&& f){

        move(f);

    }

    mathFn& operator= (const mathFn& f){
        
        deleteOut();
        copy(f);

        return *this;             

    }

    mathFn& operator= (mathFn&& f){

        deleteOut();
        move(f);      

        return *this;

    }   

    long double operator()(long double x) const{

        switch(op){

            case 0: return C;
            case 1: return fn(x);
            case 2: return (*first)(x) + (*second)(x);
            case 3: return (*first)(x) - (*second)(x);
            case 4: return (*first)(x) * (*second)(x);
            case 5: return (*first)(x) / (*second)(x);
            case 6: return pow((*first)(x), (*second)(x));
            case 7: return (*first)((*second)(x));

        }

        cout << "long double operator()(long double) err";

        return 0;

    }

    template <class... Ts>
    long double operator()(long double x, Ts... args) const{                return (*this)(vector <long double>{(long double)(x), (long double)(args)...});}

    long double operator()(const vector <long double>& args) const{         return (*this)(args, ::evaluator);}

    long double operator()(const vector <long double>& args, const vector <mathFn*>& evaluator) const{

        switch(op){

            case 0: return C;
            case 1: {       for(int i = 0; i < args.size(); i++){    if(name == evaluator[i]->name){        return args[i];}}}
            case 2: return (*first)(args, evaluator) + (*second)(args, evaluator);
            case 3: return (*first)(args, evaluator) - (*second)(args, evaluator);
            case 4: return (*first)(args, evaluator) * (*second)(args, evaluator);
            case 5: return (*first)(args, evaluator) / (*second)(args, evaluator);
            case 6: return pow((*first)(args, evaluator), (*second)(args, evaluator));
            case 7: return (*first)((*second)(args, evaluator));

        }

        cout << "long double operator()(...) err";

        return 0;
    }

    mathFn operator+(const mathFn& g) const{

        if(name == "zero"){             return g;}
        if(g.name == "zero"){           return *this;} 

        if(op == 0  &&  g.op == 0){     return C + g.C;}

        mathFn ret(*this, g, 2);
        return ret;

    }

    mathFn operator-(const mathFn& g) const{

        if(name == "zero"){             return -g;}
        if(g.name == "zero"){           return *this;}    

        if(op == 0  &&  g.op == 0){     return C - g.C;}
        
        mathFn ret(*this, g, 3);
        return ret;

    }

    mathFn operator*(const mathFn& g) const{
        
        if(name == "one"){              return g;}
        if(g.name == "one"){            return *this;}  
        if(name == "zero"){             return 0;}
        if(g.name == "zero"){           return 0;}

        if(op == 0  &&  g.op == 0){     return C * g.C;}
        
        mathFn ret(*this, g, 4);
        return ret;

    }

    mathFn operator/(const mathFn& g) const{
        
        if(g.name == "one"){            return *this;}
        if(name == "zero"){              return 0;}

        if(op == 0  &&  g.op == 0){     return C / g.C;}
        
        mathFn ret(*this, g, 5);
        return ret;

    }

    mathFn operator^(const mathFn& g) const{
        
        if(name == "one"){              return *this;}
        if(name == "zero"){             return *this;}
        if(g.name == "one"){            return *this;}
        if(g.name == "zero"){           return 1;}

        if(op == 0  &&  g.op == 0){     return pow(C, g.C);}
        
        mathFn ret(*this, g, 6);
        return ret;

    }

    mathFn operator ()(const mathFn& f) const{

        if(op == 0){    return *this;}
        if(f.op == 0){  return (*this)(f.C);}

        switch(op){
            
            case 1:     
                if(fn == &id){      return f;}
                                    return retMathFn(*this, f, 7);
            case 2:                 return (*first)(f) + (*second)(f);
            case 3:                 return (*first)(f) - (*second)(f);
            case 4:                 return (*first)(f) * (*second)(f);
            case 5:                 return (*first)(f) / (*second)(f);
            case 6:                 return (*first)(f) ^ (*second)(f);
            case 7:                 return (*first)((*second)(f));

        }

        cout << "err mathFn operator()(const mathFn& f) const";

        return 0;

    }

    mathFn operator-() const{

        if(op == 0){

            return -C;

        }
               
        mathFn ret(-1, *this, 4);
        return ret;

    }

    mathFn operator+=(const mathFn& g){  return *this = *this + g;}
    mathFn operator-=(const mathFn& g){  return *this = *this - g;}
    mathFn operator*=(const mathFn& g){  return *this = *this * g;}
    mathFn operator/=(const mathFn& g){  return *this = *this / g;}

    string print() const{

        switch(op){

            case 0: return to_string(C);
            case 1: return name;
            case 2: return "(" + first->print() + " + " + second->print() + ")";
            case 3: return "(" + first->print() + " - " + second->print() + ")";
            case 4: return "(" + first->print() + " * " + second->print() + ")";
            case 5: return "(" + first->print() + " / " + second->print() + ")";
            case 6: return "(" + first->print() + " ^ " + second->print() + ")";
            case 7: return first->print() + "(" + second->print() + ")";

        }

        return "";

    }   

};

template <class... args>
mathFn retMathFn(args... theArgs){
    
    mathFn ret(theArgs...);
    return ret;
    
}

mathFn operator+(long double C, const mathFn& f){   return mathFn(C) + f;}
mathFn operator-(long double C, const mathFn& f){   return mathFn(C) - f;}
mathFn operator*(long double C, const mathFn& f){   return mathFn(C) * f;}
mathFn operator/(long double C, const mathFn& f){   return mathFn(C) / f;}


// ==========================


long double id(long double x){                          return x;}
long double step(long double x){                        return  x < 0  ?  0  :  1;}
long double sign(long double x){     if(x < 0){          return -1;}
                                    else if(x > 0){     return 1;}
                                    return 0;}


// ==========================


mathFn sine(sin, "sin");
mathFn cosine(cos, "cos");
mathFn tangent(tan, "tan");

mathFn arcsin(asin, "arcsin");
mathFn arccos(acos, "arccos");
mathFn arctan(atan, "arctan");

mathFn exponential(exp, "exp");
mathFn ln(log, "ln");
mathFn squareRoot(sqrt, "sqrt");

mathFn stp(step, "step");
mathFn absolute(abs, "abs");
mathFn sgn(sign, "sgn");


// =======================


class var : public mathFn{
public:

    var() : mathFn(){}

    var(string name) : mathFn(&id, name){}

};


// =======================


#define sin sine
#define cos cosine
#define tan tangent

#define exp exponential

#define sqrt squareRoot

#define abs absolute


// =======================


template <class... T>
void setEvaluator(T&... args){       evaluator = vector <mathFn*>{&args...};}


// =======================







int main(){







    // some examples






    
    // define a variable
    var x("x");







    // define some mathFns
    mathFn f = 3*(x^2) + x;          // Parentheses usually must be used with ^ since it has low precedence
    mathFn g = sin(cos(x));          // The x is optional here; sin(cos) would also work

    mathFn h = -g(f + g / f) - 3;

    g = 2*g;

    mathFn k = x^(x^2);

    cout <<     f(2)                  << endl << 
                g(.5)                 << endl <<
                h(7)                  << endl <<
                h(f+g(h) / f)(3)      << endl;

    cout << g.print() << endl;      // prints "sin(cos(x))"







    // define more variables
    var y("y");
    var z("z");
    var t("t");







    // define a multivariable function
    f = cos(2*x + 2*y) / ln((z^2) * t);

    setEvaluator(x, y, z, t);       // defines which arguments get passed to which variables, and in which order 

    cout << f(-2.5, 3, 4, 65) << endl;


    




    // use of custom functions defined above
    f = sin(stp(x)) + sgn(cos(x));

    cout << f(3);



    



    return 0;
    
}