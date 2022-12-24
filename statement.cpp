#include "statement.h"
#include <QTextStream>
#include <QDebug>

QString printTree(Exp *root)
{
    // print parse tree
    QVector<QPair<Exp*,int>> output;
    QString text;
    QTextStream s(&text);
    output.push_back(QPair<Exp*,int>(root,1));
    while(!output.empty()){
        Exp *temp = output.front().first;
        int tabNumber = output.front().second;
        output.pop_front();
        for(int i=0;i<tabNumber;i++){
            s<<"    ";
        }
        s<<temp->GetString()<<"\n";
        ExpressionType type = temp->GetType();
        switch(type){
        case CONSTANT:
            break;
        case IDENTIFIER:
            break;
        case COMPOUND:
        {
            CompoundExp *exp = (CompoundExp *)temp;
            output.push_back(QPair<Exp*,int>(exp->getLeft(),tabNumber+1));
            output.push_back(QPair<Exp*,int>(exp->getRight(),tabNumber+1));
            break;
        }
        }
    }
    return text;
}

int RemStmt::Execute(evalstate &context)
{
    Q_UNUSED(context);
    return 0;
}

QString RemStmt::Print()
{
    QString ret;
    QTextStream s(&ret);
    s<<line<<" REM\n    "<<str<<"\n";
    return ret;
}


int LetStmt::Execute(evalstate &context)
{
    context.Enter(var_->GetString(),exp_->eval(context));
    return 0;
}

QString LetStmt::Print()
{
    QString ret;
    QTextStream s(&ret);
    s<<line<<" LET =\n    "<<var_->GetString()<<"\n";
    s<<printTree(exp_);
    return ret;
}

int PrintStmt::Execute(evalstate &context)
{
    emit printResult(QString::number(exp_->eval(context)));
//    qDebug()<<"print statement print "+exp_->GetString();
    return 0;
}

QString PrintStmt::Print()
{
    QString ret;
    QTextStream s(&ret);
    s<<line<<" PRINT\n";
    s<<printTree(exp_);
    return ret;
}

int InputStmt::Execute(evalstate &context)
{
    Q_UNUSED(context);
    return 0;
}

QString InputStmt::Print()
{
    QString ret;
    QTextStream s(&ret);
    s<<line<<" INPUT\n    "<<var_->GetString()<<"\n";
    return ret;
}

void InputStmt::saveVar(evalstate &context,int number)
{
    context.Enter(var_->GetString(),number);
}


int GotoStmt::Execute(evalstate &context)
{
    Q_UNUSED(context);
    return dst_;
}

QString GotoStmt::Print()
{
    QString ret;
    QTextStream s(&ret);
    s<<line<<" GOTO\n    "<<dst_<<"\n";
    return ret;
}

int IfStmt::Execute(evalstate &context)
{
    int left = left_cond_->eval(context);
    int right = right_cond_->eval(context);
    switch(oper_){
    case OP_EQ:
        if(left==right)return then_;
        break;
    case OP_GR:
        if(left>right)return then_;
        break;
    case OP_LE:
        if(left<right)return then_;
        break;
    }
    return 0;
}

// if 较为特殊涉及多个Exp tree，进行单独处理
QString IfStmt::Print()
{
    QString text;
    QTextStream s(&text);
    s<<line<<" IF THEN\n";

    QVector<QPair<Exp*,int>> output;
    ConstantExp *tempN = new ConstantExp(then_);
    IdentifierExp *tempOper = new IdentifierExp(tok.logic2string[oper_]);
    output.push_back(QPair<Exp*,int>(left_cond_,1));
    output.push_back(QPair<Exp*,int>(tempOper,1));
    output.push_back(QPair<Exp*,int>(right_cond_,1));
    output.push_back(QPair<Exp*,int>(tempN,1));
    while(!output.empty()){
        Exp *temp = output.front().first;
        int tabNumber = output.front().second;
        output.pop_front();
        for(int i=0;i<tabNumber;i++){
            s<<"    ";
        }
        s<<temp->GetString()<<"\n";
        ExpressionType type = temp->GetType();
        switch(type){
        case CONSTANT:
            break;
        case IDENTIFIER:
            break;
        case COMPOUND:
        {
            CompoundExp *exp = (CompoundExp *)temp;
            output.push_back(QPair<Exp*,int>(exp->getLeft(),tabNumber+1));
            output.push_back(QPair<Exp*,int>(exp->getRight(),tabNumber+1));
            break;
        }
        }
    }
    delete tempN;
    delete tempOper;
    return text;
}

int EndStmt::Execute(evalstate &context)
{
    Q_UNUSED(context);
    return -1;
}

QString EndStmt::Print()
{
    QString ret;
    QTextStream s(&ret);
    s<<line<<" END\n";
    return ret;
}

int ErrorStmt::Execute(evalstate &context)
{
    Q_UNUSED(context);
    emit printResult("error");
    return 0;
}

QString ErrorStmt::Print()
{
    QString ret;
    QTextStream s(&ret);
    s<<line<<" ERROR\n";
    return ret;

}
