#include <iostream>
using namespace std;

int fibonacci(int n) {
    long long numbers[n+1];
    numbers[0] = 0;
    numbers[1] = 1;
    for (int i = 2; i <= n; i++) {
        numbers[i] = numbers[i - 1] + numbers[i - 2];
    }
    for (long long i : numbers) {
        cout << i << endl;
    }
    return 0;
}

int main() {
    int n;
    cin >> n;
    fibonacci(n);
    return 0;
}