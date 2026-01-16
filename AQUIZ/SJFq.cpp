#include <iostream>
#include <iomanip>

using namespace std;

int main() {
    int n = 6;
    int id[] = {0, 1, 2, 3, 4, 5};
    double arrivalTime[] = {0.0, 1.0, 2.0, 3.0, 4.0, 5.0};
    double burstTime[] = {99.99, 107.9, 143.77, 20.24, 50.55, 66.66};
    double waitingTime[6];
    double turnAroundTime[6];
    bool completed[] = {false, false, false, false, false, false};

    int completedCount = 0;
    double currentTime = 0;

    while (completedCount < n) {
        int idx = -1;
        double minBurst = 1e18; // Large number
        bool found = false;

        // Find process with min burst time among arrived and not completed
        for (int i = 0; i < n; i++) {
            if (!completed[i] && arrivalTime[i] <= currentTime) {
                if (burstTime[i] < minBurst) {
                    minBurst = burstTime[i];
                    idx = i;
                    found = true;
                }
            }
        }

        if (found) {
            // Execute process
            waitingTime[idx] = currentTime - arrivalTime[idx];
            turnAroundTime[idx] = waitingTime[idx] + burstTime[idx];
            
            // Completion time would be currentTime + burstTime
            currentTime += burstTime[idx];
            
            completed[idx] = true;
            completedCount++;
        } else {
            // Advance time to next arrival if CPU is idle
            double nextArrival = 1e18;
            for (int i = 0; i < n; i++) {
                if (!completed[i] && arrivalTime[i] > currentTime) {
                    if (arrivalTime[i] < nextArrival) {
                        nextArrival = arrivalTime[i];
                    }
                }
            }
            if (nextArrival < 1e18) {
                currentTime = nextArrival;
            }
        }
    }

    double totalWT = 0;
    double totalTAT = 0;

    cout << "\n\t PROCESS \tARRIVAL TIME \tBURST TIME \t WAITING TIME\t TURNAROUND TIME\n";
    cout << fixed << setprecision(2);

    for (int i = 0; i < n; i++) {
        totalWT += waitingTime[i];
        totalTAT += turnAroundTime[i];
        cout << "\t P" << id[i] << " \t\t " << arrivalTime[i] << " \t\t " << burstTime[i] 
             << " \t\t " << waitingTime[i] << " \t\t " << turnAroundTime[i] << endl;
    }

    cout << "\nAverage Waiting Time -- " << totalWT / n << endl;
    cout << "\nAverage Turnaround Time -- " << totalTAT / n << endl;
    
    cin.get();
    return 0;
}
