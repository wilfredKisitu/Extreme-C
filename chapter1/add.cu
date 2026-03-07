#include <stdio.h>
#include <stdlib.h>



void vecAdd(float* A_h, float* B_h, float* C_h, int n){
    for(int i = 0; i < n; i++){
        C_h[i] = A_h[i] + B_h[i];
    }
}


// void vecAddCuda(float* A, float* B, float*C, int n) {

//     float *A_d, *B_d, *C_d;
//     int size = n * sizeof(float);

//     cudaMalloc((void**)&A_d, size);
//     cudaMalloc((void**)&B_d, size);
//     cudaMalloc((void**)&C_d, size);

//     cudaMemcpy(A_d, A, size, cudaMemcpyHostToDevice);
//     cudaMemcpy(B_d, B, size, cudaMemcpyHostToDevice);
//     cudaMemcpy(C_d, C, size, cudaMemcypHostToDevice);


//     cudaFree(A_d);
//     cudaFree(B_d);
//     cudaFree(C_d);
// }


int main(int argc, char const *argv[])
{
    int n  = 5;


    float* A = (float*)malloc(n * sizeof(float));
    float* B = (float*) malloc(n* sizeof(float));
    float* C = (float*) malloc(n* sizeof(float));


    for(int i = 0; i < n; i++){
        A[i] = i * 1.0f;
        B[i] = i * 2.0f;
    }

    vecAdd(A, B, C, n);


    for(int i = 0; i < n; i++){
        printf("%f\n", C[i]);
    }

    free(A);
    free(B);
    free(C);
    return 0;
}
