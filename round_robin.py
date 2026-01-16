def calculate_times(processes, n, burst_time, quantum):
    rem_bt = [0] * n
    wt = [0] * n
    tat = [0] * n
    
    # Copy the burst time into rem_bt[]
    for i in range(n):
        rem_bt[i] = burst_time[i]
    
    t = 0 # Current time
    
    # Keep traversing processes in round robin manner
    # until all of them are not done.
    while(1):
        done = True
        
        # Traverse all processes one by one repeatedly
        for i in range(n):
            
            # If burst time of a process is greater than 0
            # then only need to process further
            if (rem_bt[i] > 0) :
                done = False # There is a pending process
                
                if (rem_bt[i] > quantum) :
                
                    # Increase the value of t i.e. shows
                    # how much time a process has been processed
                    t += quantum
                
                    # Decrease the burst_time of current process
                    # by quantum
                    rem_bt[i] -= quantum
                
                # If burst time is smaller than or equal to
                # quantum. Last cycle for this process
                else:
                
                    # Increase the value of t i.e. shows
                    # how much time a process has been processed
                    t = t + rem_bt[i]
                
                    # Waiting time is current time minus time
                    # used by this process
                    wt[i] = t - burst_time[i]
                
                    # As the process gets fully executed
                    # make its remaining burst time = 0
                    rem_bt[i] = 0
                
        # If all processes are done
        if (done == True):
            break
            
    # Calculate turn around time
    for i in range(n):
        tat[i] = burst_time[i] + wt[i]

    # Display processes along with all details
    print("Processes    Burst Time     Waiting Time    Turn-Around Time")
    total_wt = 0
    total_tat = 0
    for i in range(n):
        total_wt = total_wt + wt[i]
        total_tat = total_tat + tat[i]
        print(f" {i+1}\t\t{burst_time[i]}\t\t {wt[i]}\t\t {tat[i]}")
        
    print(f"\nAverage waiting time = {total_wt /n}")
    print(f"Average turn around time = {total_tat / n}")

if __name__ == "__main__":
    # Driver code
    processes = [1, 2, 3]
    n = len(processes)
    burst_time = [10, 5, 8]
    quantum = 2
    
    print("Round Robin Scheduling Algorithm (Python)")
    print(f"Processes: {processes}")
    print(f"Burst Times: {burst_time}")
    print(f"Time Quantum: {quantum}\n")
    
    calculate_times(processes, n, burst_time, quantum)
