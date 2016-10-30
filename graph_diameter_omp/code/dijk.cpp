#include <cstdio>
#include <vector>
#include <queue>

using namespace std;

#define NOT_CONNECTED -1

//number of nodes
int n;

//initialize all distances to
void Initialize(int distance[], int size){
    for (int i = 0; i < n*n; ++i){
        distance[i] = i % (n+1) ? NOT_CONNECTED : 0;
    }
}

int main(){
    //get the nodes count
    scanf("%d", &n);

    int* distance = new int[n*n];

    Initialize(distance, n);

    //edges count
    int m;
    scanf("%d", &m);

    int a, b;  //nodes - let the indexation begin from 1
    int c;     //edge weight

    while(m--){
        scanf("%d-%d-%d", &a, &c, &b);
        distance[n*(a-1) + (b-1)]=c;
    }

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

    // deallocate array
    delete[] distance;

    return 0;
}
