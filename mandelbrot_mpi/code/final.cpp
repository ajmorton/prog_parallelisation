// Parallel and Multicore Project 1C
// Optimisation using OpenMP and MPI
// author: ajmorton
// date:   5/10/16


#include <cstdio>
#include <cstdlib>
#include <mpi.h>
#include <omp.h>

// global constants
#define QUARTER 1/4.0

// MPI node identifiers
#define ZERO me == 0
#define LAST me == num_nodes - 1

int me, num_nodes;

// checks if a point is within the second bulb of the mandelbrot set
bool second_bulb(double x, double i){
    return (x + 1) * (x + 1) + i*i < 1/16.0;
}

// checks if a point is in the cardioid of the mandelbrot set
bool cardioid_test(double x, double i){
    double q = (x - QUARTER) * (x - QUARTER) + i*i;
    return q*(q + (x - QUARTER) ) < QUARTER * i*i;
}

// checks if a point is inside the mandelbrot set
// may run for up to maxiter iterations, but uses
// cardioid_test and second_bulb to drastically
// reduce average complexity
int in_set(double x0, double i0, int maxiter){

    if(cardioid_test(x0, i0)) { return 1; }
    if(second_bulb(x0, i0))   { return 1; }

    double x = x0;
    double i = i0;

    for(int iters = 0; iters < maxiter; ++iters){
        double x_1 = x * x - i * i;
        double i_1 = 2.0 * x * i;

        x = x_1 + x0;
        i = i_1 + i0;

        if(x*x + i*i > 4.0) return 0;
    }
  return 1;
}

// checks num points between {x_min, i_min, x_max, i_max} to
// determine how many of said points are in the mandelbrot set.
int mandelbrotSetCount(double x_min, double x_max, double i_min, double i_max, int num, int maxiter){
    int    count   = 0;
    double x_step  = (x_max - x_min) / num;
    double i_step  = (i_max - i_min) / num;

    int  n         = 1;
    int  i_num     = num;
    bool zero_case = false;

    // symmetry optimisation, if symmetric across the x-axis then count
    // points on and above the x-axis and count them twice
    if(i_min == -1*i_max){
        i_num = num/2;
        n = 2;
        if(num%2 == 0){
            i_num -= 1;
            zero_case = true;
        }
    }

    // distribute workload evenly accross MPI nodes,
    // run in parallel with OpenMP on each node
    #pragma omp parallel for reduction(+:count) schedule(dynamic)
    for(int x = me; x <= num; x += num_nodes){
        if(zero_case){
            count += in_set(x_min + x * x_step, 0, maxiter);
        }
        for(int i = 0; i <= i_num; ++i){
            count += n*in_set(x_min + x * x_step, i_min + i * i_step, maxiter);
        }
    }
    return count;
}

// initialise MPI nodes
void mpi_init(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_nodes);
    MPI_Comm_rank(MPI_COMM_WORLD, &me);
}

// main function
// takes in num_regions arguments of (x_min, x_max, i_min, i_max, num, maxiter)
int main(int argc, char *argv[]){
    mpi_init(argc, argv);

    double x_min;
    double x_max;
    double i_min;
    double i_max;

    int num;
    int maxiter;

    const int num_regions = (argc-1)/6;
    int results[num_regions];

    int scores[num_regions];
    int my_scores[num_regions];

    for(int i = 0; i < num_regions; ++i){
        scores[i] = 0;
        my_scores[i] = 0;
    }

    for(int region=0; region < num_regions; ++region){
        // replaced sscanf with atoi/atof
        x_min   = atof(argv[region*6 + 1]);
        x_max   = atof(argv[region*6 + 2]);
        i_min   = atof(argv[region*6 + 3]);
        i_max   = atof(argv[region*6 + 4]);
        num     = atoi(argv[region*6 + 5]);
        maxiter = atoi(argv[region*6 + 6]);

        // stored partial counts of each node in its own
        // array to reduce the number of MPI_reduce() calls
        my_scores[region] = mandelbrotSetCount(x_min, x_max, i_min, i_max, num, maxiter);
    }

    MPI_Reduce(my_scores, scores, num_regions, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // only the first node prints output
    if(ZERO){
        for(int i = 0; i < num_regions; ++i){
            printf("%d\n", scores[i]);
        }
    }

    MPI_Finalize();
    return 0;
}

