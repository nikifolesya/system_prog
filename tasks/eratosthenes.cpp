#include <iostream>
using namespace std;

int main() {
    int n;
    cin >> n;
    int arr[n + 1];
    for (int i = 0; i < n + 1; i++) {
        arr[i] = i;
    }
    for (int p = 2; p < n + 1; p++) {
        if (arr[p] != 0) {
            cout << arr[p] << endl;
            for (int j = p*p; j < n + 1; j += p) {
                arr[j] = 0;
            }
        }
    }
}