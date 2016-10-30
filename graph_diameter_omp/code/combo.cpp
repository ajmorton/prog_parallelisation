#include <cstdio>
#include <vector>
#include <queue>

using namespace std;

#define NOT_CONNECTED -1
#define USE_DIJK(n, e) (e < (3*n >> 1))

int n; // number of nodes
int e; // number of edges

// all-points shortest path finding algo's
void Dijkstra(int[]);
void FloydWarshall(int[]);
void MakeGraph(int[]);
void PrintDiameter(int distance[]);

int main(){

    scanf("%d", &n);    // scan number of nodes
    scanf("%d", &e);    // scan number of edges

    int* distance = new int[n*n];
    MakeGraph(distance);

    // TODO find range
    if(USE_DIJK(n, e)){
        Dijkstra(distance);
    } else {
        FloydWarshall(distance);
    }

    PrintDiameter(distance);

    // deallocate array
    delete[] distance;

    return 0;
}


// creates the graph and loads with edges
void MakeGraph(int distance[]){

    // initialise array
    for (int i = 0; i < n*n; ++i){
        distance[i] = i % (n+1) ? NOT_CONNECTED : 0;
    }

    // load array with edges
    int a, b, c;  // a-c-b == fromNode-distance-toNode
    for(int i = 0; i < e; i++){
        scanf("%d-%d-%d", &a, &c, &b);
        distance[n*(a-1) + (b-1)]=c;
    }
}


// uses dijkstra's algorithm to find all-pairs distance
// best for sparse graphs
void Dijkstra(int distance[]){
    // Dijkstra
    for(int i = 0; i < n; ++i){
        // call for node i dijkstra

        // create pq,insert start point
        std::priority_queue< pair<int, int>, std::vector<pair<int, int>>, std::greater<pair<int, int>> > to_explore;
        to_explore.push({0, i});

        while(!to_explore.empty()) {

            // pop next shortest path
            pair<int, int> next = to_explore.top();
            to_explore.pop();
            int path = next.first;
            int loc  = next.second;

            for(int j = 0; j < n; ++j){
                if(loc == j
                || distance[n*loc + j] == NOT_CONNECTED
                || distance[n*i + j] != NOT_CONNECTED && distance[n*i + j] < path + distance[n*loc + j]
                ){ continue; }

                // else the path is the same or longer, should add either way
                if(distance[n*i + j] == NOT_CONNECTED || distance[n*i + j] > path + distance[n*loc + j]){
                    // update val
                    distance[n*i + j] = path + distance[n*loc + j];
                }

                // add new cell and distance to to_explore
                to_explore.push({distance[n*i + j], j});
            }
        }
    }

    return;
}


// uses Floyd-Warshall to find all-pairs shortest paths
// best for dense graphs
void FloydWarshall(int distance[]){
    //Floyd-Warshall
    for (int k=0; k<n ;++k){
        for (int i=0; i< n;++i){
            if (distance[n*i + k]!=NOT_CONNECTED){
                for (int j=0;j<n;++j){
                    if (distance[n*k + j]!=NOT_CONNECTED && (distance[n*i + j]==NOT_CONNECTED || distance[n*i + k]+distance[n*k + j]<distance[n*i + j])){
                        distance[n*i + j]=distance[n*i + k]+distance[n*k + j];
                    }
                }
            }
        }
    }
    return;
}


void PrintDiameter(int distance[]){
    int diameter=-1;

    //look for the most distant pair
    for (int i=0;i<n;++i){
        for (int j=0;j<n;++j){
            if (diameter<distance[n*i + j]){
                diameter=distance[n*i + j];
                // printf("%d-%d-%d\n", i+1, diameter, j+1);
            }
        }
    }

    printf("%d\n", diameter);
}
