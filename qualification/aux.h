#ifndef MAIN_H

#define MAIN_H

// #include <cstdio>
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

using namespace std;

typedef pair<int, int> pii;
typedef pii slot;
typedef int serverid;
typedef int groupid;

bool PRF(int a, int b);

void init_prf(); 

struct Server {
    int z; // size 
    int c; // capacity 
    bool used;
    int line;
    int col;
    int group;
    serverid server_id;
    Server(int _z, int _c, int _server_id);
};

struct Group {

    set<serverid> grservers;
    groupid group_id;

    void add(serverid s);
    void remove(serverid s);

    int total_cap() const;

    // first = count, second = cost
    pii max_line() const;
    int cost() const;
};

struct Matrix {

    static const int empty = -2;
    static const int unavailable = -1;

    vector<int> bits;

    int lines;
    int col;
    void set(int i, int j, int val);
    int get(int i, int j);
    bool isEmpty(int i, int j);
    Matrix(); 
    Matrix(int n, int m);

    void print();

    // space left after position i, j (including (i,j))
    int space(int i, int j);

    // if no slot, result.first < 0;
    slot findBestSlot(serverid, groupid);
};


void place(serverid si);
void remove(serverid si);
 
int objective();

int check_solution();

void print_solution();

void parse_problem();

void switchservers(serverid, serverid);

extern int R, S, U, P, M;
extern Matrix mat;
extern vector<slot> uslots;
extern vector<Server> servers;
extern vector<Group> groups;

#endif
