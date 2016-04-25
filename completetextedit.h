#ifndef COMPLETELINEEDIT_H
#define COMPLETELINEEDIT_H

#include <QtWidgets>
#include "searchengine.h"

//! [0]
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

private:
   QListView *listView;
   QStringListModel *model;
   SE::SearchEngine searchWord;
   mutable bool isUpper;

   QString textUnderCursor() const {
       QTextCursor tc = textCursor();
       tc.select(QTextCursor::WordUnderCursor);

       QString text = tc.selectedText();
       this->isUpper = (text[0].isUpper()? true : false);

       return text;
   }

public slots:
   void setCompleter(const QString &text);
   void completeText(const QModelIndex &index);

private slots:
   void textChangedSlot(){
       emit sendSignal(textUnderCursor());
   }

signals:
   void sendSignal(const QString& word);
};
//! [0]

#endif // COMPLETELINEEDIT_H
