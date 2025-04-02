#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

#define TOTAL_POINTS 1000000  // Number of random points

int main(int argc, char **argv)
{
    int rank, size, points_per_process, i, count = 0, global_count;
    double x, y, pi;

    // Initialize MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Master process prints info
    if (rank == 0) {
        printf("Using %d processes to estimate PI with %d points\n", size, TOTAL_POINTS);
    }

    // Broadcast total points to all processes
    int total_points = TOTAL_POINTS;
    MPI_Bcast(&total_points, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Calculate number of points per process
    points_per_process = total_points / size;

    // Seed random number generator uniquely for each process
    srand(rank + 1);

    // Monte Carlo simulation: Generate random points and count those inside the circle
    for (i = 0; i < points_per_process; i++) {
        x = (double)rand() / RAND_MAX;
        y = (double)rand() / RAND_MAX;
        if ((x * x + y * y) <= 1.0) {
            count++;
        }
    }

    // Reduce counts from all processes to get the total count inside the circle
    MPI_Reduce(&count, &global_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // Master process calculates and prints estimated PI value
    if (rank == 0) {
        pi = 4.0 * (double)global_count / total_points;
        printf("Estimated PI = %lf\n", pi);
    }

    // Finalize MPI
    MPI_Finalize();
    return 0;
}
