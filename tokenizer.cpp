#include "tokenizer.h"

tokenizer::tokenizer()
{
}

// 输入参数不包含空格，返回token组成的QStringList
QStringList tokenizer::scan(QString &total)
{
    QStringList ret;
    int prev=-1,current=0;
    for(auto ch=total.begin();ch!=total.end();ch++){
        if(isOp(*ch)){
            QString temp = total.mid(prev+1,current-prev-1);
            // 将夹在符号之间的操作数读取出来
            if(!temp.isEmpty()){
                ret.push_back(temp);
            }
            // 单独处理幂次情况
            if( (*ch)=='*' && *(ch+1)=='*'){
                ret.push_back("**");
                prev=current+1;
                current+=2;
                ch++;
            // 处理复数情况
            }else if((*ch)=='-' && current-prev==1){
                current++;
            }
            // 处理其他情况
            else{
                ret.push_back((*ch));
                prev = current;
                current++;
            }
        }else{
            current++;
        }
    }
    // 单独处理最后一个token
    if(current-prev>1){
        ret.push_back(total.mid(prev+1,current-prev-1));
    }
    return ret;
}

// aiming identifier
bool tokenizer::Validation(QString &tok)
{
    QChar first = tok.front();
    if(first!='_' && !first.isLetter()){
        throw("invalid identifier "+tok);
        return false;
    }
    for(auto ch : tok){
        if(!(ch.isDigit() || ch.isLetter() || ch == '_')){
            throw("invalid identifier "+tok);
            return false;
        }
    }
    return true;
}

bool tokenizer::isOp(QString ch)
{
    if(ch=='+' || ch=='-' ||ch=='*' ||ch=='/' ||ch=='(' ||ch==')')return true;
    return false;
}

