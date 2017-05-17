#include "aux.h"

int Node :: counter = 0;

int abs(int x) {
    return (x < 0)?(-x):x;
}

int columndist(int c1, int c2) {
    return min(abs(c1 - c2), C - abs(c1 - c2));
}

bool isCovered(int r, int c, int u, int v) {
    int y = columndist(c,v);
    int x = r - u; 
    return x * x + y * y <= V * V; 
}

void Node :: reachTarget() {
    for (int i = 0; i < L; i++) { // L number of targets
        int r = pos.first;
        int c = pos.second;
        int u = targets[i].first;
        int v = targets[i].second;
        if (alt > 0 && isCovered(r, c, u ,v)) {
            node_targets.insert(i);
        }
    }
}

void Node::explore() {
    assert( !stacked && !explored );
    stacked = true;
    // compute the cost...
    // cost = reachTarget(*this).size(); 
    reachTarget();
    // explore the (at most) three successors
    for(int i = -1; i <= 1; i++) {
        explore_i(i);
    }
    stacked = false;
    explored = true;
}

PNode X[LINES][COLUMNS][ALTITUDE]; 

void Node::explore_i(int move) {
    // compute new altitude and position
    int r = pos.first;
    int c = pos.second;
    int new_a = alt + move;
    // invalid altitude, no successor
    //
    if (new_a <= 0 || new_a > A) {
        return;
    }

    // compute new position
    int new_r = r + winds[r][c][new_a-1].first ;
    int new_c = (2 * C + c + winds[r][c][new_a-1].second) % C ;
    assert(new_c >= 0);

    // if not valid, nothing to be done - invalid move
    // the successor is null....
    if (new_r < 0 || new_r >= R) {
        return;
    }

    // else search if the corresp. succ node already exist in reach, 
    // if yes, return it
    PNode succ_i = X[new_r][new_c][new_a - 1];
    // if not found, create it and add it to the reach
    if (succ_i == NULL) {
        succ_i = new Node( {new_r, new_c}, new_a);
        nodes.push_back(succ_i);
        X[new_r][new_c][new_a-1] = succ_i;
    }

    successors[(path)move] = succ_i;

    // if not explored nor stacked call recursively explore on it
    if (! succ_i->explored && ! succ_i->stacked)
        succ_i->explore();
}

void build_graph() {
    memset(X, 0, LINES * COLUMNS * ALTITUDE);
    PNode init = new Node(initial, 0);
    init->successors[STAY] = init;
    nodes.push_back(init);
    // the initial node is not in the X, we do not care, never get back...
    // recursively explore init, and build the graph...
    init->explore(); 
    TRACE1("%lu valid nodes reached", nodes.size());
}

void sanity_check() {
    for (auto n : nodes) {
        for (auto s : n->successors) {
            int l = n->pos.first;
            int c = n->pos.second;
            int a = n->alt;
            int move = s.first;
            int l1 = s.second->pos.first;
            int a1 = s.second->alt;
            assert(a + move == a1);
            assert(l1 == l + winds[l][c][a1-1].first);
        }
    }
}
