#include<stdio.h>

int main()
{
    int bt[20], p[20], wt[20], tat[20], pr[20];
    int i, j, n, temp;
    float wtavg = 0, tatavg = 0;

    printf("\nEnter the number of processes -- ");
    scanf("%d", &n);

    for(i=0;i<n;i++)
    {
        p[i] = i; // Save the process ID (0, 1, 2...)
        printf("Enter Burst Time & Priority for Process %d -- ", i);
        scanf("%d %d", &bt[i], &pr[i]);
    }

    // SIMPLIFICATION: Just sort the list by Priority first!
    // Using a simple swap (Bubble Sort)
    for(i=0;i<n;i++)
    {
        for(j=i+1;j<n;j++)
        {
            // Assuming Lower Priority Number = Higher Priority (e.g., 1 is first)
            if(pr[i] > pr[j])
            {
                // Swap Priority
                temp = pr[i]; pr[i] = pr[j]; pr[j] = temp;
                // Swap Burst Time
                temp = bt[i]; bt[i] = bt[j]; bt[j] = temp;
                // Swap Process ID (so we know which P is which)
                temp = p[i]; p[i] = p[j]; p[j] = temp;
            }
        }
    }

    // Now execute exactly like your FCFS code
    wt[0] = 0;
    tat[0] = bt[0];
    wtavg = 0;
    tatavg = tat[0];

    for(i=1;i<n;i++)
    {
        wt[i] = wt[i-1] + bt[i-1];
        tat[i] = tat[i-1] + bt[i];
        
        wtavg = wtavg + wt[i];
        tatavg = tatavg + tat[i];
    }

    printf("\t PROCESS \tPRIORITY \tBURST TIME \t WAITING TIME\t TURNAROUND TIME\n");
    for(i=0;i<n;i++)
        printf("\n\t P%d \t\t %d \t\t %d \t\t %d \t\t %d", p[i], pr[i], bt[i], wt[i], tat[i]);

    printf("\nAverage Waiting Time -- %f", wtavg/n);
    printf("\nAverage Turnaround Time -- %f", tatavg/n);
    
    return 0;
}