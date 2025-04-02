#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAX_PROCESSES 10
#define MAX_NODES 10
int localWFG[MAX_NODES][MAX_PROCESSES][MAX_PROCESSES];
int globalWFG[MAX_PROCESSES][MAX_PROCESSES];
int visited[MAX_PROCESSES];
bool dfs(int node, int numProcesses)
{
    if (visited[node] == 1)
        return true;
    if (visited[node] == 2)
        return false;
    visited[node] = 1;
    for (int i = 0; i < numProcesses; i++)
    {
        if (globalWFG[node][i] == 1)
        {
            if (dfs(i, numProcesses))
                return true;
        }
    }
    visited[node] = 2;
    return false;
}
void constructGlobalWFG(int numNodes, int numProcesses)
{
    for (int i = 0; i < numProcesses; i++)
    {
        for (int j = 0; j < numProcesses; j++)
        {
            globalWFG[i][j] = 0;
        }
    }
    for (int node = 0; node < numNodes; node++)
    {
        for (int i = 0; i < numProcesses; i++)
        {
            for (int j = 0; j < numProcesses; j++)
            {
                globalWFG[i][j] |= localWFG[node][i][j];
            }
        }
    }
}
bool detectDeadlock(int numNodes, int numProcesses)
{
    constructGlobalWFG(numNodes, numProcesses);
    for (int i = 0; i < numProcesses; i++)
    {
        visited[i] = 0;
    }
    for (int i = 0; i < numProcesses; i++)
    {
        if (visited[i] == 0)
        {
            if (dfs(i, numProcesses))
                return true;
        }
    }
    return false;
}
int main()
{
    int numNodes, numProcesses;
    printf("Enter the number of nodes: ");
    scanf("%d", &numNodes);
    printf("Enter the number of processes per node: ");
    scanf("%d", &numProcesses);
    for (int node = 0; node < numNodes; node++)
    {
        printf("Enter the Local Wait-For Graph for Node %d:\n", node + 1);
        for (int i = 0; i < numProcesses; i++)
        {
            for (int j = 0; j < numProcesses; j++)
            {
                scanf("%d", &localWFG[node][i][j]);
            }
        }
    }
    if (detectDeadlock(numNodes, numProcesses))
    {
        printf("Deadlock detected!\n");
    }
    else
    {

        printf("No deadlock detected.\n");
    }
    return 0;
}