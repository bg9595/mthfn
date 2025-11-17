#include <iostream>
#include <cmath>
#include <vector>

using namespace std;








class mathFn;
long double id(long double);

vector <mathFn*> evaluator;

template <class... args>
mathFn retMathFn(args... theArgs);

class vecMathFn;








template <class T>
class linker{
public:

    T* linkee;
    
    linker(T* linkee){

        this->linkee = linkee;

    }

    T operator=(T&& G){

        vector <mathFn*> temp = linkee->evaluator;

        (*linkee) = G;
        
        linkee->evaluator = temp;

        return *linkee;

    }

};








class symbolicVars{};
symbolicVars vars;








class var;








class mathFn{
public:

    long double (*fn)(long double);

    long double C;

    mathFn* first;
    mathFn* second;

    int op;     // 0C, 1f, 2+, 3-, 4*, 5/, 6^, 7()

    string name;

    mathFn* derivative;

    vector <mathFn*> evaluator;

    
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
        evaluator = f.evaluator;
        
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
        evaluator = f.evaluator;

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

    long double operator()(const long double& x) const{

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

        return 0;

    }

    template <class... Ts>
    long double operator()(long double x, Ts... args) const{        return (*this)(vector <long double>{static_cast<long double>(x), static_cast<long double>(args)...});}

    long double operator()(const vector <long double>& args) const{       return    evaluator.size() == 0  ?  (*this)(args, ::evaluator)  :  (*this)(args, evaluator);}

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

    mathFn operator()(const mathFn& f) const{

        mathFn ret = compose(f);

        ret.evaluator = f.evaluator;

        return ret;

    }

    mathFn compose(const mathFn& f) const{

        if(op == 0){    return *this;}
        if(f.op == 0){  return (*this)(f.C);}

        switch(op){
            
            case 1:     
                if(fn == &id){      return f;}
                                    return retMathFn(*this, f, 7);
            case 2:                 return first->compose(f) + second->compose(f);
            case 3:                 return first->compose(f) - second->compose(f);
            case 4:                 return first->compose(f) * second->compose(f);
            case 5:                 return first->compose(f) / second->compose(f);
            case 6:                 return first->compose(f) ^ second->compose(f);
            case 7:                 return first->compose(second->compose(f));

        }

        cout << "err mathFn operator()(const mathFn& f) const";

        return 0;

    }

    template <class... Ts>
    mathFn operator()(const mathFn& fn, const Ts&... fns) const;

    mathFn operator()(const vecMathFn& fns) const;

    mathFn compose(const vecMathFn& fns) const;

    mathFn compose(const vecMathFn& fns, const vector <mathFn*>& evaluator) const;

    linker <mathFn> operator()(){

        evaluator.clear();

        return linker(this);
        
    }

    template <class... Ts>
    linker <mathFn> operator()(var& x, Ts&... moreVars){ 

        evaluator = vector<mathFn*>{&x, &moreVars...};

        return linker(this);

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
  
        auto addPar= []<class... Ts>(mathFn* mfn, Ts... nums){

            vector <int> numsVec{nums...};

            for(int i = 0; i < numsVec.size(); i++){        if(mfn->op == numsVec[i]){       return "(" + mfn->print() + ")";}}

            return mfn->print();

        };

        switch(op){

            case 0: return to_string(C);
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

    string printFull() const{ 

        string ret = "(";

        for(int i = 0; i < evaluator.size() - 1; i++){

            ret += evaluator[i]->name + ", "; 

        }

        ret += evaluator.back()->name + ") = ";

        ret += print();

        return ret;
    
    }

    string operator()(const symbolicVars& vars){        return printFull();}

    mathFn diff();
    mathFn diff(int n);

};

ostream& operator<<(ostream& ostr, const mathFn& f){       ostr << f.print();      return ostr;}

template <class... args>
mathFn retMathFn(args... theArgs){
    
    mathFn ret(theArgs...);
    return ret;
    
}

mathFn operator+(long double C, const mathFn& f){   return mathFn(C) + f;}
mathFn operator-(long double C, const mathFn& f){   return mathFn(C) - f;}
mathFn operator*(long double C, const mathFn& f){   return mathFn(C) * f;}
mathFn operator/(long double C, const mathFn& f){   return mathFn(C) / f;}








class cppMathFn : public mathFn{
public:

    cppMathFn(long double (*fn)(long double), string name) : mathFn(fn, name){}

    using mathFn::operator();
    mathFn operator()(var& x){      return ((mathFn)(*this))((mathFn&)(x));}

};



long double id(long double x){                          return x;}
long double step(long double x){                        return  x < 0  ?  0  :  1;}
long double sgn(long double x){     if(x < 0){          return -1;}
                                    else if(x > 0){     return 1;}
                                    return 0;}



cppMathFn sine(sin, "sin");
cppMathFn cosine(cos, "cos");
cppMathFn tangent(tan, "tan");

cppMathFn arcsin(asin, "arcsin");
cppMathFn arccos(acos, "arccos");
cppMathFn arctan(atan, "arctan");

cppMathFn exponential(exp, "exp");
cppMathFn logarithm(log, "ln");
cppMathFn squareRoot(sqrt, "sqrt");

cppMathFn identity(id, "id");
cppMathFn stepFn(step, "step");
cppMathFn absoluteValue(abs, "abs");
cppMathFn sign(sgn, "sgn");








class var : public mathFn{
public:

    var(string name) : mathFn(&id, name){}

};

var x("x");
var y("y");
var z("z");
var t("t");
var s("s");
var u("u");
var v("v");
var w("w");








template <class... T>
void setEvaluator(T&... args){       evaluator = vector <mathFn*>{&args...};}








#define sin sine
#define cos cosine
#define tan tangent

#define exp exponential
#define ln logarithm
#define sqrt squareRoot

#define id identity
#define step stepFn
#define abs absoluteValue
#define sgn sign

#define pi 3.141592654








class vecNum : public vector <long double>{
public:

    string print() const{

        string ret = "(";

        for(int i = 0; i < size() - 1; i++){

            ret += to_string((*this)[i]) + ", ";

        }

        ret += to_string((*this).back()) + ")";

        return ret;

    }

};

ostream& operator<<(ostream& ostr, const vecNum& v){       ostr << v.print();      return ostr;}








class vecMathFn : public vector <mathFn>{
public:

    vector <mathFn*> evaluator;

    vecMathFn(){}

    vecMathFn(vector<mathFn> F) : vector<mathFn>(F){}

    template <class... args>
    vecMathFn(const mathFn& first, const args&... theArgs){

        add(first, theArgs...);

    }

    void add(){}

    template <class... args>
    void add(mathFn first, args... theArgs){

        push_back(first);
        add(theArgs...);

    }

    vecMathFn operator+(const vecMathFn& G) const{

        vecMathFn ret;

        for(int i = 0; i < size(); i++){
         
            ret.push_back((*this)[i] + G[i]);
            
        }

        return ret;

    }

    vecMathFn operator-(const vecMathFn& G) const{

        vecMathFn ret;

        for(int i = 0; i < size(); i++){

            ret.push_back((*this)[i] - G[i]);

        }

        return ret;
        
    }

    vecMathFn operator-() const{

        vecMathFn ret;

        for(int i = 0; i < size(); i++){

            ret.push_back(-(*this)[i]);
        
        }

        return ret;
    
    }

    mathFn operator * (const vecMathFn& G) const{

        mathFn ret = 0;

        for(int i = 0; i < size(); i++){

            ret += ((*this)[i] * G[i]);
        }

        return ret;

    }

    template <class T>
    vecNum operator()(T x){

        vecNum ret;

        for(int i = 0; i < size(); i++){        ret.push_back((*this)[i](x));}

        return ret;
        
    }

    template <class... Ts>
    vecNum operator()(long double x, Ts... args){

        vecNum ret;

        vector <long double> argVec{static_cast<long double>(x), static_cast<long double>(args)...};

        for(int i = 0; i < size(); i++){        ret.push_back((*this)[i](argVec, evaluator.size() == 0  ?  ::evaluator  :  evaluator));}

        return ret;

    }

    vecMathFn operator()(const vecMathFn& G){

        vecMathFn ret;

        if(G.size() == 1){      for(int i = 0; i < size(); i++){        ret.push_back((*this)[i].compose(G[0]));}}
        else{                   for(int i = 0; i < size(); i++){        ret.push_back((*this)[i].compose(G, evaluator.size() == 0  ?  ::evaluator  :  evaluator));}}

        ret.evaluator = G.evaluator;

        return ret;

    }

    vecMathFn operator()(const mathFn& f){

        vecMathFn ret;

        for(int i = 0; i < size(); i++){        ret.push_back((*this)[i].compose(f));}

        ret.evaluator = f.evaluator;

        return ret;

    }

    template <class... Ts>
    vecMathFn operator()(const mathFn& f, const Ts&... fns){

        vecMathFn ret;
        
        vecMathFn args{f, fns...};

        for(int i = 0; i < size(); i++){        ret.push_back((*this)[i].compose(args, evaluator.size() == 0  ?  ::evaluator  :  evaluator));}

        return ret;

    }

    linker <vecMathFn> operator()(){
        
        evaluator.clear();

        return linker(this);

    }

    template <class... Ts>
    linker <vecMathFn> operator()(var& x, Ts&... moreVars){
        
        evaluator = vector<mathFn*>{&x, &moreVars...};

        return linker(this);

    }

    vecMathFn operator % (const vecMathFn& G) const{

        vecMathFn ret;

        ret.push_back((*this)[1] * G[2] - (*this)[2] * G[1]);
        ret.push_back((*this)[2] * G[0] - (*this)[0] * G[2]);
        ret.push_back((*this)[0] * G[1] - (*this)[1] * G[0]);

        return ret;

    }

    vecMathFn operator * (const mathFn& g) const{

        vecMathFn ret;

        for(int i = 0; i < size(); i++){

            ret.push_back((*this)[i] * g);

        }

        return ret;

    }

    vecMathFn operator / (const mathFn& g) const{

        vecMathFn ret;

        for(int i = 0; i < size(); i++){

            ret.push_back((*this)[i] / g);

        }

        return ret;

    }

    mathFn normSq() const{

        return (*this)*(*this);

    }

    mathFn norm() const{

        return sqrt(normSq());

    }


    string print() const{

        string ret = "(";

        for(int i = 0; i < size() - 1; i++){

            ret += (*this)[i].print() + ", ";

        }

        ret += (*this).back().print() + ")";

        return ret;

    }

    string printFull() const{

        string ret = "(";
    
        for(int i = 0; i < evaluator.size() - 1; i++){
            
            ret += evaluator[i]->name + ", ";

        }

        ret += evaluator.back()->name + ") = ";

        ret += print();

        return ret;

    }

    string operator()(const symbolicVars& vars){            return printFull();}

};

vecMathFn operator *(const mathFn& f, const vecMathFn& G){  return G * f;}

vecMathFn operator+=(vecMathFn& F, const vecMathFn& G){     return F = F + G;}
vecMathFn operator-=(vecMathFn& F, const vecMathFn& G){     return F = F - G;}
vecMathFn operator*=(vecMathFn& F, const mathFn& g){        return F = F * g;}
vecMathFn operator/=(vecMathFn& F, const mathFn& g){        return F = F / g;}

ostream& operator<<(ostream& ostr, const vecMathFn& F){     ostr << F.print();
                                                            return ostr;}








template <class... Ts>
mathFn mathFn::operator()(const mathFn& fn, const Ts&... fns) const{    return compose(vector<mathFn>{fn, fns...});}

mathFn mathFn::operator()(const vecMathFn& fns) const{  
    
    mathFn ret =    fns.size() == 1  ?  compose(fns[0])  :  compose(fns);
    

    ret.evaluator = fns.evaluator;

    return ret;
    
}

mathFn mathFn::compose(const vecMathFn& fns) const{

    return    evaluator.size() == 0  ?  compose(fns, ::evaluator)  :  compose(fns, evaluator);

}

mathFn mathFn::compose(const vecMathFn& fns, const vector <mathFn*>& evaluator) const{

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

    cout << "long double operator()(...) err";

    return 0;
}








mathFn norm(const vecMathFn& G){ return G.norm();}









int main(){



}















