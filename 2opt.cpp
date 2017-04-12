
// This program solve TSP. It start from a random tour and
// tries to optimize it using 2-opt algorithm.
// Author: Sepeedeh
// April 2017

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <fstream>
#include <climits>

using namespace std;

#define max_size 1000
#define no_edge INT_MAX

void print_matrix       (int [][max_size], int);
void find_random_path   (int [][max_size], int, int, int []);

int main(int argc, char *argv[]) {
    // checking the number of commandline arguments
    if (argc != 3) {
        cout << "Error in input argument!" << endl;
        cout << "Usage: <graph_file> <start_node>" << endl;
        return -1;
    }

    // reading the number of nodes and edges from the input file
    ifstream fname(argv[1], ifstream::in);
    string str;
    getline(fname, str, '\t');
    int N = atoi(str.c_str()); // number of nodes in the graph
    getline(fname, str, '\n');
    int E = atoi(str.c_str()); // number of edges in the graph
    int start = atoi(argv[2]);

    // initializing adjacency matrix with no edges
    int dist[max_size][max_size]; // distance matrix between nodes
    for(int i=0; i<N; i++) {
        for (int j=0; j<N; j++)
        dist[i][j] = no_edge;
    }

    // filling adjacency matrix from the input file
    for (int i=0; i<E; i++) {
        int n1, n2, d;

        getline(fname, str, '\t');
        n1 = atoi(str.c_str());

        getline(fname, str, '\t');
        n2 = atoi(str.c_str());

        getline(fname, str, '\n');
        d = atoi(str.c_str());

        dist[n1][n2] = d;
        dist[n2][n1] = d;
    }
    fname.close();

    int r_path[N+1];
    find_random_path(dist, N, start, r_path);

    vector<int> path;
    path.clear();
    for (int i=0; i<=N; i++)
        path.push_back(r_path[i]);

    int cost = 0;
    for (vector<int>::iterator i_it=path.begin(); i_it!=path.end()-1; i_it++) {
        cost += dist[*i_it][*(i_it+1)];
    }
//    cout << "Initial cost = " << cost << endl;

    // performing 2-opt algorithm on the initial path
    bool opt    = true; // flag to indicate if an optimumm solution has been found in the current iteration
    int opt_cnt = 0; // counter for the number of movements
    while (opt) {
        vector<int> sub_path;
        opt = false;
        for (vector<int>::iterator i_it=path.begin()+1; (!opt) && (i_it!=path.end()-2); i_it++) {
            for (vector<int>::iterator j_it=i_it+1; (!opt) && (j_it!=path.end()-1); j_it++) {
                bool flag = true; // flag to indicate if a path is using paths with no edges
                sub_path.clear();

                for (vector<int>::iterator k_it=path.begin(); k_it!=path.end(); k_it++) {
                    if ((k_it<i_it) || (k_it>j_it))
                        sub_path.push_back(*k_it);
                    else
                        sub_path.push_back(*(j_it-k_it + i_it));
                }

                int new_cost = 0;
                for (vector<int>::iterator k_it=sub_path.begin(); k_it!=sub_path.end()-1; k_it++) {
                    int d   = dist[*k_it][*(k_it+1)];
                    if (d == INT_MAX)
                        flag = false;
                    new_cost += d;
                }

                if (!flag)
                    new_cost = INT_MAX;

                if (flag && (new_cost < cost)) {
                    path = sub_path;
                    cost = new_cost;
                    opt = true;
                    opt_cnt++;
                }
            }
        }
    } // end of while

    int tot_d = 0;
    cout << "Optimal path:\t";
    for (vector<int>::iterator i_it=path.begin(); i_it!=path.end()-1; i_it++) {
        cout << *i_it << " ";
        tot_d += dist[*i_it][*(i_it+1)];
    }
    cout << *(path.end()-1) << endl;
    cout << "Total dist = " << tot_d << endl;
    cout << "Number of movements = " << opt_cnt << endl;

    return 0;
}

// This functions prints a weighted graph
void print_matrix(int G[][max_size], int n_G){
    for (int i=0; i<n_G; i++) {
        for (int j=0; j<n_G; j++)
            cout << G[i][j] << " ";
        cout << endl;
    }
}

// This function finds a Hamiltonian path starting from vertex s
void find_random_path(int G[][max_size], int n_G, int s, int perm[]) {
    srand(time(NULL));

    perm[0]     = s;
    perm[n_G]   = s;
    int offset  = 0;
    for (int i=0; i<n_G-1; i++) {
        if (i==s)
        offset = 1;
        perm[i+1] = i + offset;
    }

    bool flag = false;
    while (!flag) {
        flag = true;

        int r1  = rand() % (n_G-1) + 1;
        int r2  = rand() % (n_G-1) + 1;
        while (r1 == r2)
            r2 = rand() % (n_G-1) + 1;

        // swap the values
        perm[r1] = perm[r1] + perm[r2];
        perm[r2] = perm[r1] - perm[r2];
        perm[r1] = perm[r1] - perm[r2];

        for (int i=0; i<=n_G; i++) {
            if (G[perm[i]][perm[i+1]] == no_edge) {
                flag = false;
                break;
            }
        }

    } // end of while

    cout << "Random path:\t";
    for (int i=0; i<=n_G; i++)
        cout << perm[i] << " ";
    cout << endl;

    return;
}
