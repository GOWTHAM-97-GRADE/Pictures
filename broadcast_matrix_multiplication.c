#include <stdio.h>
#include <mpi.h>
#include <string.h>
#include <stdlib.h>

#define SIZE 3  // 3x3 Matrix

void printMatrix(int mat[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%d\t", mat[i][j]);
        }
        printf("\n");
    }
}

// Function to get valid integer input
void getMatrixInput(int matrix[SIZE][SIZE], char name) {
    printf("Enter values for Matrix %c (%dx%d):\n", name, SIZE, SIZE);
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            while (scanf("%d", &matrix[i][j]) != 1) {
                printf("Invalid input. Please enter an integer: ");
                while (getchar() != '\n'); // Clear input buffer
            }
        }
    }
}

int main(int argc, char **argv) {
    int rank, size;
    int A[SIZE][SIZE], B[SIZE][SIZE];
    int C[SIZE][SIZE] = {0};  // Result Matrix

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Calculate number of rows each process should handle
    int rowsPerProcess = SIZE / size;
    int leftoverRows = SIZE % size;

    // Compute startRow and endRow for each process
    int startRow = rank * rowsPerProcess + (rank < leftoverRows ? rank : leftoverRows);
    int endRow = startRow + rowsPerProcess + (rank < leftoverRows ? 1 : 0);
    int numRows = endRow - startRow; // Number of rows assigned to this process

    // Master process takes user input for matrices A and B
    if (rank == 0) {
        getMatrixInput(A, 'A');
        getMatrixInput(B, 'B');
    }

    // Broadcast matrices A and B to all processes
    MPI_Bcast(A, SIZE * SIZE, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(B, SIZE * SIZE, MPI_INT, 0, MPI_COMM_WORLD);

    // Each process computes its assigned rows
    int myResult[numRows][SIZE];  // Local result matrix
    memset(myResult, 0, sizeof(myResult));

    for (int i = startRow; i < endRow; i++) {
        for (int j = 0; j < SIZE; j++) {
            for (int k = 0; k < SIZE; k++) {
                myResult[i - startRow][j] += A[i][k] * B[k][j];
            }
        }
    }

    // Prepare arrays for MPI_Gatherv
    int sendcounts[size], displs[size];
    if (rank == 0) {
        for (int i = 0, pos = 0; i < size; i++) {
            int rStart = i * rowsPerProcess + (i < leftoverRows ? i : leftoverRows);
            int rEnd = rStart + rowsPerProcess + (i < leftoverRows ? 1 : 0);
            sendcounts[i] = (rEnd - rStart) * SIZE;
            displs[i] = pos;
            pos += sendcounts[i];
        }
    }

    // Gather the results correctly from all processes
    MPI_Gatherv(myResult, numRows * SIZE, MPI_INT, C, sendcounts, displs, MPI_INT, 0, MPI_COMM_WORLD);

    // Master process prints the result
    if (rank == 0) {
        printf("\nMatrix A:\n");
        printMatrix(A);

        printf("\nMatrix B:\n");
        printMatrix(B);

        printf("\nMatrix C (Result of A * B):\n");
        printMatrix(C);
    }

    MPI_Finalize();
    return 0;
}
