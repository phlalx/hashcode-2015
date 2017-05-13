#include "aux.h"

using namespace std;

int check_solution() {
    mat = Matrix(R,S);
    vector<int> grcap(P);
    vector<int> grmaxline(P);

    // reinit matrix with unavailable slots
    for (slot s : uslots) {
        mat.set(s.first, s.second, Matrix :: unavailable);
    }

    for (Server s : servers) {
        if (!s.used) continue;
        for (int i = 0; i < s.z; i++) {
            assert(mat.get(s.line, s.col + i) == Matrix :: empty);
            mat.set(s.line, s.col + i, s.server_id);
            assert(mat.get(s.line, s.col + i) != Matrix :: empty);
            assert(s.group >= 0 && s.group < P);
        }
        grcap[s.group] += s.c;
    }

    for (int gr = 0; gr < P; gr++) {
        int line_max = 0;
        for (int line = 0; line < R; line++) {
            int current = 0;

            for(Server s : servers) {
                if (!s.used || s.group != gr || s.line != line) continue;
                current += s.c;
            }

            if (current >= line_max) {
                line_max = current;
            }
        }
        grmaxline[gr] = line_max;
    }
    int ans = INT_MAX;
    for (int i = 0; i < P; i++) {
        printf("group[%d] = %d %d %d\n",i, grcap[i], grmaxline[i], grcap[i] - grmaxline[i] );
        ans = min(ans, grcap[i] - grmaxline[i]);
    }
    return ans;
}

void print_solution() {
    for (auto s : servers) {
        if (!s.used) {
            printf("x\n");
        } else {
            printf("%d %d %d\n", s.line, s.col, s.group);
        }
    }
}

void parse_problem()  {
    scanf("%d %d %d %d %d", &R, &S, &U, &P, &M);

    // initialize global structures mat, groups, uslots, servers
    mat = Matrix(R, S);
    groups = vector<Group>(P);
    for (int i = 0; i < groups.size(); i++) {
        groups[i].group_id = i;
    }

    for (int i = 0; i < U; i++) {
        int a, b;
        scanf("%d %d\n", &a, &b);
        uslots.push_back({a, b});
        mat.set(a,b, Matrix :: unavailable);
        assert(mat.get(a,b) != Matrix :: empty);
    }

    for (int i = 0; i < M; i++) {
        int slots, cap;
        scanf("%d %d\n", &slots, &cap);
        servers.push_back(Server(slots, cap, i));
    }
}
