#ifndef PARSER_H
#define PARSER_H
#include "token.h"
#include "statement.h"
#include "exp.h"
#include "tokenizer.h"


class command{
public:
    int line;
    STMT_TYPE type;
    QStringList instr;
    command(int line_,STMT_TYPE type_,QStringList instr_):line(line_),type(type_),instr(instr_){}
    command(){}
};

class parser
{
public:
    parser();
    QVector<Statement *> &GetCodes(){return codes;}
    // 带有行号的代码解析
    void parse(QVector<command *>&inputs);
    // 不带行号的代码解析
    Statement *parseConsole(command *cmd);
    // parse产生的Statement链表
    QVector<Statement *> codes;

private:
    // 用于识别identifier等是否符合命名规范
    tokenizer scanner;
    Token tok;

    // for expression tree
    QVector<QString> Operators;
    QVector<Exp *> Operands;
    // 为每一行代码生成表达式树
    Exp *genTree(QStringList &list);
    Statement *parseGoto(command *cmd);
    Statement *parsePrint(command *cmd);
    Statement *parseLet(command *cmd);
    Statement *parseIf(command *cmd);
    Statement *parseInput(command *cmd);
    bool isOp(QString &str);
    Exp *ToOperand(QString &str);
    void popExp();

};

#endif // PARSER_H
