#include <stdio.h>
#include <stdbool.h>
#define MAX_PROCESSES 10
int waitForGraph[MAX_PROCESSES][MAX_PROCESSES];
bool visited[MAX_PROCESSES];
bool sendProbe(int initiator, int current, int numProcesses)
{
    if (current == initiator && visited[current])
    {
        return true; // Cycle detected
    }
    visited[current] = true;
    for (int i = 0; i < numProcesses; i++)
    {
        if (waitForGraph[current][i] == 1)
        { // Check dependency
            if (!visited[i])
            {
                if (sendProbe(initiator, i, numProcesses))
                {
                    return true;
                }
            }
            else if (i == initiator)
            {
                return true; // Cycle detected when returning to the initiator
            }
        }
    }
    return false;
}
bool detectDeadlock(int numProcesses)
{
    for (int i = 0; i < numProcesses; i++)
    {
        for (int j = 0; j < numProcesses; j++)
        {
            visited[j] = false; // Reset visited array for each initiator
        }
        if (sendProbe(i, i, numProcesses))
        {
            return true; // Deadlock detected
        }
    }
    return false; // No deadlock detected
}
int main()
{
    int numProcesses;

    printf("Enter the number of processes: ");
    scanf("%d", &numProcesses);
    printf("Enter the Wait-For Graph as an adjacency matrix:\n");
    for (int i = 0; i < numProcesses; i++)
    {
        for (int j = 0; j < numProcesses; j++)
        {
            scanf("%d", &waitForGraph[i][j]);
        }
    }
    if (detectDeadlock(numProcesses))
    {
        printf("Deadlock detected!\n");
    }
    else
    {
        printf("No deadlock detected.\n");
    }
    return 0;
}