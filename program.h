#ifndef PROGRAM_H
#define PROGRAM_H

#include <QString>
#include <QStringList>
#include <QObject>
#include <QEventLoop>
#include "token.h"
#include "parser.h"


class program:public QObject
{
    Q_OBJECT
public:
    program();
    ~program();
    void RunProgram();
    int ToTokens(QString input);
    int RunConsole(QStringList input);
    QString GetInstructionList();
    void updateTreeDisplay(QVector<Statement *> &codes);

signals:
    void appendParseTree(QString);
    void appendTextBrowser(QString);
    void getInput();
public slots:
    void printResult_slot(QString);
    void continueToRun_slot(QString str);

private:
    parser parser;
    Token tok;
    evalstate context;
    QEventLoop loop;
    QString inputVar;

    // for display
    QVector<command *> inputs;
    int InsertCmd(command *cmd);
    int DeleteCmd(int lineNum);
    int findByLine(int lineNumber,QVector<Statement *> &codes);

};


#endif // PROGRAM_H
