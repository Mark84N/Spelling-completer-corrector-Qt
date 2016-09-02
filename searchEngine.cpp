#include <searchengine.h>
using namespace SE;

void SearchEngine::edits(const QString& word, QVector<QString>& result)
{
    //deletions
    for (QString::size_type i = 0; i < word.size(); i++)
        result.push_back(word.mid(0, i) + word.mid(i + 1));
    //transposition
    for (int i = 0; i < word.size()-1; i++)
        result.push_back(word.mid(0, i) + word.mid(i + 1,1) + word.mid(i, 1) + word.mid(i + 2));
    //alterations
    for (char j = 'a'; j <= 'z'; ++j)
    {
        for (int i = 0; i < word.size(); i++)
            result.push_back(word.mid(0, i) + j + word.mid(i + 1));
        //insertion
        for (int i = 0; i < word.size() + 1; i++)
            result.push_back(word.mid(0, i) + j + word.mid(i));
    }
}

void SearchEngine::known(const QVector<QString>& result,
                         DictCandidates& candidates)
{
    Dictionary::const_iterator end = dictionary.end();

    for (auto& str : result)
    {
        //dictionary = QHash<string, int> - word is a key, frequency of use as per .txt file - is a value
        Dictionary::const_iterator foundWord = this->dictionary.find(str);

        // candidates = Dictionary<int, string> - frequency of use is a key, word is a value. We need
        // to sort all results, that were found in main dictionary (QHash)
        if (foundWord != end)
            candidates[foundWord.value()] = foundWord.key();
    }
}

void SearchEngine::load(const QString &filename)
{
    if (!this->dictionary.empty())
        this->dictionary.clear();

    QFile file (filename);

    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::information(0, "Error!", "Unable to open dictionary file. Please relaunch program.");
        return;
    }

    QString data(file.readAll().toLower());

    /* Fill in the dictionary */
    if (!data.isEmpty())
    {
        bool currSymb, lastSymb = false;
        int count = 0;
        QString::ConstIterator start_pos;

        for (auto ch = data.cbegin(); ch != data.cend(); ch++)
        {
            currSymb = ch->isLetter();

            if (lastSymb != currSymb)
            {
                if (lastSymb)
                {
                    this->dictionary[QString(start_pos, count)]++;
                    count = 0;
                }
                start_pos = ch;
            }

            if(currSymb)
                count++;

            lastSymb = currSymb;
        }
    }
    file.close();
}

QStringList SearchEngine::correct(const QString& word)
{
    QStringList resultsList;
    resultsList << word;

    /* looking for a full match */
    if (dictionary.find(word) != dictionary.end())
        return resultsList;

    /* FIRST level edit: modifying word and searching for a match */
    QVector<QString> firstLevelEditedResults;
    DictCandidates candidates;

    edits(word, firstLevelEditedResults);
    known(firstLevelEditedResults, candidates);

    if (!candidates.empty())
    {
        auto it = candidates.rbegin();

        for (int i = 0; i < 8 && it != candidates.rend(); i++, it++)
            resultsList << it->second;

        return resultsList;
    }
    /* SECOND level edit: modifying previous results
     * and searching for every possible match (may be time consuming operation!) */
    for (int i = 0; i < firstLevelEditedResults.size(); i++)
    {
        QVector<QString> secondLevelEditedResults;;

        edits(firstLevelEditedResults[i], secondLevelEditedResults);
        known(secondLevelEditedResults, candidates);
    }
    if (!candidates.empty())
    {
        auto it = candidates.rbegin();

        for (int i = 0; i < 8 && it != candidates.rend(); i++, it++)
            resultsList << it->second;

        return resultsList;
    }
    /* If still no match - just return original word */
    return resultsList;
}
