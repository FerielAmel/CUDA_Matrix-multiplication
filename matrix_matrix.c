% % cu
#include <stdio.h>
#include <stdlib.h>
#define GRIDSIZE 4
#define BLOCKSIZE 2
#define SIZE 16

        __global__ void
        matrix_matrix(int *A, int *B, int *C)
{
    int ind_l = blockIdx.x * gridDim.x + blockIdx.y;
    int ind_c = 0;
    int sum = 0;
    int i = 0;
    for (ind_c = 0; ind_c < SIZE; ind_c++)
    {
        sum = 0;
        for (i = 0; i < SIZE; i++)
        {
            sum = sum + A[ind_l * SIZE + i] * B[i * SIZE + ind_c];
        }
        C[ind_l * SIZE + ind_c] = sum;
    }
}

void fill_dp_vector(int *vec, int size)
{
    int ind;
    for (ind = 0; ind < size; ind++)
        vec[ind] = 3 * ind;
}

int main()
{
    int *hostA, *hostB, *hostC;
    int *devA, *devB, *devC;
    int i = 0;
    dim3 grid(GRIDSIZE, GRIDSIZE);

    hostA = (int *)malloc(SIZE * SIZE * sizeof(int));
    hostB = (int *)malloc(SIZE * SIZE * sizeof(int));
    hostC = (int *)malloc(SIZE * SIZE * sizeof(int));

    cudaMalloc((void **)&devA, SIZE * SIZE * sizeof(int));
    cudaMalloc((void **)&devB, SIZE * SIZE * sizeof(int));
    cudaMalloc((void **)&devC, SIZE * SIZE * sizeof(int));

    fill_dp_vector(hostA, SIZE * SIZE);
    fill_dp_vector(hostB, SIZE * SIZE);

    cudaMemcpy(devA, hostA, SIZE * SIZE * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(devB, hostB, SIZE * SIZE * sizeof(int), cudaMemcpyHostToDevice);

    cudaEvent_t start, stop;
    float elapsedTime;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    cudaEventRecord(start, 0);

    matrix_matrix<<<grid, 1>>>(devA, devB, devC);

    cudaEventRecord(stop, 0);
    cudaEventSynchronize(stop);
    cudaEventElapsedTime(&elapsedTime, start, stop);
    cudaDeviceSynchronize();
    printf("Elapsed time : %f ms\n", elapsedTime);

    for (i = 0; i < SIZE; i++)
    {
        printf("%d\n", hostC[i]);
    }

    cudaMemcpy(hostC, devC, sizeof(int), cudaMemcpyDeviceToHost);

    cudaFree(devA);
    cudaFree(devB);
    cudaFree(devC);

    free(hostA);
    free(hostB);
    free(hostC);

    return 0;
}