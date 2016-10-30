#include <cstdio>

using namespace std;

#define MAX 10000
#define NOT_CONNECTED -1

int distance[MAX][MAX];

//number of nodes
int nodesCount;

//initialize all distances to
void Initialize(){
    for (int i=0;i<MAX;++i){
        for (int j=0;j<MAX;++j){
            distance[i][j]=NOT_CONNECTED;
//            printf("%d %d\n", i, j);
        }
        distance[i][i]=0;
    }
}



int main(){

    Initialize();

    //get the nodes count
    scanf("%d", &nodesCount);

    //edges count
    int m;
    scanf("%d", &m);

    while(m--){
        //nodes - let the indexation begin from 1
        int a, b;

        //edge weight
        int c;

        scanf("%d-%d-%d", &a, &c, &b);
        distance[a][b]=c;
    }

    //Floyd-Warshall
    for (int k=1;k<=nodesCount;++k){
        for (int i=1;i<=nodesCount;++i){
            if (distance[i][k]!=NOT_CONNECTED){
                for (int j=1;j<=nodesCount;++j){
                    if (distance[k][j]!=NOT_CONNECTED && (distance[i][j]==NOT_CONNECTED || distance[i][k]+distance[k][j]<distance[i][j])){
                        distance[i][j]=distance[i][k]+distance[k][j];
                    }
                }
            }
        }
    }

    int diameter=-1;

    //look for the most distant pair
    for (int i=1;i<=nodesCount;++i){
        for (int j=1;j<=nodesCount;++j){
            if (diameter<distance[i][j]){
                diameter=distance[i][j];
                // printf("%d-%d-%d\n", i, diameter, j);
            }
        }
    }

    printf("%d\n", diameter);

    return 0;
}
