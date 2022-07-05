#include "z3attackprocess.h"
#include <z3++.h>

Z3AttackProcess::Z3AttackProcess(uint32_t h1seed, uint32_t h1goal, uint32_t h2seed, uint32_t h2goal, int len) {
    this->h1seed = h1seed;
    this->h1goal = h1goal;
    this->h2seed = h2seed;
    this->h2goal = h2goal;
    this->len = len;
}

void Z3AttackProcess::z3Start() {
    QString res = "";

    try {

        z3::context c;
        z3::expr unkBV = c.bv_const("unk", 8 * len);
        z3::solver s(c);

        z3::expr hash1 = c.bv_val(h1seed, 32);
        z3::expr hash2 = c.bv_val(h2seed, 32);
        for (int i = 0; i < len; i++) {
            z3::expr ch = z3::zext(unkBV.extract(i * 8 + 7, i * 8), 32);
            hash1 = (hash1 * 33) ^ ch;
            hash2 = (hash2 * 33) ^ ch;
        }

        s.add(hash1 == c.bv_val(h1goal, 32));
        s.add(hash2 == c.bv_val(h2goal, 32));

        for (int i = 0; i < len; i++) {
            s.add(unkBV.extract(i * 8 + 7, i * 8) >= 0x30);
            s.add(unkBV.extract(i * 8 + 7, i * 8) < 0x80);
        }

        z3::check_result result = s.check();

        if (result == z3::unsat) {
            res = "Unsatisfiable";
        } else {
            z3::model m = s.get_model();
            res = m.to_string().c_str();
        }
    } catch(z3::exception e) {
        res = QString("Exception: ") + e.msg();
    }

    emit z3Finished(res + "\n");
}
