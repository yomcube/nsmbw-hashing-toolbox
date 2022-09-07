#include "dictionarybruteprocess.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <unordered_map>
#include <vector>
#include "hashing.h"
#include "nvidia_demangler.h"

DictionaryBruteProcess::DictionaryBruteProcess(uint32_t seedMang, uint32_t goalMang, bool useDem, uint32_t goalDem, QString prefix, QString suffix) {
    QFile wordFile(":/res/nsmbwWordList.txt");
    if(!wordFile.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "Error",
                                 "Error reading word list:\n" + wordFile.errorString() + "\nDictionary attack unavailable."
                                 );
    }

    QTextStream in(&wordFile);
    while(!in.atEnd()) {
        wordList.emplace_back(in.readLine().toStdString());
    }
    wordFile.close();

    wordList.emplace_back("");

    this->prefix = prefix;
    this->suffix = suffix;
    this->seedMang = seedMang;
    this->goalMang = goalMang;
    this->useDem = useDem;
    this->goalDem = goalDem;
}

void DictionaryBruteProcess::startBruteforce() {
    // key: hash

    std::map<uint32_t, std::vector<uint32_t>> memo;

    QStringList results = QStringList();

    emit updateProgress(0);

    for (int idx1 = 0; idx1 < wordList.size(); idx1++) {
        for (int idx2 = 0; idx2 < wordList.size(); idx2++) {

            uint32_t hash = Hashing::hash(wordList[idx2], Hashing::hash(wordList[idx1], seedMang));

            memo[hash].emplace_back((idx1 << 16) | idx2);
        }
        emit updateProgress(40.0 * ((double) idx1 / wordList.size()));
    }
    for (int idx3 = 0; idx3 < wordList.size(); idx3++) {
        for (int idx4 = 0; idx4 < wordList.size(); idx4++) {
            uint32_t hash = Hashing::inverse_hash(Hashing::inverse_hash(goalMang, wordList[idx4]), wordList[idx3]);
            if (memo.count(hash) == 0) continue;

            // collision!
            for (uint32_t val : memo[hash]) {
                int idx1 = val >> 16;
                int idx2 = val & 0xFFFF;
                results.append(QString::fromStdString(prefix.toStdString() + wordList[idx1] + wordList[idx2] + wordList[idx3] + wordList[idx4] + suffix.toStdString()));
            }
        }
        emit updateProgress(40.0 + 40.0 * ((double) idx3 / wordList.size()));
    }
    results.removeDuplicates();

    if (useDem) {
        // filter results for demangled value
        for (int i = results.size() - 1; i >= 0; i--) {
            if (Hashing::hash(QString::fromStdString(nvidia_demangler::nvidia_demangle(results[i].toStdString())), 0x1505) != goalDem) {
                // false positive, remove it
                results.removeAt(i);
            }
            //results[i] = results[i] + " -> " + QString::fromStdString(nvidia_demangler::nvidia_demangle(results[i].toStdString()));
        }
    }


    emit bruteforceEnded(results);
}
