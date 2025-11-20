#include <iostream>
#include <cmath>
#include <vector>

#ifndef mathFunction_h
#define mathFunction_h

namespace mathFunction{

class mathFn;
class vecNum;
class vecMathFn;
class var;
class cppMathFn;

template <class T>
class interm;

class symbolicVars;

// ===

extern std::vector <mathFn*> evaluator;
extern symbolicVars vars;

long double identity(long double);
long double stp(long double);
long double sign(long double);

extern cppMathFn sin, cos, tan, arcsin, arccos, arctan, exp, ln, sqrt, id, step, abs, sgn, cosh, sinh, tanh, arcsinh, arccosh, arcctanh;

// ===

template <class... T>
void setEvaluator(T&... args){       evaluator = std::vector <mathFn*>{&args...};}

// ===

mathFn norm(const vecMathFn& G);

// ===

long double fact(int n);






// ====================






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
    long double operator()(long double x, Ts... args) const;
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
    interm <mathFn> operator()(var& x, Ts&... moreVars);

    mathFn operator-() const;

    mathFn operator+=(const mathFn& g);
    mathFn operator-=(const mathFn& g);
    mathFn operator*=(const mathFn& g);
    mathFn operator/=(const mathFn& g);

    std::string print() const;
    std::string printFull() const;

    std::string operator()(const symbolicVars& vars);

};

mathFn operator+(long double C, const mathFn& f);
mathFn operator-(long double C, const mathFn& f);
mathFn operator*(long double C, const mathFn& f);
mathFn operator/(long double C, const mathFn& f);

std::ostream& operator<<(std::ostream& ostr, const mathFn& f);

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
    vecMathFn(const mathFn& first, const args&... theArgs);

    void add();
template <class... args>
    void add(mathFn first, args... theArgs);

    vecMathFn   operator+(const vecMathFn& G) const;
    vecMathFn   operator-(const vecMathFn& G) const;
    vecMathFn   operator-() const;
    mathFn      operator*(const vecMathFn& G) const;

template <class T>
    vecNum operator()(T x);
template <class... Ts>
    vecNum operator()(long double x, Ts... args);
    vecMathFn operator()(const vecMathFn& G);
    vecMathFn operator()(const mathFn& f);
template <class... Ts>
    vecMathFn operator()(const mathFn& f, const Ts&... fns);

    interm <vecMathFn> operator()();
template <class... Ts>
    interm <vecMathFn> operator()(var& x, Ts&... moreVars);

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

template <class T>
class interm{
public:

    T* ptr;

    interm(T* ptr);
    T operator=(T&& G);

};

// ====================

class symbolicVars{};






// ====================






mathFn::mathFn(){

    op = 0;
    C = 1;

    fn = NULL;
    first = second = NULL;   

}

mathFn::mathFn(int C) : mathFn((long double) C){

    if(C == 0){     name = "zero";}
    if(C == 1){     name = "one";}

}

mathFn::mathFn(unsigned int C) : mathFn((long double) C){}
mathFn::mathFn(double C) : mathFn((long double) C){}

mathFn::mathFn(long double C){

    op = 0;
    this->C = C;

    fn = NULL;
    first = second = NULL;

}

mathFn::mathFn(long double (*fn)(long double), std::string name){

    op = 1;
    C = 1;
    this->fn = fn;

    first = second = NULL;

    this->name = name;
    
}

mathFn::mathFn(const mathFn& f, const mathFn& g, int op){

    this->op = op;

    C = 1;

    fn = NULL;
    
    first = new mathFn; 
    second = new mathFn;

    *first = f;
    *second = g;

}

mathFn::~mathFn(){      deleteOut();}

void mathFn::deleteOut(){

    if(first){

        delete first;
        first = NULL;

    }
    if(second){

        delete second;
        second = NULL;

    }

}

void mathFn::copy(const mathFn& f){

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

void mathFn::move(mathFn& f){

    fn = f.fn;
    C = f.C;
    first = f.first;
    second = f.second;
    op = f.op;
    name = f.name;

    f.first = f.second = NULL;

}

mathFn::mathFn(const mathFn& f){        copy(f);}
mathFn::mathFn(mathFn&& f){             move(f);}

mathFn& mathFn::operator= (const mathFn& f){
    
    deleteOut();
    copy(f);

    return *this;             

}

mathFn& mathFn::operator= (mathFn&& f){

    deleteOut();
    move(f);      

    return *this;

}   

long double mathFn::operator()(const long double& x) const{

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

    // cout << "long double operator()(long double) err";

    return 0;

}

template <class... Ts>
long double mathFn::operator()(long double x, Ts... args) const{
                return (*this)(std::vector <long double>{static_cast<long double>(x), static_cast<long double>(args)...});}

long double mathFn::operator()(const std::vector <long double>& args) const{
                return    evaluator.size() == 0  ?  (*this)(args, mathFunction::evaluator)  :  (*this)(args, evaluator);}

long double mathFn::operator()(const std::vector <long double>& args, const std::vector <mathFn*>& evaluator) const{

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

    std::cout << "long double operator()(...) err";

    return 0;
}

mathFn mathFn::operator+(const mathFn& g) const{

    if(name == "zero"){             return g;}
    if(g.name == "zero"){           return *this;} 

    if(op == 0  &&  g.op == 0){     return C + g.C;}

    mathFn ret(*this, g, 2);
    return ret;

}

mathFn mathFn::operator-(const mathFn& g) const{

    if(name == "zero"){             return -g;}
    if(g.name == "zero"){           return *this;}    

    if(op == 0  &&  g.op == 0){     return C - g.C;}
    
    mathFn ret(*this, g, 3);
    return ret;

}

mathFn mathFn::operator*(const mathFn& g) const{
    
    if(name == "one"){              return g;}
    if(g.name == "one"){            return *this;}  
    if(name == "zero"){             return 0;}
    if(g.name == "zero"){           return 0;}

    if(op == 0  &&  g.op == 0){     return C * g.C;}
    
    mathFn ret(*this, g, 4);
    return ret;

}

mathFn mathFn::operator/(const mathFn& g) const{
    
    if(g.name == "one"){            return *this;}
    if(name == "zero"){              return 0;}

    if(op == 0  &&  g.op == 0){     return C / g.C;}
    
    mathFn ret(*this, g, 5);
    return ret;

}

mathFn mathFn::operator^(const mathFn& g) const{
    
    if(name == "one"){              return *this;}
    if(name == "zero"){             return *this;}
    if(g.name == "one"){            return *this;}
    if(g.name == "zero"){           return 1;}

    if(op == 0  &&  g.op == 0){     return pow(C, g.C);}
    
    mathFn ret(*this, g, 6);
    return ret;

}

mathFn mathFn::operator()(const mathFn& f) const{

    mathFn ret = compose(f);

    ret.evaluator = f.evaluator;

    return ret;

}

mathFn mathFn::compose(const mathFn& f) const{

    if(op == 0){    return *this;}
    if(f.op == 0){  return (*this)(f.C);}

    switch(op){
        
        case 1:     
            if(fn == &identity){      return f;}
                                return mathFn(*this, f, 7);
        case 2:                 return first->compose(f) + second->compose(f);
        case 3:                 return first->compose(f) - second->compose(f);
        case 4:                 return first->compose(f) * second->compose(f);
        case 5:                 return first->compose(f) / second->compose(f);
        case 6:                 return first->compose(f) ^ second->compose(f);
        case 7:                 return first->compose(second->compose(f));

    }

    std::cout << "err mathFn operator()(const mathFn& f) const";

    return 0;

}

template <class... Ts>
mathFn mathFn::operator()(const mathFn& fn, const Ts&... fns) const{    return compose(std::vector<mathFn>{fn, fns...});}

mathFn mathFn::operator()(const vecMathFn& fns) const{    
    
    mathFn ret =    fns.size() == 1  ?  compose(fns[0])  :  compose(fns);
    

    ret.evaluator = fns.evaluator;

    return ret;
    
}

mathFn mathFn::compose(const vecMathFn& fns) const{

    return    evaluator.size() == 0  ?  compose(fns, mathFunction::evaluator)  :  compose(fns, evaluator);

}

mathFn mathFn::compose(const vecMathFn& fns, const std::vector <mathFn*>& evaluator) const{

    switch(op){

        case 0: return C;
        case 1: {       for(int i = 0; i < fns.size(); i++){    if(name == evaluator[i]->name){        return fns[i];}}}
        case 2: return first->compose(fns, evaluator) + second->compose(fns, evaluator);
        case 3: return first->compose(fns, evaluator) - second->compose(fns, evaluator);
        case 4: return first->compose(fns, evaluator) * second->compose(fns, evaluator);
        case 5: return first->compose(fns, evaluator) / second->compose(fns, evaluator);
        case 6: return first->compose(fns, evaluator) ^ second->compose(fns, evaluator);
        case 7: return first->compose(second->compose(fns, evaluator));

    }

    std::cout << "long double operator()(...) err";

    return 0;
}

interm <mathFn> mathFn::operator()(){

    evaluator.clear();

    return interm(this);
    
}

template <class... Ts>
interm <mathFn> mathFn::operator()(var& x, Ts&... moreVars){

    evaluator = std::vector<mathFn*>{&x, &moreVars...};

    return interm(this);

}

mathFn mathFn::operator-() const{

    if(op == 0){

        return -C;

    }
    
    
    mathFn ret(-1, *this, 4);
    return ret;

}

mathFn mathFn::operator+=(const mathFn& g){  return *this = *this + g;}
mathFn mathFn::operator-=(const mathFn& g){  return *this = *this - g;}
mathFn mathFn::operator*=(const mathFn& g){  return *this = *this * g;}
mathFn mathFn::operator/=(const mathFn& g){  return *this = *this / g;}

std::string mathFn::print() const{

    auto addPar= []<class... Ts>(mathFn* mfn, Ts... nums){

        std::vector <int> numsVec{nums...};

        for(int i = 0; i < numsVec.size(); i++){        if(mfn->op == numsVec[i]){       return "(" + mfn->print() + ")";}}

        return mfn->print();

    };

    switch(op){

        case 0: return std::to_string(C);
        case 1: return name;
        case 2: return first->print() + " + " + second->print();
        case 3: return first->print() + " - " + second->print();
        case 4: return addPar(first, 2, 3) + " * " + addPar(second, 2, 3);
        case 5: return addPar(first, 2, 3) + " / " + addPar(second, 2, 3);
        case 6: return addPar(first, 2, 3, 4, 5, 6) + "^" + addPar(second, 2, 3, 4, 5, 6);
        case 7: return first->print() + "(" + second->print() + ")";

    }

    return "";

}   

std::string mathFn::printFull() const{

    std::string ret = "(";

    for(int i = 0; i < evaluator.size() - 1; i++){

        ret += evaluator[i]->name + ", ";

    }

    ret += evaluator.back()->name + ") = ";

    ret += print();

    return ret;

}

std::string mathFn::operator()(const symbolicVars& vars){        return printFull();}

// ===

std::ostream& operator<<(std::ostream& ostr, const mathFn& f){       ostr << f.print();      return ostr;}

mathFn operator+(long double C, const mathFn& f){   return mathFn(C) + f;}
mathFn operator-(long double C, const mathFn& f){   return mathFn(C) - f;}
mathFn operator*(long double C, const mathFn& f){   return mathFn(C) * f;}
mathFn operator/(long double C, const mathFn& f){   return mathFn(C) / f;}






// ====================







vecMathFn::vecMathFn(){}
vecMathFn::vecMathFn(std::vector<mathFn> F) : std::vector<mathFn>(F){}

template <class... args>
vecMathFn::vecMathFn(const mathFn& first, const args&... theArgs){

    add(first, theArgs...);

}

void vecMathFn::add(){}

template <class... args>
void vecMathFn::add(mathFn first, args... theArgs){

    push_back(first);
    add(theArgs...);

}

vecMathFn vecMathFn::operator+(const vecMathFn& G) const{

    vecMathFn ret;

    for(int i = 0; i < size(); i++){
        
        ret.push_back((*this)[i] + G[i]);
        
    }

    return ret;

}

vecMathFn vecMathFn::operator-(const vecMathFn& G) const{

    vecMathFn ret;

    for(int i = 0; i < size(); i++){

        ret.push_back((*this)[i] - G[i]);

    }

    return ret;
    
}

vecMathFn vecMathFn::operator-() const{

    vecMathFn ret;

    for(int i = 0; i < size(); i++){

        ret.push_back(-(*this)[i]);
    
    }

    return ret;

}

mathFn vecMathFn::operator*(const vecMathFn& G) const{

    mathFn ret = 0;

    for(int i = 0; i < size(); i++){

        ret += ((*this)[i] * G[i]);
    }

    return ret;

}

template <class T>
vecNum vecMathFn::operator()(T x){

    vecNum ret;

    for(int i = 0; i < size(); i++){        ret.push_back((*this)[i](x));}

    return ret;
    
}

template <class... Ts>
vecNum vecMathFn::operator()(long double x, Ts... args){

    vecNum ret;

    std::vector <long double> argVec{static_cast<long double>(x), static_cast<long double>(args)...};

    for(int i = 0; i < size(); i++){        ret.push_back((*this)[i](argVec, evaluator.size() == 0  ?  mathFunction::evaluator  :  evaluator));}

    return ret;

}

vecMathFn vecMathFn::operator()(const vecMathFn& G){

    vecMathFn ret;

    if(G.size() == 1){      for(int i = 0; i < size(); i++){        ret.push_back((*this)[i].compose(G[0]));}}
    else{                   for(int i = 0; i < size(); i++){        ret.push_back((*this)[i].compose(G, evaluator.size() == 0  ?  mathFunction::evaluator  :  evaluator));}}

    ret.evaluator = G.evaluator;

    return ret;

}

vecMathFn vecMathFn::operator()(const mathFn& f){

    vecMathFn ret;

    for(int i = 0; i < size(); i++){        ret.push_back((*this)[i].compose(f));}

    ret.evaluator = f.evaluator;

    return ret;

}

template <class... Ts>
vecMathFn vecMathFn::operator()(const mathFn& f, const Ts&... fns){

    vecMathFn ret;
    
    vecMathFn args{f, fns...};

    for(int i = 0; i < size(); i++){        ret.push_back((*this)[i].compose(args, evaluator.size() == 0  ?  mathFunction::evaluator  :  evaluator));}

    return ret;

}

interm <vecMathFn> vecMathFn::operator()(){
    
    evaluator.clear();

    return interm(this);

}

template <class... Ts>
interm <vecMathFn> vecMathFn::operator()(var& x, Ts&... moreVars){
    
    evaluator = std::vector<mathFn*>{&x, &moreVars...};

    return interm(this);

}

vecMathFn vecMathFn::operator%(const vecMathFn& G) const{

    vecMathFn ret;

    ret.push_back((*this)[1] * G[2] - (*this)[2] * G[1]);
    ret.push_back((*this)[2] * G[0] - (*this)[0] * G[2]);
    ret.push_back((*this)[0] * G[1] - (*this)[1] * G[0]);

    return ret;

}

vecMathFn vecMathFn::operator*(const mathFn& g) const{

    vecMathFn ret;

    for(int i = 0; i < size(); i++){

        ret.push_back((*this)[i] * g);

    }

    return ret;

}

vecMathFn vecMathFn::operator/(const mathFn& g) const{

    vecMathFn ret;

    for(int i = 0; i < size(); i++){

        ret.push_back((*this)[i] / g);

    }

    return ret;

}

mathFn vecMathFn::normSq() const{

    return (*this)*(*this);

}

mathFn vecMathFn::norm() const{

    return sqrt(normSq());

}


std::string vecMathFn::print() const{

    std::string ret = "(";

    for(int i = 0; i < size() - 1; i++){

        ret += (*this)[i].print() + ", ";

    }

    ret += (*this).back().print() + ")";

    return ret;

}

std::string vecMathFn::printFull() const{

    std::string ret = "(";

    for(int i = 0; i < evaluator.size() - 1; i++){
        
        ret += evaluator[i]->name + ", ";

    }

    ret += evaluator.back()->name + ") = ";

    ret += print();

    return ret;

}

std::string vecMathFn::operator()(const symbolicVars& vars){        return printFull();}






// ===================






vecMathFn operator*(const mathFn& f, const vecMathFn& G){       return G * f;}

vecMathFn operator+=(vecMathFn& F, const vecMathFn& G){         return F = F + G;}
vecMathFn operator-=(vecMathFn& F, const vecMathFn& G){         return F = F - G;}
vecMathFn operator*=(vecMathFn& F, const mathFn& g){            return F = F * g;}
vecMathFn operator/=(vecMathFn& F, const mathFn& g){            return F = F / g;}

std::ostream& operator<<(std::ostream& ostr, const vecMathFn& F){         ostr << F.print();      return ostr;}






// ====================






std::string vecNum::print() const{

    std::string ret = "(";

    for(int i = 0; i < size() - 1; i++){

        ret += std::to_string((*this)[i]) + ", ";

    }

    ret += std::to_string((*this).back()) + ")";

    return ret;

}

std::ostream& operator<<(std::ostream& ostr, const vecNum& v){       ostr << v.print();      return ostr;}






// ===================






cppMathFn::cppMathFn(long double (*fn)(long double), std::string name) : mathFn(fn, name){}

mathFn cppMathFn::operator()(var& x){      return ((mathFn)(*this))((mathFn&)(x));}

long double identity(long double x){                          return x;}
long double stp(long double x){                        return  x < 0  ?  0  :  1;}
long double sign(long double x){     if(x < 0){          return -1;}
                                    else if(x > 0){     return 1;}
                                    return 0;}

cppMathFn
    sin(std::sin, "sin"),           cos(std::cos, "cos"),           tan(std::tan, "tan"),
    arcsin(std::asin, "arcsin"),    arccos(std::acos, "arccos"),    arctan(std::atan, "arctan"),
    
    exp(std::exp, "exp"),           ln(std::log, "ln"),             sqrt(std::sqrt, "sqrt"),
    
    id(identity, "id"),             step(stp, "step"),              abs(std::abs, "abs"),           sgn(sign, "sgn"),

    sinh(std::sinh, "sinh"),        cosh(std::cosh, "cosh"),        tanh(std::tanh, "tanh"),
    arcsinh(std::asinh, "arcsinh"), arccosh(std::acosh, "arccosh"), arctanh(std::atanh, "arctanh");







// ===================






var::var(std::string name) : mathFn(&identity, name){}






// ====================






template <class T>
interm<T>::interm(T* ptr){     this->ptr = ptr;}

template <class T>
T interm<T>::operator=(T&& G){

    std::vector <mathFn*> temp = ptr->evaluator;

    (*ptr) = G;
    
    ptr->evaluator = temp;

    return *ptr;

}






// ====================






std::vector <mathFn*> evaluator;
symbolicVars vars;

long double fact(int n){

    int ret = 1;

    for( ; n > 1; n--){ ret *= n;}
    
    return ret;

}

mathFn norm(const vecMathFn& G){ return G.norm();}

}

#endif
