#ifndef Z3ATTACKPROCESS_H
#define Z3ATTACKPROCESS_H

#include <QObject>

class Z3AttackProcess : public QObject {
    Q_OBJECT
public:
    explicit Z3AttackProcess(uint32_t h1seed, uint32_t h1goal, uint32_t h2seed, uint32_t h2goal, int len);

public slots:
    void z3Start();

signals:
    void z3Finished(QString);

private:
    uint32_t h1seed, h1goal, h2seed, h2goal;
    int len;

};

#endif // Z3ATTACKPROCESS_H
