#ifndef HASHING_H
#define HASHING_H

#include <QApplication>

namespace Hashing {
    uint hash(QString, uint);
    uint hash(std::string, uint);
    uint inverse_hash(uint, QString);
    uint inverse_hash(uint, std::string);
    uint xorString(QString);
};

#endif // HASHING_H
