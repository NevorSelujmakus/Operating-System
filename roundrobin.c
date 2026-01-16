#include<stdio.h>

int main()
{
    int i, n, time = 0, quantum;
    int bt[20], rem_bt[20], wt[20], tat[20];
    int completed = 0;
    float wtavg = 0, tatavg = 0;

    printf("\nEnter the number of processes -- ");
    scanf("%d", &n);

    for(i=0;i<n;i++)
    {
        printf("\nEnter Burst Time for Process %d -- ", i);
        scanf("%d", &bt[i]);
        rem_bt[i] = bt[i]; // Copy burst time to "remaining burst time"
    }

    printf("\nEnter Time Quantum -- ");
    scanf("%d", &quantum);

    // Keep looping until all processes are completed
    while(completed < n)
    {
        for(i=0; i<n; i++)
        {
            if(rem_bt[i] > 0) // If process is not done yet
            {
                if(rem_bt[i] > quantum)
                {
                    time = time + quantum;
                    rem_bt[i] = rem_bt[i] - quantum;
                }
                else
                {
                    // Process can finish in this turn
                    time = time + rem_bt[i];
                    wt[i] = time - bt[i]; // Waiting Time = Finish Time - Original Burst Time
                    rem_bt[i] = 0;
                    completed++;
                }
            }
        }
    }

    // Calculate Turnaround Time and Averages
    printf("\t PROCESS \tBURST TIME \t WAITING TIME\t TURNAROUND TIME\n");
    for(i=0;i<n;i++)
    {
        tat[i] = wt[i] + bt[i];
        wtavg += wt[i];
        tatavg += tat[i];
        printf("\n\t P%d \t\t %d \t\t %d \t\t %d", i, bt[i], wt[i], tat[i]);
    }

    printf("\nAverage Waiting Time -- %f", wtavg/n);
    printf("\nAverage Turnaround Time -- %f", tatavg/n);
    
    return 0;
}