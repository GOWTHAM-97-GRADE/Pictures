
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define N 4

void multiply_row(int row[], int B[N][N], int result[], int n) {
    for (int j = 0; j < n; j++) {
        result[j] = 0;
        for (int k = 0; k < n; k++) {
            result[j] += row[k] * B[k][j];
        }
    }
}

int main(int argc, char *argv[]) {
    int rank, size;
    int A[N][N], B[N][N], C[N][N]; // Matrices

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (N % size != 0) {
        if (rank == 0) {
            printf("Error: Number of processes must evenly divide N (%d).\n", N);
        }
        MPI_Finalize();
        exit(1);
    }

    int rows_per_proc = N / size;
    int sub_A[rows_per_proc][N], sub_C[rows_per_proc][N];

    if (rank == 0) {
        printf("Enter elements for matrix A (%dx%d):\n", N, N);
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (scanf("%d", &A[i][j]) != 1) {
                    printf("Invalid input for A[%d][%d]. Exiting.\n", i, j);
                    MPI_Abort(MPI_COMM_WORLD, 1);
                }
            }
        }

        printf("Enter elements for matrix B (%dx%d):\n", N, N);
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (scanf("%d", &B[i][j]) != 1) {
                    printf("Invalid input for B[%d][%d]. Exiting.\n", i, j);
                    MPI_Abort(MPI_COMM_WORLD, 1);
                }
            }
        }
    }

    MPI_Bcast(B, N * N, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(A, rows_per_proc * N, MPI_INT, sub_A, rows_per_proc * N, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < rows_per_proc; i++) {
        multiply_row(sub_A[i], B, sub_C[i], N);
    }

    MPI_Gather(sub_C, rows_per_proc * N, MPI_INT, C, rows_per_proc * N, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("\nResult Matrix (Broadcast & Gather):\n");
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                printf("%d ", C[i][j]);
            }
            printf("\n");
        }
    }

    MPI_Finalize();
    return 0;
}