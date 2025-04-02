#include <stdio.h>
#include <mpi.h>
#define SIZE 3

void printMatrix(int mat[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%d\t", mat[i][j]);
        }
        printf("\n");
    }
}

int inputMatrix(int mat[SIZE][SIZE], const char* name) {
    printf("Enter elements for %s (%dx%d matrix):\n", name, SIZE, SIZE);
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%s[%d][%d] = ", name, i, j);
            if (scanf("%d", &mat[i][j]) != 1) {
                printf("Error: Invalid input\n");
                return 1;
            }
        }
    }
    return 0;
}

int main(int argc, char **argv) {
    int rank, size;
    int A[SIZE][SIZE];
    int B[SIZE][SIZE];
    int C[SIZE][SIZE] = {0};

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2) {
        if (rank == 0) {
            printf("Error: Program needs at least 2 processes\n");
        }
        MPI_Finalize();
        return 1;
    }

    if (rank == 0) {
        // Get user input for matrices
        if (inputMatrix(A, "Matrix A") || inputMatrix(B, "Matrix B")) {
            MPI_Finalize();
            return 1;
        }

        // Send data to worker processes
        for (int i = 1; i < size && i <= SIZE; i++) {
            MPI_Send(A[i-1], SIZE, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(B, SIZE * SIZE, MPI_INT, i, 0, MPI_COMM_WORLD);
        }

        // Receive results
        for (int i = 1; i < size && i <= SIZE; i++) {
            MPI_Recv(C[i-1], SIZE, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        // Print results
        printf("\nMatrix A:\n");
        printMatrix(A);
        printf("\nMatrix B:\n");
        printMatrix(B);
        printf("\nMatrix C (Result of A * B):\n");
        printMatrix(C);
    }
    else if (rank <= SIZE) {  // Worker processes (only up to rank 3 needed)
        int myRow[SIZE];
        int myResult[SIZE] = {0};
        int B_local[SIZE][SIZE];

        MPI_Recv(myRow, SIZE, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(B_local, SIZE * SIZE, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // Perform multiplication for this row
        for (int j = 0; j < SIZE; j++) {
            for (int k = 0; k < SIZE; k++) {
                myResult[j] += myRow[k] * B_local[k][j];
            }
        }

        MPI_Send(myResult, SIZE, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}