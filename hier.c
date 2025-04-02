#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX_PROCESSES 10
#define MAX_REGIONS 3

bool detect_cycle_dfs(int graph[MAX_PROCESSES][MAX_PROCESSES],
                      int num_processes, int node, int visited[], int stack[]) {
  visited[node] = 1;
  stack[node] = 1;

  for (int i = 0; i < num_processes; i++) {
    if (graph[node][i]) {
      if (!visited[i] &&
          detect_cycle_dfs(graph, num_processes, i, visited, stack)) {
        return true;
      } else if (stack[i]) {
        return true;
      }
    }
  }
  stack[node] = 0;
  return false;
}

bool detect_deadlock(int graph[MAX_PROCESSES][MAX_PROCESSES],
                     int num_processes) {
  int visited[MAX_PROCESSES] = {0};
  int stack[MAX_PROCESSES] = {0};

  for (int i = 0; i < num_processes; i++) {
    if (!visited[i] &&
        detect_cycle_dfs(graph, num_processes, i, visited, stack)) {
      return true;
    }
  }
  return false;
}

void hierarchical_deadlock_detection(
    int regions[MAX_REGIONS][MAX_PROCESSES][MAX_PROCESSES], int num_processes[],
    int num_regions) {
  int global_wfg[MAX_PROCESSES][MAX_PROCESSES] = {0};
  printf("----- Hierarchical Deadlock Detection -----\n");

  for (int i = 0; i < num_regions; i++) {
    printf("Checking for deadlocks in Region %d...\n", i);
    for (int u = 0; u < num_processes[i]; u++) {
      for (int v = 0; v < num_processes[i]; v++) {
        if (regions[i][u][v]) {
          global_wfg[u][v] = 1;
        }
      }
    }
    if (detect_deadlock(regions[i], num_processes[i])) {
      printf("Deadlock detected in Region %d! Escalating unresolved graph to "
             "global coordinator...\n",
             i);
    } else {
      printf("No deadlocks in Region %d.\n", i);
    }
  }
  printf("Checking global WFG for system-wide deadlocks...\n");
  if (detect_deadlock(global_wfg, MAX_PROCESSES)) {
    printf(
        "System-wide deadlock detected! Taking global resolution actions...\n");
  } else {
    printf("No system-wide deadlock detected.\n");
  }
}

int main() {
  int regions[MAX_REGIONS][MAX_PROCESSES][MAX_PROCESSES] = {{{0}}};
  int num_processes[MAX_REGIONS];
  int num_regions;
  printf("Enter the number of regions: ");
  scanf("%d", &num_regions);
  for (int i = 0; i < num_regions; i++) {
    printf("Enter the number of processes in Region %d: ", i);
    scanf("%d", &num_processes[i]);

    printf("Enter the adjacency matrix for Region %d (size %dx%d):\n", i,
           num_processes[i], num_processes[i]);
    for (int u = 0; u < num_processes[i]; u++) {
      for (int v = 0; v < num_processes[i]; v++) {
        scanf("%d", &regions[i][u][v]);
      }
    }
  }
  hierarchical_deadlock_detection(regions, num_processes, num_regions);
  return 0;
}
