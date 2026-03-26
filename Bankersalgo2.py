def calculate_need(processes, resources, max_req, allocated):
    """Step 4: After allocation find the need value (Need = Max - Allocated)."""
    need = [[0] * resources for _ in range(processes)]
    for i in range(processes):
        for j in range(resources):
            need[i][j] = max_req[i][j] - allocated[i][j]
    return need

def check_safe_state(processes, resources, max_req, allocated, available):
    # Calculate the Need matrix
    need = calculate_need(processes, resources, max_req, allocated)
    
    # Track which processes have finished
    finish = [False] * processes
    safe_sequence = []
    
    # Work array initializes as the available resources
    work = available.copy()
    count = 0
    
    # Step 5: Check whether it’s possible to allocate.
    while count < processes:
        found = False
        for p in range(processes):
            if not finish[p]:
                # Check if the process's needs can be met by currently available (work) resources
                if all(need[p][j] <= work[j] for j in range(resources)):
                    
                    # If resources can be allocated, simulate execution and release of resources
                    for k in range(resources):
                        work[k] += allocated[p][k]
                        
                    safe_sequence.append(p)  # Add to safe sequence
                    finish[p] = True         # Mark process as finished
                    found = True
                    count += 1
                    
        # Step 7: Else system is not in safety state
        if not found:
            print("System is NOT in a safe state. Deadlock may occur.")
            return False

    # Step 6: If it is possible then the system is in safe state.
    print("System is in a SAFE state.")
    print(f"Safe Execution Sequence: Process {safe_sequence}")
    return True

# Step 1: Start the program.
if __name__ == "__main__":
    
    # Step 2: Get the values of resources and processes.
    processes = 5
    resources = 3

    print(f"Total Processes: {processes}")
    print(f"Total Resources: {resources}\n")

    # Maximum resources that can be requested by each process
    max_matrix = [
        [7, 5, 3],
        [3, 2, 2],
        [9, 0, 2],
        [2, 2, 2],
        [4, 3, 3]
    ]

    # Resources currently allocated to each process
    allocated_matrix = [
        [0, 1, 0],
        [2, 0, 0],
        [3, 0, 2],
        [2, 1, 1],
        [0, 0, 2]
    ]

    # Step 3: Get the avail value.
    available_vector = [3, 3, 2]
    
    # Execute the safety algorithm
    check_safe_state(processes, resources, max_matrix, allocated_matrix, available_vector)
    
    # Step 8: Stop the process. (Program ends naturally here)