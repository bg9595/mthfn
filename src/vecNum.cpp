#include "mathFunction.h"

using namespace std;

namespace mathFunction{
    
string vecNum::print() const{

    string ret = "(";

    for(int i = 0; i < size() - 1; i++){

        ret += to_string((*this)[i]) + ", ";

    }

    ret += to_string((*this).back()) + ")";

    return ret;

}

// ====================

ostream& operator<<(ostream& ostr, const vecNum& v){       ostr << v.print();      return ostr;}

}
