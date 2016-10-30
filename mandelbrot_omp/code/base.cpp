
#include <cstdio>
#include <cstdlib>

// return 1 if in set, 0 otherwise
int inset(double real, double img, int maxiter){
  double z_real = real;
  double z_img  = img;

  for(int iters = 0; iters < maxiter; iters++){

    double z2_real = z_real * z_real - z_img * z_img;
    double z2_img  = 2.0 * z_real * z_img;
    z_real = z2_real + real;
    z_img  = z2_img  + img;
    if(z_real*z_real + z_img*z_img > 4.0) return 0;
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
