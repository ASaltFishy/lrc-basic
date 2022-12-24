#ifndef STATEMENT_H
#define STATEMENT_H

#include "token.h"
#include "exp.h"
#include "evalstate.h"
#include <QObject>

class Statement:public QObject
{
    Q_OBJECT
public:
    Statement(int _line):line(_line){}
    Statement(){}

    virtual STMT_TYPE GetType()=0;
    // 返回值表示控制流的跳转，0表示执行下一条，-1表示结束，其他数字表示跳转至对应行号执行
    virtual int Execute(evalstate &context)=0;

    virtual QString Print()=0;

    int line;
signals:
    void printResult(QString);
};

class RemStmt : public Statement{
public:
    RemStmt(int _line,QString _str):Statement(_line),str(_str){}
    STMT_TYPE GetType() override{return STMT_REM;}
    int Execute(evalstate &context) override;
    QString Print() override;
private:
    QString str;
};

class LetStmt :public Statement{
public:
    LetStmt(int _line,IdentifierExp *var,Exp *exp):Statement(_line),var_(var),exp_(exp){}
    STMT_TYPE GetType() override{return STMT_LET;}
    int Execute(evalstate &context) override;
    QString Print() override;
private:
    IdentifierExp *var_;
    Exp *exp_;
};

class PrintStmt :public Statement{
public:
    PrintStmt(int _line, Exp *exp):Statement(_line),exp_(exp){}
    STMT_TYPE GetType() override{return STMT_PRINT;}
    int Execute(evalstate &context) override;
    QString Print() override;
private:
    Exp *exp_;
};

class InputStmt :public Statement{
public:
    InputStmt(int _line,IdentifierExp *var):Statement(_line),var_(var){}
    STMT_TYPE GetType() override{return STMT_INPUT;}
    int Execute(evalstate &context) override;
    QString Print() override;
    void saveVar(evalstate &context,int number);
private:
    IdentifierExp *var_;
};

class GotoStmt :public Statement{
public:
    GotoStmt(int _line,int dst):Statement(_line),dst_(dst){}
    STMT_TYPE GetType() override{return STMT_GOTO;}
    int Execute(evalstate &context) override;
    QString Print() override;
private:
    int dst_;
};

class IfStmt :public Statement{
public:
    IfStmt(int _line,LOGIC_OPER oper,Exp *left,Exp *right,int then):Statement(_line),oper_(oper),left_cond_(left),right_cond_(right),then_(then){}
    STMT_TYPE GetType() override{return STMT_IF;}
    int Execute(evalstate &context) override;
    QString Print() override;
private:
    LOGIC_OPER oper_;
    Exp *left_cond_;
    Exp *right_cond_;
    int then_;

};

class EndStmt :public Statement{
public:
    EndStmt(int _line):Statement(_line){}
    STMT_TYPE GetType() override{return STMT_END;}
    int Execute(evalstate &context) override;
    QString Print() override;
private:
};

class ErrorStmt: public Statement{
public:
    ErrorStmt(int _line):Statement(_line){}
    STMT_TYPE GetType() override{return STMT_ERROR;}
    int Execute(evalstate &context) override;
    QString Print() override;
};

#endif // STATEMENT_H
