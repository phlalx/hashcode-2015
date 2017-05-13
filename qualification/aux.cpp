#include "aux.h"

using namespace std;

#define NMAX 2000


vector<vector<int>> memo;

bool PRF(int a, int b) {
    int x = memo.at(a).at(b);
    if (x != -1) {
        return x;
    }
    return memo[a][b] = ((rand() % 2));
}

void init_prf() {
    srand(time(NULL));
    memo = vector<vector<int>>(NMAX, vector<int>(NMAX, -1));
}


Server :: Server(int _z, int _c, int _server_id) {
    z = _z;
    c = _c;
    server_id = _server_id;
    used = false;
}

void Matrix :: set(int i, int j, int val) {
    bits.at(i * col + j) = val;
}

int Matrix :: get(int i, int j) {
    return bits.at(i * col + j);
}

bool Matrix :: isEmpty(int i, int j) {
    return bits.at(i * col + j) == -2;
}

Matrix :: Matrix() {  };

void Matrix :: print() { 
    for (int i = 0; i < lines; i++) {
        for (int j = 0; j < col; j++) {
            char d;
            switch (get(i,j)) {
                case empty: d = '.'; break;
                case unavailable: d = '#'; break;
                default: d = 'S'; break;
            }
            cout << d;
        }
        cout << endl;
    }
}

Matrix :: Matrix(int _lines, int _col) { // n = rows, m = colomns
    lines = _lines;
    col = _col;
    bits = vector<int>(lines * col); 
    for (int i = 0; i < bits.size(); i++) {
        bits[i] = empty;
    }
}

// Group functions
void Group :: add(serverid si) {
    Server &s = servers[si];
    s.used = true;
    s.group = group_id;
    grservers.insert(si);
}

void Group :: remove(serverid si) {
    Server &s = servers[si];
    s.used = false;
    grservers.erase(si);
}


pii Group :: max_line() const {
    vector<int> c(R);

    for(auto sid : grservers) {
        Server &s = servers[sid];
        assert(s.group == group_id && s.used);
        c[s.line] += s.c;
    }
    auto me = max_element(c.begin(), c.end());
    int count = 0;
    for (int i = 0; i < c.size(); i++) {
        if (c[i] == *me) count++;
    }

    return {count, *me}; 
}

int Group :: total_cap() const {
    int res = 0;
    for(auto sid : grservers) {
        Server s = servers[sid];
        assert(s.used);
        res += s.c;
    }
    return res;
}


int Group :: cost() const {
    return total_cap() - max_line().second;
}


struct BestSlot {
    slot s;
    int cost;
    int total;
    int remain_slots;
    int pt;

    BestSlot(slot _s, int _cost, int _total, int _remain_slots) {
        s = _s; cost = _cost; total = _total, remain_slots = _remain_slots; 
        int target = 1000 / groups.size(); 
        int percent = (total == 0)?target:cost * 1000 / total;
        pt = target - percent;
    }


    int heuristic ()  const {
//       return remain_slots -  cost + pt; 
       return remain_slots + 10 * (400 -  cost) + 10000 * pt; 
    }

    bool  operator<(BestSlot const &bs) const{ 
        int x = heuristic();
        int y = bs.heuristic();
//        if (x == y) {
//            return PRF(s.second, bs.s.second);
//        }

        return x < y; 
    }
};

int Matrix :: space(int i, int j) {
    int remaining_slots = 0;
    while (j < S && mat.isEmpty(i, j)) {
        remaining_slots++;
        j++;
    }
    return remaining_slots;
}


void place(serverid si) {
    int l = servers[si].line;
    int c = servers[si].col;
    for (int i = 0; i < servers[si].z; i++) {
        assert(mat.isEmpty(l, c+i));
        mat.set(l, c + i, si);
    }
}

void remove(serverid si) {
    int l = servers[si].line;
    int c = servers[si].col;
    int z = servers[si].z;
    for (int i = 0; i < z; i++) {
        mat.set(l, c + i, Matrix :: empty);
    }
}

slot Matrix :: findBestSlot(serverid si, int gi) {
    Server &s = servers[si];
    vector<int> lines(R);
    vector<int> total(R);

    // compute total capacity on this line
    for (auto &s : servers) {
        if (!s.used) { continue; }
        total[s.line] += s.c;
        if (s.group == gi) {
            lines[s.line] += s.c;
        }
    }
    //init_prf();

    // compute a weight for each slot // the highest, the better
    priority_queue<BestSlot> sl;

    for (int l = 0; l < R; l++) {

        for (int col = 0; col + s.z <= S; col++) {
            bool fit = true;
            for (int i = 0; i < s.z; i++) {
                if (get(l, col + i) >= -1) {
                    fit = false;
                    break;
                }
            }
            if (!fit) { continue; }
            // it fits at slot (l, col)
            if (col > 0 && (isEmpty(l, col - 1))) {
                // we only consider slots that are next to a used slot
                continue;
            }

            int remaining_slots = space(l, col + s.z);

            // heuristic minimize cost, but maximize remaining slots
            auto bs = BestSlot({l, col}, lines[l], total[l], remaining_slots);
            sl.push(bs);
        }
    }

    if (sl.size() == 0) {
        // first is negative if no possible slot 
        return {-1, 0};
    }

    auto best = sl.top().s;
    return best;
}

const int SHIFT = 100;

int objective() {
    int ans = INT_MAX;
    for (auto &gr : groups) {
        ans = min(ans, gr.cost());
    }
    int count = 0;

    for (auto &gr : groups) {
        if (gr.cost() == ans) {
            count++;
        }
    }

    return (SHIFT * ans +  P - count);
}

void switchservers(serverid si1, serverid si2) {
    assert(si1 != si2);
    Server &s1 = servers[si1]; Server &s2 = servers[si2];
    assert(s1.used && s2.used);
 
    int gi1 = s1.group;
    int gi2 = s2.group;
    Group &g1 = groups[gi1]; Group &g2 = groups[gi2];

    // switch in groups
    g1.remove(si1);
    g2.remove(si2);
    g1.add(si2);
    g2.add(si1);
}

