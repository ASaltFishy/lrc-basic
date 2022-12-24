#include "evalstate.h"

evalstate::evalstate()
{

}

void evalstate::Enter(QString var, int value)
{
    venv[var] = value;
}

int evalstate::Find(QString var)
{
    if(venv.find(var)==venv.end()){
        throw("undefined var "+var);
    }
    return venv[var];
}
