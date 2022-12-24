#ifndef EVALSTATE_H
#define EVALSTATE_H

#include <QMap>

class evalstate
{
public:
    evalstate();
    void Enter(QString var,int value);
    int Find(QString var);
private:
    QMap<QString,int> venv;
};

#endif // EVALSTATE_H
