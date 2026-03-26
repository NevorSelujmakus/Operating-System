#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <numeric>

using namespace std;

struct Process {
    int pid;
    int at; // Arrival Time
    int bt; // Burst Time
    int ct; // Completion Time
    int tt; // Turnaround Time
    int wt; // Waiting Time
    int remaining_bt; // For SRTF/RR
    int start_time; // To verify
};

// Function to print table
void printTable(const vector<Process>& processes, string algorithmName) {
    cout << algorithmName << ":" << endl;
    cout << "PID\tAT\tBT\tCT\tTT\tWT" << endl;
    double total_tt = 0;
    double total_wt = 0;

    // Print sorted by PID for consistency or by completion? usually table is by PID or execution order
    // The prompt says "(Table for SJR with the columns of PID, AT, BT, CT, TT, WT)"
    // I'll sort by PID for readability.
    vector<Process> sorted_p = processes;
    sort(sorted_p.begin(), sorted_p.end(), [](const Process& a, const Process& b) {
        return a.pid < b.pid;
    });

    for (const auto& p : sorted_p) {
        cout << p.pid << "\t" << p.at << "\t" << p.bt << "\t" << p.ct << "\t" << p.tt << "\t" << p.wt << endl;
        total_tt += p.tt;
        total_wt += p.wt;
    }
    
    cout << fixed << setprecision(2); // Assuming 2 decimal places for averages? Prompt doesn't specify precision but "AVEtt" usually implies float.
    cout << algorithmName << " AVEtt: " << (total_tt / processes.size()) << endl;
    cout << algorithmName << " AVEwt: " << (total_wt / processes.size()) << endl;
    cout << endl;
}

int calculateRecommendedQuantum(const vector<Process>& processes) {
    vector<int> bts;
    for(const auto& p : processes) {
        bts.push_back(p.bt);
    }
    sort(bts.begin(), bts.end());
    int n = bts.size();
    if (n % 2 == 0) {
        return (bts[n/2 - 1] + bts[n/2]) / 2;
    } else {
        return bts[n/2];
    }
}

void calculateSJF(vector<Process> processes) {
    int n = processes.size();
    int current_time = 0;
    int completed = 0;
    vector<bool> is_completed(n, false);
    
    // Sort by Arrival Time initially to handle idle time correctly? No, iterate.
    // Or just sort by PID for easier access? No.
    // We need to pick process with min BT among arrived.

    // Let's create a result vector
    vector<Process> result_processes = processes;
    // Map pid to index in result_processes
    // Actually, working with result_processes directly is tricky if we reorder.
    // We'll keep them in original order and use is_completed array.

    // Find min AT to start current_time (optional, usually start at 0)
    // current_time = 0; 
    
    while(completed < n) {
        int idx = -1;
        int min_bt = 1e9;
        
        for(int i=0; i<n; ++i) {
            if(!is_completed[i] && result_processes[i].at <= current_time) {
                if(result_processes[i].bt < min_bt) {
                    min_bt = result_processes[i].bt;
                    idx = i;
                }
                // Tie breaker: FCFS (Arrival Time) - already implicitly handled if we iterate by index (input order) and use <.
                // However, input order isn't AT sorted.
                // If tie in BT, pick one with earlier AT?
                // Standard SJF tie-breaker is FCFS.
                else if(result_processes[i].bt == min_bt) {
                    if (result_processes[i].at < result_processes[idx].at) {
                        idx = i;
                    }
                }
            }
        }
        
        if(idx != -1) {
            // Process found
            current_time += result_processes[idx].bt;
            result_processes[idx].ct = current_time;
            result_processes[idx].tt = result_processes[idx].ct - result_processes[idx].at;
            result_processes[idx].wt = result_processes[idx].tt - result_processes[idx].bt;
            is_completed[idx] = true;
            completed++;
        } else {
            current_time++;
        }
    }
    
    printTable(result_processes, "SJR");
}

void calculateSRTF(vector<Process> processes) {
    int n = processes.size();
    int current_time = 0;
    int completed = 0;
    // Reset remaining times
    for(auto& p : processes) p.remaining_bt = p.bt;
    
    // Determine min AT
    int min_at = 1e9;
    for(const auto& p : processes) min_at = min(min_at, p.at);
    // current_time = 0; // Standard. If first process arrives at 1, 0-1 is idle.
    
    while(completed < n) {
        int idx = -1;
        int min_rem = 1e9;
        
        for(int i=0; i<n; ++i) {
            if(processes[i].at <= current_time && processes[i].remaining_bt > 0) {
                if(processes[i].remaining_bt < min_rem) {
                    min_rem = processes[i].remaining_bt;
                    idx = i;
                } else if (processes[i].remaining_bt == min_rem) {
                     if (processes[i].at < processes[idx].at) {
                        idx = i;
                    }
                }
            }
        }
        
        if(idx != -1) {
            processes[idx].remaining_bt--;
            current_time++;
            if(processes[idx].remaining_bt == 0) {
                processes[idx].ct = current_time;
                processes[idx].tt = processes[idx].ct - processes[idx].at;
                processes[idx].wt = processes[idx].tt - processes[idx].bt;
                completed++;
            }
        } else {
            current_time++;
        }
    }
    
    printTable(processes, "SRTF");
}

void calculateRR(vector<Process> processes, int quantum) {
    int n = processes.size();
    int current_time = 0;
    int completed = 0;
    
    // Sort by AT first for initial queue population
    vector<int> p_indices(n);
    iota(p_indices.begin(), p_indices.end(), 0);
    sort(p_indices.begin(), p_indices.end(), [&](int a, int b) {
        if(processes[a].at != processes[b].at)
            return processes[a].at < processes[b].at;
        return processes[a].pid < processes[b].pid;
    });
    
    vector<int> queue;
    vector<bool> in_queue(n, false);
    
    // Push first arrived process(es)
    // Actually logic needs to handle time progression.
    
    // Re-implementing typical RR simulation
    // queue stores indices
    
    // Add processes that have arrived at time 0
    int i = 0; // index in sorted p_indices
    // Since min AT is 1, queue is empty initially if current_time = 0.
    
    while(completed < n) {
        // Add new arrivals
        while(i < n && processes[p_indices[i]].at <= current_time) {
            if(!in_queue[p_indices[i]] && processes[p_indices[i]].remaining_bt > 0) {
                queue.push_back(p_indices[i]);
                in_queue[p_indices[i]] = true;
            }
            i++;
        }
        
        if(!queue.empty()) {
            int idx = queue.front();
            queue.erase(queue.begin());
            
            // Execute
            int exec_time = min(quantum, processes[idx].remaining_bt);
            processes[idx].remaining_bt -= exec_time;
            current_time += exec_time;
            
            // During execution, more processes might arrive
            while(i < n && processes[p_indices[i]].at <= current_time) {
                 // Check if already in queue or completed?
                 // Wait, if it arrives *during* execution, it should be added.
                 // But strictly speaking, it arrives at specific time.
                 // Usually added to queue immediately.
                 // But we are stepping by `exec_time`.
                 // We should add them.
                 // IMPORTANT: A process that is currently running is not in the queue.
                 // New arrivals go to back of queue.
                 // If current process finishes, it's done.
                 // If not, it goes to back of queue *after* new arrivals.
                 if(!in_queue[p_indices[i]] && processes[p_indices[i]].remaining_bt > 0) { // Should be > 0 (it is initial BT here)
                    queue.push_back(p_indices[i]);
                    in_queue[p_indices[i]] = true;
                 }
                 i++;
            }
            
            if(processes[idx].remaining_bt > 0) {
                queue.push_back(idx);
            } else {
                processes[idx].ct = current_time;
                processes[idx].tt = processes[idx].ct - processes[idx].at;
                processes[idx].wt = processes[idx].tt - processes[idx].bt;
                completed++;
            }
            
        } else {
            // Queue empty, jump to next arrival
            if(i < n) {
                current_time = processes[p_indices[i]].at;
                 while(i < n && processes[p_indices[i]].at <= current_time) {
                    if(!in_queue[p_indices[i]]) {
                        queue.push_back(p_indices[i]);
                        in_queue[p_indices[i]] = true;
                    }
                    i++;
                }
            } else {
                // Should not happen if completed < n
                current_time++;
            }
        }
    }

    cout << "the recommended quantum time" << endl; // Just print the label as requested? 
    // Wait, prompt says: "The results should be exactly this: the recommended quantum time ... RR: the quantum time ... (Table...)"
    // It seems "the recommended quantum time" is a value.
    // I will print the value.
    cout << "Quantum Time: " << quantum << endl;

    printTable(processes, "RR");
}


int main() {
    vector<int> ats = {1, 3, 8, 4, 6, 7, 2, 5};
    vector<int> bts = {53, 43, 18, 16, 24, 73, 99, 27};
    vector<Process> processes;
    
    for(int i=0; i<8; ++i) {
        Process p;
        p.pid = i + 1;
        p.at = ats[i];
        p.bt = bts[i];
        p.remaining_bt = p.bt;
        p.ct = 0; p.tt = 0; p.wt = 0;
        processes.push_back(p);
    }
    
    int quantum = calculateRecommendedQuantum(processes);
    
    cout << "Recommended Quantum Time: " << quantum << endl << endl;
    
    calculateSJF(processes);
    calculateSRTF(processes);
    calculateRR(processes, quantum);
    
    return 0;
}
