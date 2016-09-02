#ifndef COMPLETELINEEDIT_H
#define COMPLETELINEEDIT_H

#include <QtWidgets>
#include "searchengine.h"

namespace Ui {
class CompleteTextEdit;
}

class CompleteTextEdit : public QTextEdit
{
    Q_OBJECT

public:

    CompleteTextEdit(QWidget *parent = 0);

protected:

    virtual void keyPressEvent(QKeyEvent *e);
    virtual void focusOutEvent(QFocusEvent *e);

signals:

    void sendSignal(const QString& word);


public slots:

    void setCompleter(const QString &text);
    void completeText(const QModelIndex &index);

private slots:

    void textChangedSlot();

private:

    QListView *listView;
    QStringListModel *model;
    SE::SearchEngine searchingEngine;
    mutable bool isUpper;

    QString textUnderCursor() const;
};

#endif // COMPLETELINEEDIT_H
