#ifndef HASHING_H
#define HASHING_H

#include <QApplication>

namespace Hashing {
    uint hash(QString, uint);
    uint inverse_hash(uint, QString);
    uint xorString(QString);
};

#endif // HASHING_H
