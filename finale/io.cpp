#include "aux.h"

int DEBUG = 0;

// Input
int R, C, A;
int L, V, B, T;
pii initial;
pii targets[TARGETS];
pii winds[LINES][COLUMNS][ALTITUDE];

// Output
// altitude change at the end of each round
vector<path> solution[BALLOONS]; // altitudes for each ballon; 
vector<PNode> nodes;

void print_solution() {
    for (int turn = 0; turn < T; turn++) {
        for (int b = 0; b < B; b++) {
            assert(solution[b].size() == T);
            int move = solution[b][turn];
            if (move == DUMMY) { move = STAY; };
            printf("%2d ", move);
        }
        cout << endl;
    }
}

void parse_problem()  {
    scanf("%d %d %d\n", &R, &C, &A);
    scanf("%d %d %d %d\n", &L, &V, &B, &T);
    int r, c;
    scanf("%d %d\n", &r, &c);
    initial.first = r;
    initial.second = c;

    for (int i = 0; i < L; i++) {
        scanf("%d %d\n", &r, &c);
        targets[i].first = r;
        targets[i].second = c; 
    }

    for (int alt = 0; alt < A; alt++) {
        for (int row = 0; row < R; row++) {
            for (int col = 0; col < C; col++) {
                if (col == C-1) {
                    scanf("%d %d\n", &r, &c);
                } else {
                    scanf("%d %d ", &r, &c);
                }
                winds[row][col][alt].first = r; 
                winds[row][col][alt].second = c; 
            }
        }
    }
    assert(B == BALLOONS);
    assert(R == LINES);
    assert(C == COLUMNS);
    assert(A == ALTITUDE);
    assert(L == TARGETS);
    assert(T == ROUNDS);
}

