#include "mathFunction.h"

using namespace std;

namespace mathFunction{

vecMathFn::vecMathFn(){}
vecMathFn::vecMathFn(vector<mathFn> F) : vector<mathFn>(F){}

void vecMathFn::add(){}

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

interm <vecMathFn> vecMathFn::operator()(){
    
    evaluator.clear();

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


string vecMathFn::print() const{

    string ret = "(";

    for(int i = 0; i < size() - 1; i++){

        ret += (*this)[i].print() + ", ";

    }

    ret += (*this).back().print() + ")";

    return ret;

}

string vecMathFn::printFull() const{

    string ret = "(";

    for(int i = 0; i < evaluator.size() - 1; i++){
        
        ret += evaluator[i]->name + ", ";

    }

    ret += evaluator.back()->name + ") = ";

    ret += print();

    return ret;

}

string vecMathFn::operator()(const symbolicVars& vars){        return printFull();}






// ===================






vecMathFn operator*(const mathFn& f, const vecMathFn& G){       return G * f;}

vecMathFn operator+=(vecMathFn& F, const vecMathFn& G){         return F = F + G;}
vecMathFn operator-=(vecMathFn& F, const vecMathFn& G){         return F = F - G;}
vecMathFn operator*=(vecMathFn& F, const mathFn& g){            return F = F * g;}
vecMathFn operator/=(vecMathFn& F, const mathFn& g){            return F = F / g;}

ostream& operator<<(ostream& ostr, const vecMathFn& F){         ostr << F.print();      return ostr;}

}

