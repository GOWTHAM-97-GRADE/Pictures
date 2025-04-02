#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAX 100

int status[MAX], n, coordinator;

// Display function with improved formatting
void display() {
    printf("\n***** Process Status *****\n");
    for(int i = 1; i <= n; i++) {
        printf("Process %d - Status: %s\n", i, status[i] ? "Alive" : "Dead");
    }
    printf("Current Coordinator: %d\n", coordinator);
}

// Ring election algorithm with proper message passing
void ring_election(int initiator) {
    if (n == 0) {
        printf("Error: No processes available for election.\n");
        return;
    }

    // Find if initiator exists and is active
    if (!status[initiator]) {
        printf("Error: Initiator process %d is dead.\n", initiator);
        return;
    }

    printf("\nProcess %d initiates election...\n", initiator);
    int max_id = initiator;
    int current = initiator;
    bool full_circle = false;

    // First pass: from initiator to end
    for(int i = current + 1; i <= n; i++) {
        if(status[i]) {
            printf("Process %d sends election message to Process %d\n", current, i);
            if(i > max_id) {
                max_id = i;
            }
            current = i;
        }
    }

    // Second pass: from start to initiator
    for(int i = 1; i < initiator; i++) {
        if(status[i]) {
            printf("Process %d sends election message to Process %d\n", current, i);
            if(i > max_id) {
                max_id = i;
            }
            current = i;
        }
    }

    // Check if we have a valid coordinator
    if(max_id != initiator || status[max_id]) {
        coordinator = max_id;
        printf("Process %d is elected as the new coordinator!\n", coordinator);
    } else {
        printf("No active processes found for election.\n");
    }
}

// Main algorithm with menu
void ring_algo() {
    int choice, process;
    printf("\n***** RING ELECTION ALGORITHM *****\n");
    
    while(true) {
        printf("\nMenu:\n");
        printf("1. Crash a Process\n");
        printf("2. Activate a Process\n");
        printf("3. Start Election\n");
        printf("4. Display Status\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: // Crash a process
                printf("Enter process to crash (1-%d): ", n);
                scanf("%d", &process);
                if(process < 1 || process > n) {
                    printf("Invalid process ID.\n");
                } else if(!status[process]) {
                    printf("Process %d is already dead.\n", process);
                } else {
                    status[process] = 0;
                    printf("Process %d has crashed.\n", process);
                    if(process == coordinator) {
                        // Find next active process
                        int next = (process % n) + 1;
                        while(!status[next] && next != process) {
                            next = (next % n) + 1;
                        }
                        ring_election(next);
                    }
                }
                break;

            case 2: // Activate a process
                printf("Enter process to activate (1-%d): ", n);
                scanf("%d", &process);
                if(process < 1 || process > n) {
                    printf("Invalid process ID.\n");
                } else if(status[process]) {
                    printf("Process %d is already alive.\n", process);
                } else {
                    status[process] = 1;
                    printf("Process %d is now activated.\n", process);
                    // If newly activated process has higher ID, start election
                    if(process > coordinator) {
                        ring_election(process);
                    }
                }
                break;

            case 3: // Manual election
                printf("Enter process to initiate election (1-%d): ", n);
                scanf("%d", &process);
                if(process < 1 || process > n) {
                    printf("Invalid process ID.\n");
                } else {
                    ring_election(process);
                }
                break;

            case 4: // Display status
                display();
                break;

            case 5: // Exit
                printf("Exiting program.\n");
                exit(0);

            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

int main() {
    printf("Enter number of processes: ");
    scanf("%d", &n);
    
    if(n <= 0 || n > MAX) {
        printf("Invalid number of processes. Must be between 1 and %d.\n", MAX);
        return 1;
    }

    printf("Enter status of all processes (0 = dead, 1 = alive):\n");
    coordinator = 0;
    for(int i = 1; i <= n; i++) {
        printf("Process %d: ", i);
        scanf("%d", &status[i]);
        if(status[i] && i > coordinator) {
            coordinator = i;
        }
    }

    if(coordinator == 0) {
        printf("No active processes found. Starting with no coordinator.\n");
    } else {
        printf("Initial coordinator: %d\n", coordinator);
    }

    ring_algo();
    return 0;
}