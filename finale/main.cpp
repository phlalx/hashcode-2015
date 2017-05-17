#include "aux.h"

// target[r][t] est vrai si le target est visité 
// par au moins un ballon après round changements de
// positions
bool target[ROUNDS+1][TARGETS];

// soit b un ballon en position PNode n
// le ballon a déja subi round changements de position
// dp retourne le meilleur score possible pour les 
// (T - round) changements de positions restant
// renvoie res.first = DUMMY si ballon perdu ou r = T

pair<path, int> memo[NODES][ROUNDS+1];
pair<path, int> dp(PNode n, int round) {
    if (memo[n->id][round].second != -1) {
        return memo[n->id][round];
    }

    int cost = 0;
    path move = DUMMY;

    for (auto t : n->node_targets) {
        cost += target[round][t]?0:1; // count it if not covered already 
    }

    if (round != T) {
        int max_cost = 0;
        for (auto succ : n->successors) {
            int tmp = dp(succ.second, round + 1).second;
            if (tmp > max_cost) {
                move = succ.first; 
                max_cost = tmp; 
            } 
        }   
        cost += max_cost;
    }
    return memo[n->id][round] = {move, cost};
}   


// call dp to compute best score and move sequence for balloon bal
// update visited targets according to that itinerary
// compute solution
int findDpSolution(int bal) {
    TRACE1("findDpSolution %d", bal);
    assert(solution[bal].size() == 0);

    for (int i = 0; i < nodes.size(); i++) {
        for (int j = 0; j <= T; j++) {
            memo[i][j] = {DUMMY, -1};
        }
    }

    PNode current = nodes[0];

    // compute path and score starting from initial position
    pair<path, int> p = dp(current, 0);
    assert(p.first != DUMMY);

    TRACE1("ballon dp %d", p.second);

    // T altitude changes
    int i = 0;
    while (i <= T) {
        pair<path, int> s = memo[current->id][i];
        int new_seen = 0;
        // udpate visited target 
        for (auto t : current->node_targets) {
            if (!target[i][t]) { new_seen++; }
            target[i][t] = true;
        }

        if (s.first != DUMMY) {
            // compute next position
            solution[bal].push_back(s.first);
            current = current->successors[s.first];
            i++;
        } else { 
            if (i < T) {
                while (i < T) {
                    // balloon lost, fill solution with  
                    solution[bal].push_back(DUMMY);
                    i++;
                }
            }
            i++;
        }
    }
    assert(solution[bal].size() == T);
    TRACE1("findDpSolution done %d", p.second);
    return p.second;
}

// if i != -1 erase ballon i
// returns current score, and number of balloons
pii erase_ballon(int i) {
    if (i != -1) {
        TRACE1("erase balloon %d", i);
        solution[i] = vector<path>(); // reinit altitude for this ballon
    }

    int score = 0;
    int nb_balloons = 0;

    memset(&target[0][0], false, (TARGETS * (ROUNDS + 1)) * sizeof(target[0][0]));

    // recompute targets
    for (int b = 0; b < B; b++) {
        if (solution[b].size() == 0) { continue; }
        nb_balloons++;
        PNode current = nodes[0];

        int i = 0;
        while (i < T) {
            // score after i moves
            if (solution[b][i] == DUMMY) {
                break;
            }
            path p = solution[b][i];
            current = current->successors[p];
            i++;

            for (auto tar : current->node_targets) {
                if (!target[i][tar]) score++;
                target[i][tar] = true;
            }
        }  
    }
    TRACE1("score after erasal %d", score);
    return {score, nb_balloons};
}


int main(int nargs, char **argv) {
    if (nargs > 1) {
        DEBUG = 1;
    }

    srand(time(NULL));

    // parse problem and fill global structures
    parse_problem(); 
    build_graph();

    int score = 0;

    bool improved[BALLOONS];
    memset(improved, false, BALLOONS * sizeof(improved[0]));


    // int i = 0 
    // compute (possibly recompute )balloons itinerary 
    // until no improvement after trying to recompute all balloons
    //
    while (true){ 
        int i = rand() % B;
        cerr << "computing ballon " << i << endl;
        if (solution[i].size() != 0) {
            cerr << "erase previous solution " << i << endl;
            erase_ballon(i);
        }
        findDpSolution(i);
        int old_score = score;
        pii x = erase_ballon(-1); 
        score = x.first;
        int nb_balloons = x.second; 
        improved[i] = score > old_score;

        cerr << "new score = " << score << " for " << nb_balloons << " balloons" << endl;

        bool improvement = false;
        for (int i = 0; i < BALLOONS; i++) {
            if (improved[i]) { improvement = true; break; }
        }
        if (nb_balloons == B && !improvement) break;
        //i++;
        //int stop = B;
        //if (i == stop) {
            //for (int j = 0; j < T; j++) {
                //for (int bal = stop; bal < B; bal++) {
                    //solution[bal].push_back(solution[0][j]);
                //}
            //}
            //break;
        //}
    }

    if (!DEBUG) {
        print_solution();
    }

    TRACE0("exit main");

    return 0;
}

