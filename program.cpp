#include "program.h"
#include <QString>
#include <QStringList>
#include <QDebug>


program::program()
{

}

program::~program(){
}

// 运行程序
void program::RunProgram()
{
    int size,index;
    QVector<Statement *> codes;
    parser.parse(inputs);
    codes = parser.GetCodes();
    updateTreeDisplay(codes);
    index = 0;
    size = codes.size();
    while(index<size){
        Statement *current = codes[index];
        QT_TRY{
            // wait user to input
            if(current->GetType()==STMT_INPUT){
                emit getInput();
                loop.exec();
                bool ok;
                int number = inputVar.toInt(&ok);
                if(!ok)throw "You have to input a number";
                ((InputStmt *)current)->saveVar(context,number);
                index++;
                continue;
            }
            if(current->GetType()==STMT_PRINT || current->GetType()==STMT_ERROR){
                connect(current,SIGNAL(printResult(QString)),this,SLOT(printResult_slot(QString)),Qt::UniqueConnection);
            }
            int go = current->Execute(context);

            // 处理控制流的变化
            if(go==0){
                index++;
            }
            else if(go == -1){
                break;
            }
            else{
                index = findByLine(go,codes);
            }
        }QT_CATCH(QString e){
            qDebug()<<"program catche exception and print";
            printResult_slot(e);
            index++;
        }
    }

}

// 将String类型的input转换为command链表
int program::ToTokens(QString input)
{
    QStringList list = input.split(' ',Qt::SkipEmptyParts);

    // check the validation of input
    bool ok = false;
    int line = list[0].toInt(&ok,10);
    if(!ok){
        throw("invalid line number");
        return -1;
    }
    if(list.size()<2){
        // delete a line
        return DeleteCmd(line);
    }

    QString type;
    if(line>=1 && line <=1000000){
        type = list[1];
    }else{
        throw("invalid line number, have to be from 1 to 1000000");
        return -1;
    }

    list.remove(0,2);
    if(tok.string2type.find(type)==tok.string2type.end()){
        throw("uninplemented statement");
        return -1;
    }
    STMT_TYPE ty = tok.string2type[type];
    return InsertCmd(new command(line,ty,list));

}

// 一句句运行console中直接输入不带行号的命令
int program::RunConsole(QStringList input)
{

    QT_TRY{
    STMT_TYPE type = tok.string2type[input[0]];
    input.pop_front();
    command *cmd = new command(0,type,input);
    Statement *stmt = parser.parseConsole(cmd);
    if(stmt->GetType()==STMT_PRINT || stmt->GetType()==STMT_ERROR){
        connect(stmt,SIGNAL(printResult(QString)),this,SLOT(printResult_slot(QString)),Qt::UniqueConnection);
    }
    if(stmt->GetType()==STMT_INPUT){
        emit getInput();
        loop.exec();
        bool ok;
        int number = inputVar.toInt(&ok);
        if(!ok)throw "You have to input a number";
        ((InputStmt *)stmt)->saveVar(context,number);
        return 0;
    }
    stmt->Execute(context);
    }QT_CATCH(QString e){
        printResult_slot(e);
    }
    return 0;
}


// 返回QString类型用于展示在代码框
QString program::GetInstructionList()
{
    QString ret;
    QTextStream str(&ret);
    for(command *instr: inputs){
        str<<instr->line<<" "<<tok.type2string[instr->type]<<" "<<instr->instr.join(" ")<<"\n";
    }
    return ret;
}

// 更新语法树
void program::updateTreeDisplay(QVector<Statement *> &codes)
{
    QString text;
    for(auto line: codes){
        text += line->Print();
    }
    emit appendParseTree(text);
}

// 将代码输出打印至UI界面
void program::printResult_slot(QString result)
{
    emit appendTextBrowser(result);
}

// 等待用户输入完毕后程序继续运行的响应槽函数
void program::continueToRun_slot(QString str)
{
    loop.exit();
    inputVar = str;
}

// 按行号顺序插入command
int program::InsertCmd(command *cmd)
{
    int lineNum = cmd->line;
    int size = inputs.size();
    for(int i=0;i<size;i++){
        if(inputs[i]->line > lineNum){
            inputs.insert(i,cmd);
            return 1;
        }
        else if(inputs[i]->line == lineNum){
            throw("replicated line number");
            return -1;
        }
    }
    inputs.push_back(cmd);
    return 1;
}

// 按行号删除
int program::DeleteCmd(int lineNum)
{
    for(auto it=inputs.begin();it!=inputs.end();it++){
        if((*it)->line==lineNum){
            inputs.erase(it);
            return 1;
        }
    }
    throw("non-exist line number");
    return -1;
}

// 按行号找到链表中的对应index
int program::findByLine(int lineNumber, QVector<Statement *> &codes)
{
    int i=0;
    for(auto instr:codes){
        if(instr->line==lineNumber)return i;
        i++;
    }
    throw("error: non-exist line number"+QString::number(lineNumber));
    return 0;
}


