#include "completetextedit.h"
#include "searchengine.h"

CompleteTextEdit::CompleteTextEdit(QWidget *parent)
    : QTextEdit(parent), isUpper(false)
{

    listView = new QListView(this);
    model = new QStringListModel;
    listView->setWindowFlags(Qt::ToolTip);
    listView->setModel(model);

    this->searchingEngine.load(QString(":/big.txt"));

    connect(this, SIGNAL(textChanged()),this, SLOT(textChangedSlot()));
    connect(this, SIGNAL(sendSignal(QString)),this, SLOT(setCompleter(QString)));
    connect(listView, SIGNAL(clicked(const QModelIndex &)),this, SLOT(completeText(const QModelIndex &)));
}

void CompleteTextEdit::focusOutEvent(QFocusEvent *e)
{
    listView->hide();
    QTextEdit::focusOutEvent(e);
}

QString CompleteTextEdit::textUnderCursor() const
{
    QTextCursor tc = textCursor();
    tc.select(QTextCursor::WordUnderCursor);

    QString text = tc.selectedText();
    this->isUpper = (text[0].isUpper()? true : false);

    return text;
}

void CompleteTextEdit::keyPressEvent(QKeyEvent *e)
{
    if (!listView->isHidden())
    {
        int key = e->key();
        int rowCount = listView->model()->rowCount();
        QModelIndex currentIndex = listView->currentIndex();

        /*User is walking through the list of suggestions*/
        if (Qt::Key_Down == key) {
            int row = currentIndex.row() + 1;

            if (row >= rowCount)
                row = 0;

            QModelIndex index = listView->model()->index(row, 0);
            listView->setCurrentIndex(index);
        }
        else if (Qt::Key_Up == key)
        {
            int row = currentIndex.row() - 1;

            if (row < 0)
                row = rowCount - 1;
            QModelIndex index = listView->model()->index(row, 0);
            listView->setCurrentIndex(index);
        }
        /*User clicked a suggestion*/
        else if (Qt::Key_Enter == key || Qt::Key_Return == key ||
                   Qt::Key_Tab == key)
        {
            if (currentIndex.isValid())
                completeText(listView->currentIndex()); // complete word!

            listView->hide();
        }
        else
        {
            /*If none of modifier keys were pressed -
             * continue receiving letters*/
            listView->hide();
            QTextEdit::keyPressEvent(e);
        }
    }
    else
        QTextEdit::keyPressEvent(e);
}

void CompleteTextEdit::setCompleter(const QString &text)
{
    if (text.isEmpty()) {
        listView->hide();
        return;
    }

    model->setStringList(searchingEngine.correct(text));
    listView->setModel(model);

    if (model->rowCount() == 0)
        return;

    /*A position of list with suggestions will rely
    * on the current position of the text cursor.
    */
    QTextCursor cursor = this->textCursor();
    QPoint p(cursor.verticalMovementX(),
             (cursor.blockNumber()+1) * this->fontMetrics().height() + 10);
    int x = mapToGlobal(p).x();
    int y = mapToGlobal(p).y();

    listView->move(x, y);

    int maxVisibleRows = 8;
    listView->setMinimumWidth(0);
    listView->setMaximumWidth(this->width());

    if (model->rowCount() > maxVisibleRows)
    {
        listView->setFixedHeight(maxVisibleRows*
                                 (listView->fontMetrics().height() + 3));
    }
    else
    {
        listView->setFixedHeight(model->rowCount()*
                                 (listView->fontMetrics().height() + 3));
    }
    listView->setResizeMode(QListView::Adjust);
    listView->show();
}

void CompleteTextEdit::completeText(const QModelIndex &index) {

    QTextCursor tc = textCursor();
    tc.movePosition(QTextCursor::StartOfWord, QTextCursor::KeepAnchor);

    QString completion = index.data().toString();

    if(this->isUpper)
    {
        completion = completion[0].toUpper() + completion.mid(1);
        this->isUpper = false;
    }

    tc.insertText(completion);
    setTextCursor(tc);

    listView->hide();
}

void CompleteTextEdit::textChangedSlot()
{
    emit sendSignal(textUnderCursor());
}
