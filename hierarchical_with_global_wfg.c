#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_PROCESSES 50
#define MAX_LEVELS 10

typedef struct
{
    int graph[MAX_PROCESSES][MAX_PROCESSES];
    int num_processes;
}waitforgraph;

waitforgraph globalWFG;
int globalProcesses=0;
void InitializeGraph(waitforgraph *wfg,int num_processes)
{
    wfg->num_processes=num_processes;
    for(int i=0;i<num_processes;i++)
    {
        for(int j=0;j<num_processes;j++)
        {
            wfg->graph[i][j]=0;
        }
    }
}
void AddEdge(waitforgraph *wfg,int from,int to)
{
    wfg->graph[from][to]=1;
}
bool DetectCycleUtil(waitforgraph *wfg,int process,bool *visited,bool *recStack)
{
    if(!visited[process])
    {
        visited[process]=true;
        recStack[process]=true;
    }
    for(int i=0;i<wfg->num_processes;i++)
    {
        if(wfg->graph[process][i])
        {
            if(!visited[i]  && DetectCycleUtil(wfg,i,visited,recStack))
            {
                return true;
            }
            else if(recStack[i])
            {
                return true;
            }
        }
    }
    recStack[process]=false;
    return false;
}
bool DetectCycle(waitforgraph *wfg)
{
    bool visited[MAX_PROCESSES]={false};
    bool recStack[MAX_PROCESSES]={false};
    for(int i=0;i<wfg->num_processes;i++)
    {
        if(DetectCycleUtil(wfg,i,visited,recStack))
        {
            return true;
        }
    }
    return false;
}
void display(waitforgraph *wfg)
{
    printf("Wait for graph:\n");
    for(int i=0;i<wfg->num_processes;i++)
    {
        printf("P%d -> ",i);
        for(int j=0;j<wfg->num_processes;j++)
        {
            if(wfg->graph[i][j])
            {
                printf("P%d ",j);
            }
        }
        printf("\n");
    }
}
int main()
{
    int num_levels;
    printf("Enter the no. of levels:");
    scanf("%d",&num_levels);
    if(num_levels<=0 || num_levels>MAX_LEVELS)
    {
        printf("Invalid no. of levels entered.Exiting\n");
        exit(1);
    }
    printf("Enter the total no. of processes in system:");
    scanf("%d",&globalProcesses);
    InitializeGraph(&globalWFG,globalProcesses);
    int processOffset=0;
    for(int i=0;i<num_levels;i++)
    {
        if(processOffset>globalProcesses)
        {
            break;
        }
        waitforgraph wfg;
        printf("Level:%d\n",i+1);
        int num_processes,num_edges;
        printf("Enter the no. of processes for level %d",i+1);
        scanf("%d",&num_processes);
        if(num_processes<=0 || num_processes>MAX_PROCESSES)
        {
            printf("Invalid no. of processes!Skipping level %d...\n",i+1);
            continue;
        }
        InitializeGraph(&wfg,num_processes);
        printf("Enter the no. of edges:");
        scanf("%d",&num_edges);
        printf("Enter the edge in the format(from,to)\n");
        for(int i=0;i<num_edges;i++)
        {
            int from,to;
            scanf("%d %d",&from,&to);
            if(from<0 || from>=num_processes || to<0 || to>=num_processes)
            {
                printf("Invalid edge entered!Skipping..\n");
                continue;
            }
            AddEdge(&wfg,from,to);
            AddEdge(&globalWFG,from+processOffset,to+processOffset);
        }
        display(&wfg);
        if(DetectCycle(&wfg))
        {
            printf("Deadlock detected in level %d\n",i+1);
        }
        else
        {
            printf("No Deadlock detected in level %d\n",i+1);
        }
        processOffset+=num_processes;

    }
    globalWFG.num_processes=globalProcesses;
    display(&globalWFG);
    if(DetectCycle(&globalWFG))
    {
        printf("-----GLOBAL DEADLOCK DETECTED!-----\n");
    }
    else
    {
        printf("-----NO GLOBAL DEADLOCK DETECTED.-----\n");
    }

}
