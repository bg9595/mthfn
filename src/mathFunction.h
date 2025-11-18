// v4: add compl fn
// v5: change diff()
// v6: streamlined diff(), need debug, reverted
// v7: vecmathfn composition and some streamline ie 0+g
// v8: f(x,y,z) (evaluator)
// v9: += -=
// v10: evaluator in mathFn, linker, f(g,h,j), F(g,h,j), f(x,y,z), F = {}
// v11: vecmathfn op(mathfn) For vecmathfn, compose; then finish up () mathfn,
//      mathfns, vecmathfn for mf and vmf; vecNum; ostreams; static_cast modify; vecmathfn setevaluator
// v12: .h/.cpp

#include <iostream>
#include <cmath>
#include <vector>

#ifndef mathfn_h
#define mathfn_h

namespace mathFunction{

class mathFn;
class vecNum;
class vecMathFn;
class var;
class cppMathFn;

template <class T>
class interm;

class symbolicVars{};

// ===

extern std::vector <mathFn*> evaluator;
extern symbolicVars vars;

long double identity(long double x);
long double stp(long double x);
long double sign(long double x);

extern cppMathFn sin, cos, tan, arcsin, arccos, arctan, exp, ln, sqrt, id, step, abs, sgn;

// ===

template <class... T>
void setEvaluator(T&... args){       evaluator = std::vector <mathFn*>{&args...};}

// ===

mathFn norm(const vecMathFn& G);

// ===

long double fact(int n);

// ===

template <class T>
class interm{
public:

    T* ptr;

    interm(T* ptr){     this->ptr = ptr;}

    T operator=(T&& G){

        std::vector <mathFn*> temp = ptr->evaluator;

        (*ptr) = G;
        
        ptr->evaluator = temp;

        return *ptr;

    }

};

// ===

class mathFn{

public:
    long double (*fn)(long double);
    long double C;
    mathFn* first;
    mathFn* second;
    int op;     // 0C, 1f, 2+, 3-, 4*, 5/, 6^, 7()
    std::string name;
    mathFn* derivative;
    std::vector <mathFn*> evaluator;

    
    mathFn();
    mathFn(int C);
    mathFn(unsigned int C);
    mathFn(double C);
    mathFn(long double C);
    mathFn(long double (*fn)(long double), std::string name);
    mathFn(const mathFn& f, const mathFn& g, int op);
    ~mathFn();

    void deleteOut();
    void copy(const mathFn& f);
    void move(mathFn& f);
    
    mathFn(const mathFn& f);
    mathFn(mathFn&& f);

    mathFn& operator= (const mathFn& f);
    mathFn& operator= (mathFn&& f);

    long double operator()(const long double& x) const;

    template <class... Ts>
    long double operator()(long double x, Ts... args) const{        return (*this)(std::vector <long double>{static_cast<long double>(x), static_cast<long double>(args)...});}
    
    long double operator()(const std::vector <long double>& args) const;
    long double operator()(const std::vector <long double>& args, const std::vector <mathFn*>& evaluator) const;

    mathFn operator+(const mathFn& g) const;
    mathFn operator-(const mathFn& g) const;
    mathFn operator*(const mathFn& g) const;
    mathFn operator/(const mathFn& g) const;
    mathFn operator^(const mathFn& g) const;
    mathFn operator()(const mathFn& f) const;

    mathFn compose(const mathFn& f) const;

    template <class... Ts>
    mathFn operator()(const mathFn& fn, const Ts&... fns) const;

    mathFn operator()(const vecMathFn& fns) const;

    mathFn compose(const vecMathFn& fns) const;
    mathFn compose(const vecMathFn& fns, const std::vector <mathFn*>& evaluator) const;

    interm <mathFn> operator()();

    template <class... Ts>
    interm <mathFn> operator()(var& x, Ts&... moreVars){

        evaluator = std::vector<mathFn*>{&x, &moreVars...};

        return interm(this);

    }

    mathFn operator-() const;

    mathFn operator+=(const mathFn& g);
    mathFn operator-=(const mathFn& g);
    mathFn operator*=(const mathFn& g);
    mathFn operator/=(const mathFn& g);

    std::string print() const;
    std::string printFull() const;

    std::string operator()(const symbolicVars& vars);

};

std::ostream& operator<<(std::ostream& ostr, const mathFn& f);

mathFn operator+(long double C, const mathFn& f);
mathFn operator-(long double C, const mathFn& f);
mathFn operator*(long double C, const mathFn& f);
mathFn operator/(long double C, const mathFn& f);

// ===

class var : public mathFn{
public:

    var(std::string name);

};

// ===

class cppMathFn : public mathFn{
public:

    cppMathFn(long double (*fn)(long double), std::string name);

    using mathFn::operator();
    mathFn operator()(var& x);

};

// ===

class vecNum : public std::vector <long double>{
public:

    std::string print() const;

};

std::ostream& operator<<(std::ostream& ostr, const vecNum& v);

// ===

class vecMathFn : public std::vector <mathFn>{
public:

    std::vector <mathFn*> evaluator;

    vecMathFn();
    vecMathFn(std::vector<mathFn> F);

    template <class... args>
    vecMathFn(const mathFn& first, const args&... theArgs){

        add(first, theArgs...);

    }

    void add();

    template <class... args>
    void add(mathFn first, args... theArgs){

        push_back(first);
        add(theArgs...);

    }

    vecMathFn   operator+(const vecMathFn& G) const;
    vecMathFn   operator-(const vecMathFn& G) const;
    vecMathFn   operator-() const;
    mathFn      operator*(const vecMathFn& G) const;

    template <class T>
    vecNum operator()(T x){

        vecNum ret;

        for(int i = 0; i < size(); i++){        ret.push_back((*this)[i](x));}

        return ret;
        
    }

    template <class... Ts>
    vecNum operator()(long double x, Ts... args){

        vecNum ret;

        std::vector <long double> argVec{static_cast<long double>(x), static_cast<long double>(args)...};

        for(int i = 0; i < size(); i++){        ret.push_back((*this)[i](argVec, evaluator.size() == 0  ?  mathFunction::evaluator  :  evaluator));}

        return ret;

    }

    vecMathFn operator()(const vecMathFn& G);
    vecMathFn operator()(const mathFn& f);

    template <class... Ts>
    vecMathFn operator()(const mathFn& f, const Ts&... fns){

        vecMathFn ret;
        
        vecMathFn args{f, fns...};

        for(int i = 0; i < size(); i++){        ret.push_back((*this)[i].compose(args, evaluator.size() == 0  ?  mathFunction::evaluator  :  evaluator));}

        return ret;

    }

    interm <vecMathFn> operator()();

    template <class... Ts>
    interm <vecMathFn> operator()(var& x, Ts&... moreVars){
        
        evaluator = std::vector<mathFn*>{&x, &moreVars...};

        return interm(this);

    }

    vecMathFn operator % (const vecMathFn& G) const;
    vecMathFn operator * (const mathFn& g) const;
    vecMathFn operator / (const mathFn& g) const;

    mathFn normSq() const;
    mathFn norm() const;

    std::string print() const;
    std::string printFull() const;

    std::string operator()(const symbolicVars& vars);

};

vecMathFn operator *(const mathFn& f, const vecMathFn& G);

vecMathFn operator+=(vecMathFn& F, const vecMathFn& G);
vecMathFn operator-=(vecMathFn& F, const vecMathFn& G);
vecMathFn operator*=(vecMathFn& F, const mathFn& g);
vecMathFn operator/=(vecMathFn& F, const mathFn& g);

std::ostream& operator<<(std::ostream& ostr, const vecMathFn& F);

// ===

template <class... Ts>
mathFn mathFn::operator()(const mathFn& fn, const Ts&... fns) const{    return compose(std::vector<mathFn>{fn, fns...});}

}

#endif











