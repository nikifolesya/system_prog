#include <iostream>
using namespace std;

int factorial(int n) {
    long fact = 1;
    for(int i = 1; i <= n; i++) {
        fact *= i;
    }
    return fact;
}

int main() {
    int n;
    cin >> n;
    cout << factorial(n) << endl;
    return 0;
}