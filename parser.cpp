#include "parser.h"
#include <QMap>

parser::parser()
{

}
// 带有行号的代码解析
void parser::parse(QVector<command *> &inputs)
{
    codes.clear();
    for(command *cmd : inputs){
        QT_TRY{
            switch(cmd->type){
            case STMT_REM:
                codes.push_back(new RemStmt(cmd->line,cmd->instr.join(" ")));
                break;
            case STMT_GOTO:
            {
                codes.push_back(parseGoto(cmd));
                break;
            }
            case STMT_IF:
            {
                codes.push_back(parseIf(cmd));
                break;
            }
            case STMT_INPUT:
//                codes.push_back(new InputStmt(cmd->line,new IdentifierExp(cmd->instr[0])));
                codes.push_back(parseInput(cmd));
                break;
            case STMT_LET:
            {
                codes.push_back(parseLet(cmd));
                break;
            }
            case STMT_PRINT:
            {
                codes.push_back(parsePrint(cmd));
                break;
            }
            case STMT_END:
                codes.push_back(new EndStmt(cmd->line));
                break;
            default:
                break;
            }
        }QT_CATCH(QString e){
            codes.push_back(new ErrorStmt(cmd->line));
        }
    }
}
// 不带行号的代码解析
Statement *parser::parseConsole(command *cmd)
{
    switch(cmd->type){
    case STMT_LET:
    {
        return parseLet(cmd);
        break;
    }
    case STMT_INPUT:
    {
        return parseInput(cmd);
        break;
    }
    case STMT_PRINT:
    {
        return parsePrint(cmd);
        break;
    }
    default:
        return nullptr;
    }
}

// 解析GOTO语句
Statement *parser::parseGoto(command *cmd)
{
    bool ok = false;
    int dst = cmd->instr[0].toInt(&ok,10);
    if(ok){
        return new GotoStmt(cmd->line,dst);
    }else{
        return new ErrorStmt(cmd->line);
    }
}
// 解析PRINT语句
Statement *parser::parsePrint(command *cmd)
{
    QString para = cmd->instr.join("");
    QStringList tokenList = scanner.scan(para);
    Exp *exp = genTree(tokenList);
    return new PrintStmt(cmd->line,exp);
}
// 解析LET语句
Statement *parser::parseLet(command *cmd)
{
    QString instr = cmd->instr.join("");
    int pos = instr.indexOf("=");
    if(pos != -1){
        QString var = instr.mid(0,pos);
        if(scanner.Validation(var)){
            QString para = instr.mid(pos+1);
            QStringList tokenList = scanner.scan(para);
            Exp *exp = genTree(tokenList);
            return new LetStmt(cmd->line,new IdentifierExp(var),exp);
        }
    }
    return new ErrorStmt(cmd->line);
}
// 解析IF语句
Statement *parser::parseIf(command *cmd)
{
    QString instr = cmd->instr.join("");
    int ThenPos = instr.indexOf("THEN");
    if(ThenPos!=-1){
        bool ok = false;
        QString dststr = instr.mid(ThenPos+4);
        int dst = dststr.toInt(&ok,10);
        if(ok){
            QString condition = instr.mid(0,ThenPos);
            int logicPos;
            QString logicOper;
            if((logicPos = condition.indexOf("="))!=-1){
                logicOper = "=";
            }
            else if((logicPos = condition.indexOf("<"))!=-1){
                logicOper = "<";
            }
            else if((logicPos = condition.indexOf(">"))!=-1){
                logicOper = ">";
            }
            else{
                return new ErrorStmt(cmd->line);
            }
            QString l = condition.mid(0,logicPos);
            QString r = condition.mid(logicPos+1);
            QStringList leftList = scanner.scan(l);
            Exp *lhs = genTree(leftList);
            QStringList rightList = scanner.scan(r);
            Exp *rhs = genTree(rightList);
            return new IfStmt(cmd->line,tok.string2logic[logicOper],lhs,rhs,dst);
        }
    }
    return new ErrorStmt(cmd->line);
}
// 解析INPUT语句
Statement *parser::parseInput(command *cmd)
{
    QString var = cmd->instr[0];
    if(scanner.Validation(var)){
        return new InputStmt(cmd->line,new IdentifierExp(cmd->instr[0]));
    }
    return nullptr;
}

bool parser::isOp(QString &str)
{
    if(str=='+' || str=='-' ||str=='*' ||str=='/' ||str=='(' ||str==')' || str=="**")return true;
    return false;
}

Exp *parser::ToOperand(QString &str)
{
    bool ok;
    int number = str.toInt(&ok,10);
    if(ok){
        return new ConstantExp(number);
    }else{
        if(scanner.Validation(str)){
            return new IdentifierExp(str);
        }else{
            // TODO
            return nullptr;
        }
    }
}

void parser::popExp()
{
    OP_OPER oper = tok.string2op[Operators.front()];
    Operators.pop_front();
    Exp *rhs = Operands.front();
    Operands.pop_front();
    Exp *lhs = Operands.front();
    Operands.pop_front();
    Operands.push_front(new CompoundExp(oper,lhs,rhs));
}


// 遇到运算数，直接压到Operands栈中
// 遇到左括号，直接压入Operators栈中
// 遇到右括号,将栈顶的运算符弹出，并和exps中的两个运算数组成一颗小树，然后把小树的根节点放入exps栈中
// 当前扫描的运算符优先级大于ops栈栈顶运算符的优先级，直接入ops栈
// 当前扫描的运算符优先级小于等于ops栈顶运算符的优先级,将栈顶运算符弹出，
// 并和exps中的两个运算数组成一颗小树，然后把小树的根节点放入exps栈中
// 直到该运算符优先级大于栈顶运算符优先级或栈为空，然后入栈
// generate expression tree
Exp *parser::genTree(QStringList &list)
{
    // 单独处理只有一个常数或标识符的情况
    if(list.size()==1){
        return ToOperand(list[0]);
    }

    // 处理复合表达式的情况
    QMap<QString,int> precedence;
    precedence["("] = 0;
    precedence[")"] = -1;
    precedence["+"]=precedence["-"]=1;
    precedence["*"]=precedence["/"]=2;
    precedence["**"]=3;

    Operators.clear();
    Operands.clear();
    int pos=0,tokenNum = list.size();
    while(pos!=tokenNum){
        // the token is an operand
        if(!isOp(list[pos])){
            Operands.push_front(ToOperand(list[pos]));
            pos++;
            continue;
        }

        // the token is an operator
        // 当操作符栈为空/下一个符号是左括号/下一个符号有更高的优先级————直接入栈
        if(Operators.empty() || list[pos]=="(" || precedence[Operators.front()]<precedence[list[pos]]){
            Operators.push_front(list[pos]);
            pos++;
            continue;
        }
        // 下一个元素为右括号，出栈直至遇到左括号
        if(list[pos]==")"){
            while(Operators.front()!="("){
                popExp();
            }
            Operators.pop_front();
            pos++;
            continue;
        }
        // 当栈顶符号有更高优先级————栈顶元素出栈
        if(precedence[Operators.front()]>precedence[list[pos]]){
            popExp();
            continue;
        }
        // 优先级相同,求幂符号（继续入栈）与其他符号(出栈)运算结合性不同，分开处理
        if(list[pos]=="**"){
            Operators.push_front(list[pos]);
            pos++;
        }
        else{
            popExp();
        }
    }
    while(Operands.size()!=1){
        popExp();
    }
    Exp *ret = Operands.front();
    Operands.pop_front();
    return ret;
}

