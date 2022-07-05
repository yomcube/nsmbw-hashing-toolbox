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
        std::vector<z3::expr> unkBits;
        char **buf = (char **) malloc(len * sizeof(char *));
        for (int i = 0; i < len; i++) {
            buf[i] = (char *) malloc(10 * sizeof(char));
            sprintf(buf[i], "unk%d", i);
            unkBits.push_back(c.bv_const(buf[i], 8));
        }
        z3::solver s(c);

        z3::expr hash1 = c.bv_val(h1seed, 32);
        z3::expr hash2 = c.bv_val(h2seed, 32);
        for (int i = 0; i < len; i++) {
            z3::expr ch = z3::zext(unkBits[i], 32 - 8);
            hash1 = (hash1 * 33) ^ ch;
            hash2 = (hash2 * 33) ^ ch;
        }

        s.add(hash1 == c.bv_val(h1goal, 32));
        s.add(hash2 == c.bv_val(h2goal, 32));

        for (int i = 0; i < len; i++) {
            s.add(unkBits[i] >= 0x30);
            s.add(unkBits[i] < 0x7F);
        }

        z3::check_result result = s.check();

        if (result == z3::unsat) {
            res = "Unsatisfiable";
        } else {
            QStringList solutions;
            for (int i = 0; i < 64; i++) {
                z3::model m = s.get_model();
                QString solStr = "";
                z3::expr_vector andAll(c);
                for (int j = m.size() - 1; j >= 0; j--) {
                    z3::func_decl v = m[j];
                    solStr += QString((char) m.get_const_interp(v).as_uint64());

                    // ensure that we don't get this one again
                    andAll.push_back(unkBits[j] == m.get_const_interp(v));
                }
                solutions.append(solStr);
                s.add(!z3::mk_and(andAll));
                result = s.check();
                if (result == z3::unsat) break;
            }
            res = solutions.join("\n");
        }

        for (int i = 0; i < len; i++) {
            free(buf[i]);
        }
        free(buf);
    } catch(z3::exception e) {
        res = QString("Exception: ") + e.msg();
    }

    emit z3Finished(res + "\n");
}
