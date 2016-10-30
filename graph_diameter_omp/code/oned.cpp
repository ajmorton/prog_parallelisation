#include <cstdio>
#include <vector>

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

    //Floyd-Warshall
    for (int k=0;k<n;++k){
        for (int i=0;i<n;++i){
            if (distance[n*i + k]!=NOT_CONNECTED){
                for (int j=0;j<n;++j){
                    if (distance[n*k + j]!=NOT_CONNECTED && (distance[n*i + j]==NOT_CONNECTED || distance[n*i + k]+distance[n*k + j]<distance[n*i + j])){
                        distance[n*i + j]=distance[n*i + k]+distance[n*k + j];
                    }
                }
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
