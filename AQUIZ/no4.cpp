#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

int main()
{
    int n = 6;
    // Burst times from requirements
    double bt[] = {99.99, 107.9, 143.77, 20.24, 50.55, 66.66};
    double at[] = {0, 0, 0, 0, 0, 0}; 
    double rem_bt[6];
    double wt[6];
    double tat[6];
    double quantum = 25;
    double currentTime = 0;
    
    // To store execution order for Gantt chart
    vector<int> gantt_processes;
    vector<double> gantt_timestamps;

    // Initialize remaining burst time
    for(int i = 0; i < n; i++) {
        rem_bt[i] = bt[i];
    }

    gantt_timestamps.push_back(0);

    bool done = false;
    while(!done) {
        done = true;
        for(int i = 0; i < n; i++) {
            if(rem_bt[i] > 0) {
                done = false; // There is a pending process

                gantt_processes.push_back(i);

                if(rem_bt[i] > quantum) {
                    currentTime += quantum;
                    rem_bt[i] -= quantum;
                } else {
                    currentTime += rem_bt[i];
                    // Turnaround Time = Completion Time - Arrival Time (0)
                    tat[i] = currentTime; 
                    rem_bt[i] = 0;
                }
                gantt_timestamps.push_back(currentTime);
            }
        }
    }

    // Calculate Waiting Time
    for(int i = 0; i < n; i++) {
        wt[i] = tat[i] - bt[i];
    }

    // Averages
    double wtavg = 0, tatavg = 0;
    for(int i = 0; i < n; i++) {
        wtavg += wt[i];
        tatavg += tat[i];
    }

    // Output Table
    cout << "\t PROCESS \t ARRIVAL TIME \t BURST TIME \t WAITING TIME \t TURNAROUND TIME" << endl;
    cout << fixed << setprecision(2);

    for(int i = 0; i < n; i++)
    {
        cout << "\n\t P" << i << " \t\t " << at[i] << " \t\t " << bt[i] << " \t\t " << wt[i] << " \t\t " << tat[i];
    }

    cout << "\n\nAverage Waiting Time -- " << wtavg/n;
    cout << "\nAverage Turnaround Time -- " << tatavg/n << endl;

    // Gantt Chart
    cout << "\n\n\t\t GANTT CHART" << endl;
    
    // Print timestamps and processes in a simple list format if graphical is hard, 
    // but let's try a simple timeline.
    for(size_t i = 0; i < gantt_processes.size(); i++) {
        cout << "| P" << gantt_processes[i] << " ";
    }
    cout << "|" << endl;

    cout << "0";
    for(size_t i = 1; i < gantt_timestamps.size(); i++) {
        cout << "\t" << gantt_timestamps[i];
    }
    cout << endl;

    cin.get(); 

    return 0;
}
