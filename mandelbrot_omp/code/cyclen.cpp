// cyclen
// checks for stable points in the mandelbrot set, length n
// if found then ther is no need to continue iterating

#include <cstdio>
#include <cstdlib>

// optimal value seems to be 5
#define NUM 1

// return 1 if in set, 0 otherwise
int inset(double x0, double i0, int maxiter){

  double prev_points_x[NUM];
  double prev_points_i[NUM];

  int j;

  // define each prev value
  for(j = 0; j < NUM; j++) {
    prev_points_x[j] = x0;
    prev_points_i[j] = i0;
  }

  // current iteration
  double x;
  double i;

  double x_1;
  double i_1;

  for(int iters = 0; iters < maxiter; iters++){

    x_1 = prev_points_x[0];
    i_1 = prev_points_i[0];

    x = x_1 * x_1 - i_1 * i_1 + x0;
    i = 2.0 * x_1 * i_1 + i0;

    // periodicity testing
    // if the new value is the same as the previous then there is a cycle
    // the point will never bound to inifinity
    for(j = 0; j < NUM; j++) {
        if(x == prev_points_x[j] && i == prev_points_i[j] ){
            return 1;
        }
    }

    if(x * x + i * i > 4.0) return 0;

    //update prev_points
    for(j = NUM-1; j > 0; j--) {
        prev_points_x[j] = prev_points_x[j-1];
        prev_points_i[j] = prev_points_i[j-1];
    }
    prev_points_x[0] = x;
    prev_points_i[0] = i;
  }
  return 1;
}

// count the number of points in the set, within the region
int mandelbrotSetCount(double x_min, double x_max, double i_min, double i_max, int num, int maxiter){
  int count=0;
  double real_step = (x_max - x_min) / num;
  double img_step  = (i_max - i_min) / num;

  for(int real = 0; real <= num; real++){
    for(int img = 0; img <= num; img++){
      count+=inset(x_min + real * real_step, i_min + img * img_step, maxiter);
    }
  }
  return count;
}

// main
int main(int argc, char *argv[]){
  double x_min;
  double x_max;
  double i_min;
  double i_max;
  int num;
  int maxiter;
  int num_regions = (argc-1)/6;
  for(int region=0; region < num_regions; region++){
    // scan the arguments
    sscanf(argv[region*6+1],"%lf",&x_min);
    sscanf(argv[region*6+2],"%lf",&x_max);
    sscanf(argv[region*6+3],"%lf",&i_min);
    sscanf(argv[region*6+4],"%lf",&i_max);
    sscanf(argv[region*6+5],"%i",&num);
    sscanf(argv[region*6+6],"%i",&maxiter);
    printf("%d\n",mandelbrotSetCount(x_min, x_max, i_min, i_max, num, maxiter));
  }
  return EXIT_SUCCESS;
}
