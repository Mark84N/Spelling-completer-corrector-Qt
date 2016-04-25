#ifndef SEARCHENGINE_H
#define SEARCHENGINE_H

#include <QCoreApplication>
#include <QString>
#include <QFile>
#include <QDebug>
#include <QStringList>
#include <QHash>
#include <QStringList>
#include <QMessageBox>

//! [0]
namespace SE{

    using Dictionary = QHash <QString, int>;
    /* std::map is needed for default sorting and reverse iterators */
    using DictCandidates = std::map <int, QString>;

    class SearchEngine
    {
    private:

        Dictionary dictionary;
        /* Alterates the original word using 4 methods */
        void edits(const QString& word,
                       QVector<QString>& result);
        /* Checks if altered word is present in dictionary */
        void known(QVector<QString>& result,
                       DictCandidates& candidates);

    public:

        void load(const QString& filename);
        QStringList correct(const QString& word);
    };
}
//! [0]

#endif // SEARCHENGINE_H
