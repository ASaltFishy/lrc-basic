#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QString>
#include <QTextStream>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    console = new program();
    initPrograms();
    connect(console,SIGNAL(appendTextBrowser(QString)),this,SLOT(appendTextBrowser_slot(QString)),Qt::UniqueConnection);
    connect(console,SIGNAL(getInput()),this,SLOT(getInput_slot()),Qt::UniqueConnection);
    connect(this,SIGNAL(continueToRun(QString)),console,SLOT(continueToRun_slot(QString)),Qt::UniqueConnection);
}

MainWindow::~MainWindow()
{
    delete prog;
    delete console;
    delete ui;
}

// 只负责窗口的修改，其他逻辑交给ParseInput及更深层来处理
void MainWindow::on_cmdLineEdit_editingFinished()
{
    QString instruction = ui->cmdLineEdit->text();
    ui->cmdLineEdit->setText("");
    QT_TRY{
        if(ParseInput(instruction)==1)
            ui->CodeDisplay->setText(prog->GetInstructionList());
    }QT_CATCH(const char *e){
        QString str(e);
        QMessageBox::warning(this,"语法错误",str);
    }
}

void MainWindow::on_btnClearCode_clicked()
{
    // TODO 清空窗口的同时也要清空所有状态,prog地址变了记得重新连接信号与槽函数
    ui->CodeDisplay->setText("");
    ui->textBrowser->setText("");
    ui->treeDisplay->setText("");
    delete prog;
    initPrograms();
}


void MainWindow::on_btnLoadCode_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open");
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Failed to open file: " + file.errorString());
        return;
    }
    // 清除prog程序状态
    on_btnClearCode_clicked();
    QTextStream out(&file);
    QString line;
    while((line = out.readLine())!=nullptr){
        QT_TRY{
            ParseInput(line);
        }QT_CATCH(const char *e){
            QString str(e);
            QMessageBox::warning(this,"语法错误",str);
        }
    }
    ui->CodeDisplay->setText(prog->GetInstructionList());
    file.close();
    on_btnRunCode_clicked();
}



void MainWindow::on_btnRunCode_clicked()
{
    ui->textBrowser->setText("");
    ui->treeDisplay->setText("");
    prog->RunProgram();
}

void MainWindow::appendParseTree_slot(QString text)
{
    ui->treeDisplay->append(text);
}

void MainWindow::appendTextBrowser_slot(QString text)
{
    ui->textBrowser->append(text);
}

void MainWindow::getInput_slot()
{
    ui->cmdLineEdit->setText(" ?");
}

int MainWindow::ParseInput(QString input)
{
    if(input.isEmpty())return 0;
    if(input.startsWith(" ?")){
        QStringList list = input.split(' ',Qt::SkipEmptyParts);
        QString t = list.join("");
        int pos = t.indexOf("?");
        QString numStr = t.mid(pos+1);
//        int number = numStr.toInt(&ok);
        emit continueToRun(numStr);
        return 0;
    }
    QStringList list = input.split(' ',Qt::SkipEmptyParts);
    if(list[0]=="LOAD"){
        on_btnLoadCode_clicked();
        return 0;
    }
    if(list[0]=="RUN"){
        on_btnRunCode_clicked();
        return 0;
    }
    if(list[0]=="LIST"){
        return 0;
    }
    if(list[0]=="CLEAR"){
        on_btnClearCode_clicked();
        return 0;
    }
    if(list[0]=="HELP"){
        QMessageBox::information(this,"help","This is a Basic Interpreter implemented by lrc");
        return 0;
    }
    if(list[0]=="QUIT"){
        this->close();
        return 0;
    }
    if(list[0]=="LET" || list[0]=="PRINT" || list[0]=="INPUT"){
        console->RunConsole(list);
        return 0;
    }
    return prog->ToTokens(input);
}

void MainWindow::initPrograms()
{
    prog = new program();
    connect(prog,SIGNAL(appendParseTree(QString)),this,SLOT(appendParseTree_slot(QString)),Qt::UniqueConnection);
    connect(prog,SIGNAL(appendTextBrowser(QString)),this,SLOT(appendTextBrowser_slot(QString)),Qt::UniqueConnection);
    connect(prog,SIGNAL(getInput()),this,SLOT(getInput_slot()),Qt::UniqueConnection);
    connect(this,SIGNAL(continueToRun(QString)),prog,SLOT(continueToRun_slot(QString)),Qt::UniqueConnection);
}

