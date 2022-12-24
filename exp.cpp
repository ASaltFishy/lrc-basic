#include "exp.h"
#include <QTextStream>

int ConstantExp::eval(evalstate & context){
    Q_UNUSED(context);
    return num_;
}


int IdentifierExp::eval(evalstate & context){
    return context.Find(var_);
}


int CompoundExp::eval(evalstate & context){
    int ret;
    switch(oper_){
    case OP_PLUS:
        ret = left_->eval(context) + right_->eval(context);
        break;
    case OP_MINUS:
        ret = left_->eval(context) - right_->eval(context);
        break;
    case OP_MUL:
        ret = left_->eval(context) * right_->eval(context);
        break;
    case OP_DIV:
        ret = left_->eval(context) / right_->eval(context);
        break;
    case OP_EXPON:
        ret = pow(left_->eval(context),right_->eval(context));
        break;
    default:
        throw("invalid operator");
        break;
    }
    return ret;
}


