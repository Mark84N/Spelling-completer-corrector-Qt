#include <searchengine.h>
using namespace SE;

//! [0]
void SearchEngine::edits(const QString& word,
                                  QVector<QString>& result){
    //deletions
    for (QString::size_type i = 0; i < word.size(); i++)
        result.push_back(word.mid(0, i) + word.mid(i + 1));
    //transposition
    for (int i = 0; i < word.size()-1; i++)
        result.push_back(word.mid(0, i) + word.mid(i + 1,1) + word.mid(i, 1) + word.mid(i + 2));
    //alterations
    for (char j = 'a'; j <= 'z'; ++j) {
        for (int i = 0; i < word.size(); i++)
            result.push_back(word.mid(0, i) + j + word.mid(i + 1));
        //insertion
        for (int i = 0; i < word.size() + 1; i++)
            result.push_back(word.mid(0, i) + j + word.mid(i));
    }
}
//! [0]

//! [1]
void SearchEngine::known(QVector<QString>& result,
                             DictCandidates& candidates){

    Dictionary::const_iterator end = dictionary.end();

    for (auto& str : result){

        Dictionary::const_iterator value = this->dictionary.find(str);

        if (value != end) {
            candidates[value.value()] = value.key();
        }
    }
}
//! [1]

//! [2]
void SearchEngine::load(const QString &filename) {

    if (!this->dictionary.empty()){
        this->dictionary.clear();
    }
    QFile file (filename);

    if (!file.open(QFile::ReadOnly | QFile::Text)){
        QMessageBox::information(0,
                                 "Error!", "Unable to open file. Please relaunch program.");
        return;
    }

    QString data(file.readAll().toLower());
    /* Fill in the dictionary */
    if (!data.isEmpty()){

        bool currSymb, lastSymb = false;
        QString::ConstIterator start_pos;
        int count = 0;

        for (auto ch = data.cbegin(); ch != data.cend(); ch++){
            currSymb = ch->isLetter();
            if (lastSymb != currSymb){
                if (lastSymb){
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
//! [2]

//! [3]
QStringList SearchEngine::correct(const QString& word) {

    QStringList strList;
    strList << word;
    /* FIRST level: looking for a full match */
    if (dictionary.find(word) != dictionary.end()) {
        return strList;
    }
    /* SECOND level: modifying word and searching for match */
    QVector<QString> temp_results;
    DictCandidates candidates;

    edits(word, temp_results);
    known(temp_results, candidates);

    if (!candidates.empty()) {
        auto it = candidates.rbegin();

        for (int i = 0; i < 8 && it != candidates.rend(); i++, it++){
            strList << it->second;
        }

        return strList;
    }
    /* THIRD level: modifying previous results
     * and searching for every possible match (may be expensive!) */
    for (int i = 0; i < temp_results.size(); i++) {
        QVector<QString> sub_results;;

        edits(temp_results[i], sub_results);
        known(sub_results, candidates);
    }
    if (!candidates.empty()) {
        auto it = candidates.rbegin();

        for (int i = 0; i < 8 && it != candidates.rend(); i++, it++){
            strList << it->second;
        }
        return strList;
    }
    /* If still no match - just return original word */
    return strList;
}
//! [3]
