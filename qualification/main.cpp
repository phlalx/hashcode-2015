#include "aux.h"

using namespace std;

int R, S, U, P, M;
vector<slot> uslots;
vector<Server> servers;
Matrix mat;
vector<Group> groups;

vector<int> used_servers;

pii nextMove(int current) {
    auto compare = [](Group const &g1, Group const &g2) {                    
        int g1c = g1.cost();                                                 
        int g2c = g2.cost();
        bool ans;                                                            
        if (g1c == g2c) {
            return PRF(g1.group_id, g2.group_id);
        }
        ans = g1c < g2c;                                                     
        return ans;                                                          
    };                
    init_prf();

    auto i1 = min_element(groups.begin(), groups.end(), compare) - groups.begin();             
    auto r1 = groups[i1].grservers;    

    pii res = {-1, -1};
    for (auto si1 : r1) {
        for (auto si2 : used_servers) {
            if (si1 == si2 || servers[si1].group == servers[si2].group) {
                continue;
            }
            assert(servers[si1].used && servers[si2].used);
            switchservers(si1, si2);
            int obj = objective();
            if (obj > current) {
                std::cout << obj << std::endl;
                res = {si1, si2};
                switchservers(si1, si2);
                return res;
            }
            switchservers(si1, si2);
        }
    }
    return res;
}

void local_search() {
    for (auto &s : servers){
        if (s.used) used_servers.push_back(s.server_id);
    }
    while (true) {
        int current = objective();
        pii nm = nextMove(current);
        if (nm.first < 0) {
            return;
        }
        switchservers(nm.first, nm.second);
    }
}

#define N 2000

bool compare_best (Server const &s1, Server const &s2) {
        
        float s1ratio = (float) s1.c  / (float) sqrt(s1.z); 
        float s2ratio = (float) s2.c  / (float) sqrt(s2.z); 

        if (s1ratio == s2ratio) {
            return PRF(s1.server_id, s2.server_id);
        }
        return s1ratio > s2ratio ;
    };


void allocate_servers() {
    // higher ratio should be tried first
    init_prf();

    auto serversc = servers;
    sort(serversc.begin(), serversc.end(), compare_best);

    for (auto x: serversc) {
        Server &s = servers[x.server_id];
        if (s.used) { continue; }

        // find mininal cost for all groups
        auto compare = [](Group &g1, Group &g2) { 
            int g1c = g1.cost();
            int g2c = g2.cost();
            if (g1c == g2c) {
                
            }
            bool ans = g1c < g2c;
            return ans;
        };

        auto min = min_element(groups.begin(), groups.end(), compare);
        int group_i = min - groups.begin();
        //cout << group_i << endl;

        // we want to balance line capacity for this group 
        // so we find the best slot available in this order
        slot sl = mat.findBestSlot(s.server_id, group_i);
        if (sl.first < 0) {
            // no slot
            continue;
        }

        s.line = sl.first;
        s.col = sl.second;
        place(s.server_id);
        groups[group_i].add(s.server_id);
        assert(servers[s.server_id].used);
    }
}

void init() {
    mat = Matrix(R, S);
    groups = vector<Group>(P);
    for (auto p : uslots) {
        mat.set(p.first, p.second, Matrix :: unavailable);
    }
    for (auto &s : servers) {
        s.used = false;
    }   
    for (int i = 0; i < groups.size(); i++) {
        groups[i].group_id = i;
    }
    used_servers = vector<int>();
}

int main(int nargs, char **argv) {

    // parse problem and fill global structures
    parse_problem(); 

    int m = 0;
    while(true) {
        // compute solution: assign position and group id to server
        init();
        allocate_servers();
        cerr << "first score:" << objective() << endl;
        // try to greedily improve solution by perform group swaps
        local_search();
        int obj = objective();
        m = max(m, obj);
        cerr << "second score:" << objective() << endl;
        cout << "max = " << m << endl;
    }

    if (nargs > 1) {
        cout << check_solution();
    } else {
        print_solution();
    }
    return 0;
}

