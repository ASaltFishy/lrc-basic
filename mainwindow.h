#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "program.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_cmdLineEdit_editingFinished();
    void on_btnClearCode_clicked();
    void on_btnLoadCode_clicked();
    void on_btnRunCode_clicked();

public slots:
    void appendParseTree_slot(QString text);
    void appendTextBrowser_slot(QString text);
    void getInput_slot();
signals:
    void continueToRun(QString number);

private:
    Ui::MainWindow *ui;
    program *prog;
    program *console;
    int ParseInput(QString input);
    void initPrograms();
};
#endif // MAINWINDOW_H
