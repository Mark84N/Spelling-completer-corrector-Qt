/*
    Current class is responsible for working with text source (vast text file) and has powerful mechanism of treating
    misspellings and finding correct words. The work is heavily dependent on Dictionary and Hash table features.

    QHash<string, int> is the main dictionary, where word is a key, frequency of use as per .txt file - is a value. It has
    robust finding capabilities.

    std::map<int, string> is the intermediate dictionary, used during the process of finding matches.
    Frequency of use of the word (as per vast text file) is a key, word  itself is a value. Here are stored the search results that were found in main dictionary (QHash), sorted  by their so-called index of frequency of use - i.e. how many times word has been
    used in the source file. That's why source file has to contain plenty of different-style novels and, possible, articles.

    Algorithm of work:

    - it receives the word that user was trying to type
    - then searches for a full match according to existing dictionary
    - if there's no full match, then word is being altered in 4 possible ways and the process of finding matches is continued for
        every altered word
    - in case of still no matches, previous list of altered words is being altered once again, and the process of finding
        is continued
    - if no matches at all - the original word is returned.

*/

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

namespace SE
{
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
        void known(const QVector<QString> &result,
                       DictCandidates& candidates);

    public:

        void load(const QString& filename);
        QStringList correct(const QString& word);
    };
}

#endif // SEARCHENGINE_H
