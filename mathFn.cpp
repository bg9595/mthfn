#include "mathFunction.h"

using namespace std;

namespace mathFunction{

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

mathFn::mathFn(long double (*fn)(long double), string name){

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

long double mathFn::operator()(const vector <long double>& args) const{
                return    evaluator.size() == 0  ?  (*this)(args, mathFunction::evaluator)  :  (*this)(args, evaluator);}

long double mathFn::operator()(const vector <long double>& args, const vector <mathFn*>& evaluator) const{

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

    cout << "err mathFn operator()(const mathFn& f) const";

    return 0;

}

mathFn mathFn::operator()(const vecMathFn& fns) const{    cout << "ineval" << endl << endl;
    
    mathFn ret =    fns.size() == 1  ?  compose(fns[0])  :  compose(fns);
    

    ret.evaluator = fns.evaluator;
        //cout << ret.evaluator[0] << ":";
    return ret;
    
}

mathFn mathFn::compose(const vecMathFn& fns) const{

    return    evaluator.size() == 0  ?  compose(fns, mathFunction::evaluator)  :  compose(fns, evaluator);

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

interm <mathFn> mathFn::operator()(){

    evaluator.clear();

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

string mathFn::print() const{

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

string mathFn::printFull() const{

    string ret = "(";

    for(int i = 0; i < evaluator.size() - 1; i++){

        ret += evaluator[i]->name + ", ";

    }

    ret += evaluator.back()->name + ") = ";

    ret += print();

    return ret;

}

string mathFn::operator()(const symbolicVars& vars){        return printFull();}





// ===






ostream& operator<<(ostream& ostr, const mathFn& f){       ostr << f.print();      return ostr;}

mathFn operator+(long double C, const mathFn& f){   return mathFn(C) + f;}
mathFn operator-(long double C, const mathFn& f){   return mathFn(C) - f;}
mathFn operator*(long double C, const mathFn& f){   return mathFn(C) * f;}
mathFn operator/(long double C, const mathFn& f){   return mathFn(C) / f;}

}