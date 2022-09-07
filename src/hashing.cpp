#include "hashing.h"

uint Hashing::hash(QString str, uint seed = 0x1505) {
    uint hash = seed;
    for (int i = 0; i < str.size(); i++) {
        hash = (33 * hash) ^ str[i].toLatin1();
    }

    return hash;
}

uint Hashing::hash(std::string str, uint seed = 0x1505) {
    uint hash = seed;
    for (char c : str) {
        hash = (33 * hash) ^ c;
    }

    return hash;
}

uint Hashing::inverse_hash(uint hash, QString str) {
    for (int i = str.size() - 1; i >= 0; i--) {
        hash = 1041204193 * (str[i].toLatin1() ^ hash);
    }

    return hash;
}

uint Hashing::inverse_hash(uint hash, std::string str) {
    for (int i = str.size() - 1; i >= 0; i--) {
        hash = 1041204193 * (str[i] ^ hash);
    }

    return hash;
}

uint Hashing::xorString(QString str) {
    uint res = 0;
    for (int i = 0; i < str.size(); i++) {
        res ^= str[i].toLatin1();
    }

    return res;
}
