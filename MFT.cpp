#include <iostream>
using namespace std;

int main()
{
    int ms, bs, nob, ef, n, p = 0, tif = 0;
    int mp[10];

    cout << "Enter the total memory available (in Bytes) -- ";
    cin >> ms;
    cout << "Enter the block size (in Bytes) -- ";
    cin >> bs;

    nob = ms / bs;
    ef = ms - nob * bs;

    cout << "\nEnter the number of processes -- ";
    cin >> n;

    for (int i = 0; i < n; i++)
    {
        cout << "Enter memory required for process " << i + 1 << " (in Bytes)-- ";
        cin >> mp[i];
    }

    cout << "\nNo. of Blocks available in memory--" << nob;
    cout << "\n\nPROCESS\tMEMORYREQUIRED\tALLOCATED\tINTERNAL FRAGMENTATION";

    for (int i = 0; i < n && p < nob; i++)
    {
        cout << "\n " << i + 1 << "\t\t" << mp[i];
        if (mp[i] > bs)
            cout << "\t\tNO\t\t---";
        else
        {
            cout << "\t\tYES\t" << bs - mp[i];
            tif += bs - mp[i];
            p++;
        }
    }

    if (n > p)
        cout << "\nMemory is Full, Remaining Processes cannot be accomodated";
    
    cout << "\n\nTotal Internal Fragmentation is " << tif;
    cout << "\nTotal External Fragmentation is " << ef;
    
    return 0;
}
