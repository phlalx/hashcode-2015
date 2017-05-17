#ifndef MAIN_H
#define MAIN_H

#include <cstdio>
#include <set>
#include <stack>
#include <utility>
#include <vector>
#include <queue>
#include <map>
#include <iostream>
#include <cassert>
#include <cmath>
#include <algorithm>
#include <climits>
#include <cstring>

using namespace std;

#define TRACE0(FMT) if (DEBUG) { printf("trace: "); printf((FMT)); printf("\n"); }
#define TRACE1(FMT,ARG1) if (DEBUG) { printf("trace: "); printf((FMT), (ARG1)); printf("\n"); }
#define TRACE2(FMT,ARG1,ARG2) if (DEBUG) { printf("trace: "); printf((FMT), (ARG1), (ARG2)); printf("\n"); }
#define TRACE3(FMT,ARG1,ARG2,ARG3) if (DEBUG) { printf("trace: "); printf((FMT), (ARG1), (ARG2), (ARG3)); printf("\n"); }
typedef pair<int, int> pii;

extern int DEBUG;

const int NODES = 119000;
const int ROUNDS = 400;
const int TARGETS = 2250;
const int LINES = 75;
const int COLUMNS = 300;
const int ALTITUDE = 8;
const int BALLOONS = 53;

enum path { UP = 1, DOWN = -1, STAY = 0, DUMMY = -2};

// input
extern int R, C, A;
extern int L, V, B, T; 
extern pii initial; 
extern pii targets[TARGETS];
extern pii winds[LINES][COLUMNS][ALTITUDE];

// solution
extern vector<path> solution[BALLOONS]; // altitude at round i (0 before beginning of round)

struct Node {
    const pii pos;
    const int alt;

    set<int> node_targets;
    int id = 0;

    map<path, Node *> successors;  

    bool explored, stacked;
    
    static int counter;

    Node (pii p, int a) : pos{p}, alt{a}{
        stacked = explored = false;
        id = counter++;
    }

    void explore();
    void explore_i(int);
    void reachTarget();
};

typedef Node *PNode;
extern vector<PNode> nodes;

void build_graph();
void print_solution();
void parse_problem();

#endif
