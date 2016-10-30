
#include <cstdio>
#include <cstdlib>

// doubles are not precise for high values of num
// symmetry is only guaranteed to be correct below this value

// return 1 if in set, 0 otherwise
int inset(long double real, long double img, int maxiter){
  long double z_real = real;
  long double z_img  = img;

  for(int iters = 0; iters < maxiter; iters++){

    long double z2_real = z_real * z_real - z_img * z_img;
    long double z2_img  = 2.0 * z_real * z_img;
    z_real = z2_real + real;
    z_img  = z2_img  + img;
    if(z_real*z_real + z_img*z_img > 4.0) return 0;
  }
  return 1;
}

long mandelbrotSetCount(double x_min, double x_max, double i_min, double i_max, int num, int maxiter){
  long count=0;
  long double real_step = (x_max - x_min) / num;
  long double img_step  = (i_max - i_min) / num;

  int n = 1;
  int i_num = num;
  bool zero_case = false;

  if(i_min == -1*i_max){
      i_num = num/2;
      n = 2;
      if(num%2 == 0){
          i_num -= 1;
          zero_case = true;
      }
  }

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
    printf("%ld\n",mandelbrotSetCount(x_min, x_max, i_min, i_max, num, maxiter));
  }
  return EXIT_SUCCESS;
}
