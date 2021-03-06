
#include <cstdio>
#include <cstdlib>
#include <omp.h>


bool second_bulb(double x, double i){
    return (x + 1)*(x + 1) + i*i < 1/16.0;
}

bool cardioid_test(double x, double i){
    double q = (x - 1/4.0)*(x - 1/4.0) + i*i;
    return q*(q + (x - 1/4.0) ) < 1/4.0 * i*i;
}

// return 1 if in set, 0 otherwise
int inset(double x0, double i0, int maxiter){

    if(cardioid_test(x0, i0)) { return 1; }
    if(second_bulb(x0, i0))   { return 1; }

  double x = x0;
  double i = i0;

  for(int iters = 0; iters < maxiter; iters++){

    double x_1 = x * x - i * i;
    double i_1  = 2.0 * x * i;
    x = x_1 + x0;
    i  = i_1  + i0;
    if(x*x + i*i > 4.0) return 0;
  }
  return 1;
}

int mandelbrotSetCount(double x_min, double x_max, double i_min, double i_max, int num, int maxiter){
  int count=0;
  double real_step = (x_max - x_min) / num;
  double img_step  = (i_max - i_min) / num;

  int n = 1;
  int i_num = num;
  bool zero_case = false;

  if(i_min == -1*i_max) {
      i_num = num/2;
      n = 2;
      if(num%2 == 0){
          i_num -= 1;
          zero_case = true;
      }
  }

  #pragma omp parallel for reduction(+:count) schedule(dynamic)
  for(int real = 0; real <= num; real++){
      if(zero_case){ count += inset(x_min + real * real_step, 0, maxiter); }
    for(int img = 0; img <= i_num; img++){
      count+=n*inset(x_min + real * real_step, i_min + img * img_step, maxiter);
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
