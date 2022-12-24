#ifndef TOKEN_H
#define TOKEN_H
#include <QMap>

enum STMT_TYPE{
    STMT_REM,
    STMT_LET,
    STMT_PRINT,
    STMT_INPUT,
    STMT_GOTO,
    STMT_IF,
    STMT_END,
    STMT_ERROR
};

enum OP_OPER{
    OP_PLUS,
    OP_MINUS,
    OP_MUL,
    OP_DIV,
    OP_LBRAC,
    OP_RBRAC,
    OP_EXPON,

};

enum LOGIC_OPER{
    OP_EQ,
    OP_LE,
    OP_GR
};

class Token{
public:
    QMap<STMT_TYPE,QString> type2string;
    QMap<QString,STMT_TYPE> string2type;
    QMap<OP_OPER,QString> op2string;
    QMap<QString,OP_OPER> string2op;
    QMap<LOGIC_OPER,QString> logic2string;
    QMap<QString,LOGIC_OPER> string2logic;

    Token(){
        type2string[STMT_REM] = "REM";
        type2string[STMT_LET] = "LET";
        type2string[STMT_PRINT] = "PRINT";
        type2string[STMT_INPUT] = "INPUT";
        type2string[STMT_GOTO] = "GOTO";
        type2string[STMT_IF] = "IF";
        type2string[STMT_END] = "END";
        type2string[STMT_ERROR] = "Error";


        string2type["REM"] = STMT_REM;
        string2type["LET"] = STMT_LET;
        string2type["PRINT"] = STMT_PRINT;
        string2type["INPUT"] = STMT_INPUT;
        string2type["GOTO"] = STMT_GOTO;
        string2type["IF"] = STMT_IF;
        string2type["END"] = STMT_END;

        op2string[OP_PLUS] = "+";
        op2string[OP_MINUS] = "-";
        op2string[OP_MUL] = "*";
        op2string[OP_DIV] = "/";
        op2string[OP_LBRAC] = "(";
        op2string[OP_RBRAC] = ")";
        op2string[OP_EXPON] = "**";

        string2op["+"] = OP_PLUS;
        string2op["-"] = OP_MINUS;
        string2op["*"] = OP_MUL;
        string2op["/"] = OP_DIV;
        string2op["("] = OP_LBRAC;
        string2op[")"] = OP_RBRAC;
        string2op["**"] = OP_EXPON;

        logic2string[OP_EQ] = "=";
        logic2string[OP_LE] = "<";
        logic2string[OP_GR] = ">";

        string2logic["="] = OP_EQ;
        string2logic["<"] = OP_LE;
        string2logic[">"] = OP_GR;

    }

};



#endif // TOKEN_H
