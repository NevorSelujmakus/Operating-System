using System;
using System.Diagnostics;

namespace SystemCallsLab
{
    class Program
    {
        static void Main(string[] args)
        {
            try
            {
                // In C code: printf("\n Process ID is: %d\n", getpid());
                // We print the Parent's PID first
                int parentPid = Process.GetCurrentProcess().Id;
                Console.WriteLine($"\n Process ID is: {parentPid}");

                // In C code: execlp("whoami", "1s", NULL);
                // We configure the child process to run "whoami"
                ProcessStartInfo startInfo = new ProcessStartInfo();
                startInfo.FileName = "whoami";
                startInfo.UseShellExecute = false; // Required to not use the OS shell
                
                // create the child process
                using (Process childProcess = Process.Start(startInfo))
                {
                    // In C code: wait(NULL);
                    // The parent waits for the child to finish
                    childProcess.WaitForExit();
                }
            }
            catch (Exception ex)
            {
                // In C code: printf("fork failed");
                Console.WriteLine($"Process creation failed: {ex.Message}");
                Environment.Exit(1);
            }

            Environment.Exit(0);
        }
    }
}