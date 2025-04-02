#include <stdio.h>
#define n 5
int dfs(int process, int adj[n][n], int visited[n], int stack[n]) {
  visited[process] = 1;
  stack[process] = 1;
  for (int i = 0; i < n; i++) {
    if (adj[process][i] == 1) {
      if (!visited[i] && dfs(i, adj, visited, stack)) {
        return 1;
      } else if (stack[i]) {
        return 1;
      }
    }
  }
  stack[process] = 0;
  return 0;
}
int cycledetect(int adj[n][n]) {
  int visited[n] = {0};
  int stack[n] = {0};
  for (int i = 0; i < n; i++) {
    if (!visited[i]) {
      if (dfs(i, adj, visited, stack)) {
        return 1;
      }
    }
  }
  return 0;
}
int main() {
  int adj[n][n] = {0};
  printf("Add dependecy between processes\n");
  printf("Enter -1 -1 to stop\n");
  while (1) {
    int from, to;
    scanf("%d %d", &from, &to);
    if (from == -1 && to == -1) {
      break;
    }
    if (from >= 0 && from < n && to >= 0 && to < n) {
      adj[from][to] = 1;
      printf("Process %d->%d added\n", from, to);
    } else {
      printf("Invalid input\n");
    }
  }
  printf("Adjacency matrix\n");
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      printf("%d", adj[i][j]);
    }
    printf("\n");
  }
  if (cycledetect(adj)) {
    printf("Deadlock detected\n");
  } else {
    printf("No deadlock detected\n");
  }
}
