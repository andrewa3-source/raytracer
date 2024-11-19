#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 1000000

#define frand() (rand() / (RAND_MAX + 1.0))

int main(int argc, char *argv[]) {
    int i, n;
    double f;
    double sum;
    double b;

    n = N;

    if (argc != 1){ 
        n = atoi(argv[1]);
    }

    b = M_PI;

    if(argc == 3){
        b = strtod(argv[2], NULL);
    }
    srand(time(NULL));

    for (i = 0, sum = 0; i < n; i++) {
        f = frand() * b;
        sum += sin(f);
    }

    sum /= n;

    printf("Result: %f\n", sum * b);

    return 0;
}