#ifndef EXP_H
#define EXP_H

#include <QString>
#include "token.h"
#include "evalstate.h"

enum ExpressionType { CONSTANT, IDENTIFIER, COMPOUND };
static Token tok;

class Exp
{
public:
    Exp(){}
    virtual ~Exp()=default;
    // 返回表达式类型
    virtual ExpressionType GetType()=0;
    // 计算表达式的值
    virtual int eval(evalstate & context) = 0;
    // 获取表达式在语法树中的显示符号
    virtual QString GetString() =0;


};

class ConstantExp :public Exp{
public:
    ConstantExp(int num):num_(num){}
    ~ConstantExp(){}
    ExpressionType GetType() override {return CONSTANT;}
    int eval(evalstate & context) override;
    QString GetString() override{return QString::number(num_);}

private:
    int num_;
};

class IdentifierExp :public Exp{
public:
    IdentifierExp(QString var):var_(var){}
    ~IdentifierExp(){}
    ExpressionType GetType() override {return IDENTIFIER;}
    int eval(evalstate & context) override;
    QString GetString() override{return var_;}

private:
    QString var_;
};

// 括号代表的层次结构在生成嵌套表达式时已经处理掉了
class CompoundExp :public Exp{
public:
    CompoundExp(OP_OPER oper,Exp *left,Exp *right):oper_(oper),left_(left),right_(right){}
    ~CompoundExp(){delete left_;delete right_;}
    ExpressionType GetType() override {return COMPOUND;}
    int eval(evalstate & context) override;
    QString GetString() override{return tok.op2string[oper_];}
    Exp* getLeft(){return left_;}
    Exp *getRight(){return right_;}

private:
    OP_OPER oper_;
    Exp *left_;
    Exp *right_;
};

#endif // EXP_H
