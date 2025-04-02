#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAX 100
int status[MAX],n,coordinator;
void display()
{
    printf("*****PROCESSES*****\n");
    for(int i=1;i<=n;i++)
    {
        printf("Process:%d\n",i);
    }
    printf("*****STATUS*****\n");
    for(int i=1;i<=n;i++)
    {
        if(status[i])
        {
            printf("Process %d is alive\n",i);
        }
        else
        {
            printf("Process %d is dead\n",i);
        }
    }
    printf("Current Coordinator:%d\n",coordinator);
}
void bully()
{
    int choice,process;
    while(true)
    {
        printf("*****MENU DRIVEN BULLY ALGORITHM*****\n");
        printf("Enter\n");
        printf("1.Crash a process\n2.Activate a process\n3.Display the status of all processes \n4.Exit\n");
        scanf("%d",&choice);
        switch(choice)
        {
            case 1:
            printf("Enter the process to crash:");
            scanf("%d",&process);
            if(!status[process])
            {
                printf("Process %d is already dead.\n",process);
            }
            else
            {
                status[process]=0;
                printf("Process %d is now crashed\n",process);
            }
            if(process==coordinator)
            {
                    printf("Coordinator Crashed.Initiating election...\n");
                    status[coordinator]=0;
                    coordinator=0;
                    for(int i=n;i>=1;i--)
                    {
                        if(status[i])
                        {
                            coordinator=i;
                            break;
                        }
                    }
                    printf("New Coordinator:%d\n",coordinator);
            }
            break;
            case 2:
            printf("Enter a process to activate:");
            scanf("%d",&process);
            if(status[process])
            {
                printf("Process %d is already alive.\n",process);
            }
            else
            {
                status[process]=1;
                printf("Process %d is now activated.\n",process);
            }
            if(process>coordinator)
            {
                coordinator=process;
            }
            printf("New Coordinator:%d\n",coordinator);
            break;
            case 3:
            display();
            break;
            case 4:
            exit(0);
            default:
            printf("Invalid choice!\n");
        }
    }
}
int main()
{
    printf("Enter no. of processes:");
    scanf("%d",&n);
    printf("Enter status of all processes(0->Dead , 1->Alive):\n");
    for(int i=1;i<=n;i++)
    {
        printf("Process:%d\n",i);
        scanf("%d",&status[i]);
    }
    for(int i=1;i<=n;i++)
    {
        if(status[i])
        {
            coordinator=i;
        }
    }
    printf("Initial Coordinator:%d\n",coordinator);
    bully();
}
