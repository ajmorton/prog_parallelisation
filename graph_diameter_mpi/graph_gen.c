#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_LENGTH 10

int main(int argc, char** argv){

    if(argc != 3){
        printf("inval args, must provide 2\n");
        return 0;
    }

    int nodes = atoi(argv[1]);
    int edges = atoi(argv[2]);

    // print orig details
    printf("%d\n%d\n", nodes, edges);

    srand(time(NULL));


    int a,b,c;
    for(int i = 0; i < edges; ++i){
        a = rand() % nodes + 1;
        b = rand() % nodes + 1;
        c = rand() % MAX_LENGTH + 1;

        if(a != b){
            printf("%d-%d-%d\n", a, c, b);
        } else {
            i--;
        }
    }

    return 0;
}
