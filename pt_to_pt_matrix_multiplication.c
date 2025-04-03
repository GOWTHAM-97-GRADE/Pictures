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
    int A[N][N], B[N][N], C[N][N];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    

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

        for (int i = 1; i < size; i++) {
            MPI_Send(&A[i * rows_per_proc], rows_per_proc * N, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    } else {
        MPI_Recv(sub_A, rows_per_proc * N, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    MPI_Bcast(B, N * N, MPI_INT, 0, MPI_COMM_WORLD);
    
    for (int i = 0; i < rows_per_proc; i++) {
        multiply_row(sub_A[i], B, sub_C[i], N);
    }

    if (rank == 0) {
        for (int i = 0; i < rows_per_proc; i++) {
            for (int j = 0; j < N; j++) {
                C[i][j] = sub_C[i][j];
            }
        }

        for (int i = 1; i < size; i++) {
            MPI_Recv(&C[i * rows_per_proc], rows_per_proc * N, MPI_INT, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        printf("\nResult Matrix C (point-to-point):\n");
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                printf("%d ", C[i][j]);
            }
            printf("\n");
        }
    } else {
        MPI_Send(sub_C, rows_per_proc * N, MPI_INT, 0, 1, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}