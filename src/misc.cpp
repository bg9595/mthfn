#include "mathFunction.h"

using namespace std;

namespace mathFunction{

vector <mathFn*> evaluator;
symbolicVars vars;

long double fact(int n){

    int ret = 1;

    for( ; n > 1; n--){ ret *= n;}
    
    return ret;

}

mathFn norm(const vecMathFn& G){ return G.norm();}

}
