#include <stdio.h>

int sobel_x[3][3] = {
    { -1, 0, 1 },
    { -2, 0, 2 },
    { -1, 0, 1 }
};

int sobel_y[3][3] = {
    { 1, 2, 1 },
    { 0, 0, 0 },
    { -1, -2, -1 }
};


void convolve(int k[3][3], int image[1024][1024], int out[1024][1024]){
    int i, j, m, n; //two dimensions for kernel, two for image

    for (n = 0; n < 1024; n++){
        for (m = 0; m < 1024; m++){
            int sum = 0;
            out[n][m] = 0;
            for (j = 0; j < 3; j++){
                for (i = 0; i < 3; i++){
                    sum += k[i][j] * image[n + i][m + j];
                    out[n][m] = k[j][i] * image[m][n];
                }
            }
            image[n][m] = sum;
        }
    }


}