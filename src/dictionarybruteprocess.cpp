#include "dictionarybruteprocess.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <unordered_map>
#include <vector>
#include "hashing.h"

DictionaryBruteProcess::DictionaryBruteProcess(uint32_t seedMang, uint32_t goalMang, uint32_t seedDem, uint32_t goalDem) {
    wordList = QStringList();

    QFile wordFile(":/res/nsmbwWordList.txt");
    if(!wordFile.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "Error",
                                 "Error reading word list:\n" + wordFile.errorString() + "\nDictionary attack unavailable."
                                 );
    }

    QTextStream in(&wordFile);
    while(!in.atEnd()) {
        QString line = in.readLine();
        line[0] = line[0].toUpper();
        wordList.append(line);
    }
    wordFile.close();

    wordList.append("");
    wordList.removeDuplicates();

    this->seedMang = seedMang;
    this->goalMang = goalMang;
    this->seedDem = seedDem;
    this->goalDem = goalDem;
}

void DictionaryBruteProcess::startBruteforce() {
    // key: hash
    // value: 16 bits per index into hash table, uppermost bit = "first char is lower-/uppercase"
    std::unordered_map<uint32_t, std::vector<uint64_t>> memo;

    QStringList results = QStringList();

    emit updateProgress(0);

    for (int idx1 = 0; idx1 < wordList.size(); idx1++) {
        for (int idx2 = 0; idx2 < wordList.size(); idx2++) {

            uint32_t hash = Hashing::hash(wordList[idx1] + wordList[idx2], seedMang);
            memo[hash].emplace_back((idx1 << 16) | idx2 | (1ULL << 63));

            if (wordList[idx1].length() > 0) {
                QString withFirstLower = wordList[idx1];
                withFirstLower[0] = withFirstLower[0].toLower();
                uint32_t hash2 = Hashing::hash(withFirstLower + wordList[idx2], seedMang);
                memo[hash2].emplace_back((idx1 << 16) | idx2);
            }
        }
        emit updateProgress(40.0 * ((double) idx1 / wordList.size()));
    }
    for (int idx3 = 0; idx3 < wordList.size(); idx3++) {
        for (int idx4 = 0; idx4 < wordList.size(); idx4++) {
            uint32_t hash = Hashing::inverse_hash(goalMang, wordList[idx3] + wordList[idx4]);
            if (memo.find(hash) != memo.end()) {
                // collision!
                for (uint64_t indices : memo[hash]) {
                    int idx1 = indices >> 16;
                    int idx2 = indices & 0xFFFF;
                    bool isUpper = (indices >> 63) != 0;
                    QString firstWord = wordList[idx1];
                    if (!isUpper && firstWord.length() > 0) {
                        firstWord = firstWord[0].toLower() + firstWord.mid(1);
                    }
                    results.append(firstWord + wordList[idx2] + wordList[idx3] + wordList[idx4]);
                }
            }
        }
        emit updateProgress(40.0 + 40.0 * ((double) idx3 / wordList.size()));
    }
    results.removeDuplicates();

    // filter results for demangled value
    for (int i = results.size() - 1; i >= 0; i--) {
        if (Hashing::hash(results[i], seedDem) != goalDem) {
            // false positive, remove it
            results.removeAt(i);
        } else {
            results[i] = results[i];
        }
        emit updateProgress(80.0 + 20.0 * (1 - ((double) i / results.size())));
    }

    emit bruteforceEnded(results);
}
