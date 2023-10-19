#include <iostream>
#include <cstdlib>
#include <time.h>

using namespace std;

int random(int low, int high) {
    return low + time(NULL)*rand()%(high - low + 1);
}

int main() {
    int n;
    cin >> n;
    int arr[n];
    int summ1 = 0;
    int summ2 = 0;
    int count1 = 0;
    int count2 = 0;
    for (int i = 0; i < n; i++) {
        arr[i] = random(0, 100);
    }

    for (int i = 0; i < n; i++) {
        cout << arr[i] << ' '; 
        if (arr[i] < 50) {
            summ1 += arr[i];
            count1++;
        }
        else {
            summ2 += arr[i];
            count2++;
        }
    }
    cout << '\n' << (float) summ1 / count1 << '\n' << (float) summ2 / count2 << '\n';

    return 0;
}