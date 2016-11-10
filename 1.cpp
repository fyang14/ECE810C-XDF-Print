
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, const char * argv[]) {
    // insert code here...
    int n;
    string line;
    ifstream infile("2f.dat");
    
    getline(infile, line);
    n = stoi(line);
    
    int arr[n],  sum = 0;
    for (int i = 0; i < n; i++) {
        getline(infile, line);
        arr[i] = stoi(line);
        sum += arr[i];
    }
    
    cout << (double)sum / n << endl;
    return 0;
}
