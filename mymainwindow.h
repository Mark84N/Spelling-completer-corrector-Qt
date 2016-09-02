#ifndef MYMAINWINDOW_H
#define MYMAINWINDOW_H

#include <QMainWindow>
#include "completetextedit.h"

namespace Ui {
class MyMainWindow;
}

class MyMainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MyMainWindow(QWidget *parent = 0);
    ~MyMainWindow();

private slots:
    void on_actionInfo_triggered();

    void on_actionClose_triggered();

    void on_actionNew_triggered();

    void on_actionCopy_Ctrl_C_triggered();

    void on_actionPaste_Ctrl_V_triggered();

    void on_actionCut_Ctrl_X_triggered();

    void on_actionUndo_Ctrl_Z_triggered();

    void on_actionRedo_Ctrl_Y_triggered();

private:

    QString buffer;
    Ui::MyMainWindow *ui;
    CompleteTextEdit *textEdit;
};

#endif // MYMAINWINDOW_H
