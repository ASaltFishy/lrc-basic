#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <QStringList>
#include "token.h"


class tokenizer
{
public:
    tokenizer();
    QStringList scan(QString &total);
    bool Validation(QString &tok);
private:
    bool isOp(QString ch);
};

#endif // TOKENIZER_H
