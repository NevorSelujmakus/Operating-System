#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <numeric>
#include <queue>

using namespace std;

struct Process {
    int pid;
    int at;
    int bt;
    int tt;
    int wt;
    int ct;
    int rt; // remaining time
};

void printTable(const vector<Process>& procs, string algoName, double avgTT, double avgWT) {
    if (algoName == "RR") {
        // RR has specific formatting in prompt, but handled in calling function mostly
        // Just print table here
    } else {
        cout << algoName << ":" << endl;
    }
    
    cout << left << setw(10) << "PID" << setw(10) << "AT" << setw(10) << "BT" 
         << setw(10) << "CT" << setw(10) << "TT" << setw(10) << "WT"<< endl;
    for (const auto& p : procs) {
        cout << left << setw(10) << p.pid << setw(10) << p.at << setw(10) << p.bt 
             << setw(10) << p.ct << setw(10) << p.tt << setw(10) << p.wt << endl;
    }
    cout << algoName << " AVEtt and AVEwt" << endl;
    cout << "Average TT: " << fixed << setprecision(2) << avgTT << endl;
    cout << "Average WT: " << fixed << setprecision(2) << avgWT << endl;
    cout << endl;
}

int calculateOptimalQuantum(const vector<Process>& procs) {
    double sum = 0;
    for (const auto& p : procs) {
        sum += p.bt;
    }
    return (int)(sum / procs.size());
}

// SJF Non-preemptive (labeled SJR in prompt)
void solveSJF(vector<Process> procs) {
    int n = procs.size();
    int currentTime = 0;
    int completed = 0;
    vector<bool> isCompleted(n, false);
    
    // To maintain order of completion or similar, we update 'procs' in place.
    // However, finding the next process is what matters.
    
    // Adjust start time if no process arrives at 0
    // Find min AT
    int minAT = 1000000;
    for(auto& p : procs) minAT = min(minAT, p.at);
    if (currentTime < minAT) currentTime = minAT;

    while (completed < n) {
        int idx = -1;
        int minBT = 1000000;
        
        for (int i = 0; i < n; i++) {
            if (procs[i].at <= currentTime && !isCompleted[i]) {
                if (procs[i].bt < minBT) {
                    minBT = procs[i].bt;
                    idx = i;
                }
                else if (procs[i].bt == minBT) {
                    // Tie breaker: Arrival Time
                    if (procs[i].at < procs[idx].at) {
                        idx = i;
                    }
                }
            }
        }
        
        if (idx != -1) {
            currentTime += procs[idx].bt;
            procs[idx].ct = currentTime;
            procs[idx].tt = procs[idx].ct - procs[idx].at;
            procs[idx].wt = procs[idx].tt - procs[idx].bt;
            isCompleted[idx] = true;
            completed++;
        } else {
            currentTime++;
        }
    }
    
    double totalTT = 0, totalWT = 0;
    for(const auto& p : procs) {
        totalTT += p.tt;
        totalWT += p.wt;
    }
    
    printTable(procs, "SJR", totalTT/n, totalWT/n);
}

// SRTF Preemptive
void solveSRTF(vector<Process> procs) {
    int n = procs.size();
    int completed = 0;
    int currentTime = 0;
    for(auto& p : procs) p.rt = p.bt;
    
    int minAT = 1000000;
    for(auto& p : procs) minAT = min(minAT, p.at);
    if (currentTime < minAT) currentTime = minAT;
    
    while(completed < n) {
        int idx = -1;
        int minRT = 1000000;
        
        for(int i=0; i<n; i++) {
            if(procs[i].at <= currentTime && procs[i].rt > 0) {
                if(procs[i].rt < minRT) {
                    minRT = procs[i].rt;
                    idx = i;
                }
                 else if (procs[i].rt == minRT) {
                    if (procs[i].at < procs[idx].at) {
                        idx = i;
                    }
                }
            }
        }
        
        if(idx != -1) {
            procs[idx].rt--;
            currentTime++;
            if(procs[idx].rt == 0) {
                completed++;
                procs[idx].ct = currentTime;
                procs[idx].tt = procs[idx].ct - procs[idx].at;
                procs[idx].wt = procs[idx].tt - procs[idx].bt;
            }
        } else {
            currentTime++;
        }
    }
    
    double totalTT = 0, totalWT = 0;
    for(const auto& p : procs) {
        totalTT += p.tt;
        totalWT += p.wt;
    }
    printTable(procs, "SRTF", totalTT/n, totalWT/n);
}

// Round Robin
void solveRR(vector<Process> procs, int quantum) {
    int n = procs.size();
    for(auto& p : procs) p.rt = p.bt;

    int currentTime = 0;
    int completed = 0;
    
    vector<int> readyQueue;
    vector<bool> inQueue(n, false);
    
    // Sort indices by arrival time
    vector<int> arrivalOrder(n);
    for(int i=0; i<n; ++i) arrivalOrder[i] = i;
    sort(arrivalOrder.begin(), arrivalOrder.end(), [&](int a, int b){
        return procs[a].at < procs[b].at;
    });

    int arrivalIndex = 0;
    
    // Jump to first arrival if needed
    if (procs[arrivalOrder[0]].at > currentTime)
        currentTime = procs[arrivalOrder[0]].at;
        
    // Initial loading of queue
    while(arrivalIndex < n && procs[arrivalOrder[arrivalIndex]].at <= currentTime) {
        readyQueue.push_back(arrivalOrder[arrivalIndex]);
        arrivalIndex++;
    }

    while(completed < n) {
        if(readyQueue.empty()) {
             if(arrivalIndex < n) {
                currentTime = procs[arrivalOrder[arrivalIndex]].at;
                 while(arrivalIndex < n && procs[arrivalOrder[arrivalIndex]].at <= currentTime) {
                    readyQueue.push_back(arrivalOrder[arrivalIndex]);
                    arrivalIndex++;
                }
            }
            continue;
        }
        
        int idx = readyQueue.front();
        readyQueue.erase(readyQueue.begin());
        
        int timeSlice = min(quantum, procs[idx].rt);
        
        // Advance time
        int finishTime = currentTime + timeSlice;
        
        // Add newly arrived processes
        // Important: check arrivals up to finishTime
        while(arrivalIndex < n && procs[arrivalOrder[arrivalIndex]].at <= finishTime) {
            readyQueue.push_back(arrivalOrder[arrivalIndex]);
            arrivalIndex++;
        }
        
        procs[idx].rt -= timeSlice;
        currentTime = finishTime;

        if (procs[idx].rt == 0) {
            completed++;
            procs[idx].ct = currentTime;
            procs[idx].tt = procs[idx].ct - procs[idx].at;
            procs[idx].wt = procs[idx].tt - procs[idx].bt;
        } else {
            readyQueue.push_back(idx);
        }
    }
    
    double totalTT = 0, totalWT = 0;
    for(const auto& p : procs) {
        totalTT += p.tt;
        totalWT += p.wt;
    }
    
    cout << "RR:" << endl;
    cout << "the quantum time" << endl;
    cout << quantum << endl;
    printTable(procs, "RR", totalTT/n, totalWT/n);
}

int main() {
    // Hardcoded processes
    // PIDs assumed 1-8
    vector<Process> processes = {
        {1, 1, 53},
        {2, 3, 43},
        {3, 8, 18},
        {4, 4, 16},
        {5, 6, 24},
        {6, 7, 73},
        {7, 2, 99},
        {8, 5, 27}
    };
    
    int optimalQuantum = calculateOptimalQuantum(processes);
    cout << "the recommended quantum time" << endl;
    cout << optimalQuantum << endl << endl;
    
    solveSJF(processes);
    solveSRTF(processes);
    solveRR(processes, optimalQuantum);
    
    return 0;
}
