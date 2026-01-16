#include <iostream>

using namespace std;

int main()
{
    int n = 10;
    int bt[10] = {9, 8, 7, 8, 7, 8, 2, 1, 2, 5};
    int at[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int wt[10], tat[10];
    float wtavg = 0, tatavg = 0;
    int currentTime = 0;

    cout << "\t PROCESS \t ARRIVAL TIME \t BURST TIME \t WAITING TIME \t TURNAROUND TIME" << endl;

    
    
    for(int i = 0; i < n; i++)
    {
        // If the CPU is idle until the process arrives
        if (currentTime < at[i]) {
            currentTime = at[i];
        }

        // Waiting time = Current time - Arrival time
        wt[i] = currentTime - at[i];
        
        // Turnaround time = Waiting time + Burst time
        tat[i] = wt[i] + bt[i];
        
        // Update current time (Completion time)
        currentTime += bt[i];

        // Accumulate averages
        wtavg += wt[i];
        tatavg += tat[i];

        cout << "\n\t P" << i << " \t\t " << at[i] << " \t\t " << bt[i] << " \t\t " << wt[i] << " \t\t " << tat[i];
    }

    cout << "\n\nAverage Waiting Time -- " << wtavg/n;
    cout << "\nAverage Turnaround Time -- " << tatavg/n << endl;
    
    cin.get(); 

    return 0;
}