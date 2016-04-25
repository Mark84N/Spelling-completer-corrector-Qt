#include "mymainwindow.h"
#include "ui_mymainwindow.h"

MyMainWindow::MyMainWindow(QWidget *parent) :
    QMainWindow(parent),buffer(""),
    ui(new Ui::MyMainWindow){

    ui->setupUi(this);

    this->textEdit = new CompleteTextEdit;
    this->setCentralWidget(textEdit);

    this->setWindowTitle("Spelling corrector");
}

MyMainWindow::~MyMainWindow(){
    delete ui;
}

void MyMainWindow::on_actionInfo_triggered(){
    QString about("Dmitriy Dieiev\nSpell checking application was created"
                  " to simplify the process of typing a text. Program is able"
                  " to correct misspellings and suggests what user wanted to type."
                  " Made for educational purposes.\n"
    "dieiev.dima@gmail.com");
    QMessageBox::information(this, "About", about);
}

void MyMainWindow::on_actionClose_triggered(){
    this->close();
}

void MyMainWindow::on_actionNew_triggered(){
    this->textEdit->clear();
}

void MyMainWindow::on_actionCopy_Ctrl_C_triggered(){
    this->textEdit->copy();
}

void MyMainWindow::on_actionPaste_Ctrl_V_triggered(){
    this->textEdit->paste();
}

void MyMainWindow::on_actionCut_Ctrl_X_triggered(){
    this->textEdit->cut();
}

void MyMainWindow::on_actionUndo_Ctrl_Z_triggered(){
    this->textEdit->undo();
}

void MyMainWindow::on_actionRedo_Ctrl_Y_triggered(){
    this->textEdit->redo();
}
