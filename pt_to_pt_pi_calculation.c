#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include <time.h>

#define NUM_POINTS 1000000

// Better random number generator
double random_double() {
    return (double)rand() / (double)RAND_MAX;
}

int main(int argc, char **argv) {
    int rank, size;
    int local_count = 0;
    long total_count = 0;  // Using long to handle larger numbers
    double start_time, end_time;

    // Initialize MPI
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Seed random number generator (unique for each process)
    srand(time(NULL) + rank * 1000);  // Better seed separation

    // Record start time on rank 0
    if (rank == 0) {
        start_time = MPI_Wtime();
    }

    // Monte Carlo method: count points inside the quarter-circle
    for (int i = 0; i < NUM_POINTS; i++) {
        double x = random_double();
        double y = random_double();
        if ((x * x + y * y) <= 1.0) {
            local_count++;
        }
    }

    if (rank == 0) {
        total_count = local_count;

        // Receive results from worker processes
        for (int i = 1; i < size; i++) {
            int received_count;
            MPI_Recv(&received_count, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total_count += received_count;
        }

        // Compute π estimate using double for precision
        double total_points = (double)NUM_POINTS * size;
        double pi_estimate = 4.0 * total_count / total_points;

        // Get end time and calculate duration
        end_time = MPI_Wtime();

        printf("Number of processes: %d\n", size);
        printf("Total points: %.0f\n", total_points);
        printf("Points inside circle: %ld\n", total_count);
        printf("Estimated Pi = %.10f\n", pi_estimate);
        printf("Actual Pi    = %.10f\n", M_PI);
        printf("Error        = %.10f\n", fabs(pi_estimate - M_PI));
        printf("Time taken   = %.6f seconds\n", end_time - start_time);
    } else {
        // Worker processes send their local count to rank 0
        MPI_Send(&local_count, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
