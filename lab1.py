import os
import sys

def main():
    try:
        # 1. Create a child process
        # This maps to: int pid; pid = fork();
        pid = os.fork()
    except OSError:
        # This maps to: if (pid < 0)
        print("fork failed")
        sys.exit(1)

    if pid == 0:
        # --- CHILD PROCESS ---
        # This maps to: else if (pid == 0)
        
        # execlp(file, arg0, arg1, ...)
        # The C code used "1s" as the second argument (argv[0]), so we match that here.
        # This replaces the current process image with 'whoami'.
        try:
            os.execlp("whoami", "1s")
        except OSError as e:
            print(f"Execution failed: {e}")
            sys.exit(0)
            
    else:
        # --- PARENT PROCESS ---
        # This maps to: else
        
        # print the current (Parent) Process ID
        print(f"\n Process ID is: {os.getpid()}")
        
        # Wait for the child process to finish
        # This maps to: wait(NULL);
        os.wait()
        
        sys.exit(0)

if __name__ == "__main__":
    main()