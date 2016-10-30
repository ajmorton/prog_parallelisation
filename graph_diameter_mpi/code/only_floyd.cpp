// diameter.cpp

// optimisations made:
//   - removed static 2D array, replaced with dynamic 1D array
//   - implemeted parallel Dijkstra

#define ZERO me == 0

#include <cstdio>
#include <queue>
#include <mpi.h>
#include <utility>

using namespace std;

typedef std::pair<int, int> pint;

#define NOT_CONNECTED -1

// graph features
int n;          // number of nodes
int e;          // number of edges

int ac;
char** av;

// MPI values
int me;         // process id
int num_nodes;  // number of nodes in use

int longest = -1;         // the longest, shortest path overall
int my_longest = -1; // the longest, shortest path per process

void Dijkstra(pint[]);
void MakeGraph(pint[]);
void mpi_init();
void Floyd_Warshall(pint[]);

int main(){

    mpi_init();

    scanf("%d", &n);    // scan number of nodes
    scanf("%d", &e);    // scan number of edges

    pint* distance = new pint[n*n]; // instantiate distance array

    MakeGraph(distance);

    // use dijkstra?
    if(false){
        Dijkstra(distance);
        MPI_Reduce(&my_longest, &longest, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
    } else {
        if(ZERO)
            Floyd_Warshall(distance);
    }

    if(ZERO)
    printf("%d\n", longest);

    delete[] distance;     // deallocate arrays

    MPI_Finalize();

    return 0;
}

void mpi_init(){
    MPI_Init(&ac, &av);
    MPI_Comm_size(MPI_COMM_WORLD, &num_nodes);
    MPI_Comm_rank(MPI_COMM_WORLD, &me);
}

// creates the graph and loads with edges
void MakeGraph(pint distance[]){

    for (int i = 0; i < n*n; ++i){
        distance[i] = i % (n+1) ? make_pair(NOT_CONNECTED,-1) : make_pair(0, -1);
    }

    // load array with edges
    int a, b, c;  // a-c-b == fromNode-distance-toNode
    for(int i = 0; i < e; i++){
        scanf("%d-%d-%d", &a, &c, &b);
        distance[n*(a-1) + (b-1)]= make_pair(c, -1);
    }

//    for(int i=0; i<n; i++){
//        for(int j=0; j<n; j++){
//	    printf("%d ", distance[n*i + j].first);
//        }
//        printf("\n");
//    }

}

void Floyd_Warshall(pint distance[]){

//Floyd-Warshall
    for (int k=0;k<n;++k){
        for (int i=0;i<n;++i){
            if (distance[n*i + k].first!=NOT_CONNECTED){
                for (int j=0;j<n;++j){
                    if (distance[n*k + j].first != NOT_CONNECTED && (distance[n*i + j].first == NOT_CONNECTED || distance[n*i + k].first + distance[n*k + j].first < distance[n*i + j].first)){
                        distance[n*i + j].first = distance[n*i + k].first + distance[n*k + j].first;
                    }
                }
            }
        }
    }

    //look for the most distant pair
    for (int i=0;i<n;++i){
        for (int j=0;j<n;++j){
            if (longest<distance[n*i + j].first){
                longest=distance[n*i + j].first;
            }
        }
    }
}


// uses dijkstra's algorithm to find all-pairs distance
// best for sparse graphs
void Dijkstra(pint distance[]){

    for(int i = me; i < n; i += num_nodes){
        // insert start point
        std::priority_queue< pair<int, int>, std::vector<pair<int, int> >, std::greater<pair<int, int> > > to_explore;
        to_explore.push( make_pair(0, i) );

        while(!to_explore.empty()) {

            // pop next shortest path
            pair<int, int> next = to_explore.top();
            to_explore.pop();
            int path = next.first;
            int loc  = next.second;

            // potential for omp here, but startup costs are likely too great
            for(int j = 0; j < n; ++j){

                int old_distance = distance[n*i + j].first;
                int new_distance = path + distance[n*loc + j].first;

                // if
                // 1. travelling to self,
                // 2. destination is unreachable from loc
                // 3. The node already is on a shorter path
                // 4. The node has already been explored by an equivalent path
                // don't expand
                if(loc == j
                ||  distance[n*loc + j].first == NOT_CONNECTED
                || (distance[n*i + j].first != NOT_CONNECTED && old_distance < new_distance)
                || (distance[n*i + j].first != NOT_CONNECTED && old_distance == new_distance && distance[n*i + j].second == i)
                ){ continue; }

                // else the path is the same or longer, should add either way
                if(old_distance == NOT_CONNECTED || old_distance >= new_distance){
                    // update value
                    distance[n*i + j].first  = new_distance;
                    distance[n*i + j].second = i;
                }

                to_explore.push( make_pair(distance[n*i + j].first, j) );
            }
        }
    }

    for(int i = me; i < n; i += num_nodes){
        for(int j=0; j < n; j++){
            if(my_longest < distance[n*i + j].first){
                my_longest = distance[n*i + j].first;
            }
        }
    }

    return;
}

