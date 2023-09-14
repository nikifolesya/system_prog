#include <iostream>
using namespace std;

int main() {
    int summ = 0;
    int n;
    cin >> n;

    for(int i = 1; i < n; i++) {
        summ += i;
    }

    cout << summ << endl;
    return 0;
}