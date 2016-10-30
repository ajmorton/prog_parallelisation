
#include <cstdio>
#include <cstdlib>
#include <mpi.h>
#include <omp.h>

#define QUARTER 1/4.0
#define ZERO me == 0
#define LAST me == num_nodes - 1

int me, num_nodes;

bool second_bulb(double x, double i){
    return (x + 1) * (x + 1) + i*i < 1/16.0;
}

bool cardioid_test(double x, double i){
    double q = (x - QUARTER) * (x - QUARTER) + i*i;
    return q*(q + (x - QUARTER) ) < QUARTER * i*i;
}

// return 1 if in set, 0 otherwise
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

int mandelbrotSetCount(double x_min, double x_max, double i_min, double i_max, int num, int maxiter){
  int    count     = 0;
  double real_step = (x_max - x_min) / num;
  double img_step  = (i_max - i_min) / num;

  int  n         = 1;
  int  i_num     = num;
  bool zero_case = false;

  if(i_min == -1*i_max){
      i_num = num/2;
      n = 2;
      if(num%2 == 0){
          i_num -= 1;
          zero_case = true;
      }
  }

  // distribute workload accross processes, give leftover vals to last process
  int start = me*(num / num_nodes);
  int end = (me + 1)*(num/num_nodes) - 1;

  if(LAST){
      end += num % num_nodes;
  }

  #pragma omp parallel for reduction(+:count) schedule(dynamic)
  for(int real = start; real <= end; ++real){
      if(zero_case){
          count += in_set(x_min + real * real_step, 0, maxiter);
      }
    for(int img = 0; img <= i_num; ++img){
      count += n*in_set(x_min + real * real_step, i_min + img * img_step, maxiter);
    }
  }

  return count;
}


void mpi_init(int argc, char **argv) {
    int blank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_nodes);
    MPI_Comm_rank(MPI_COMM_WORLD, &me);
}

// main
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
      x_min   = atof(argv[region*6 + 1]);
      x_max   = atof(argv[region*6 + 2]);
      i_min   = atof(argv[region*6 + 3]);
      i_max   = atof(argv[region*6 + 4]);
      num     = atoi(argv[region*6 + 5]);
      maxiter = atoi(argv[region*6 + 6]);

    my_scores[region] = mandelbrotSetCount(x_min, x_max, i_min, i_max, num, maxiter);
  }

  MPI_Reduce(my_scores, scores, num_regions, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  if(ZERO){
      for(int i = 0; i < num_regions; ++i){
          printf("%d\n", scores[i]);
      }
  }

  MPI_Finalize();
  return 0;
}
