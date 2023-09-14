#include <iostream>
#include <cmath>

using namespace std;

bool armstrong(int n) {
    int x, y;
    int count = 0;
    int summ = 0;
    x = n;
    y = n;
    while (x > 0) {
        x /= 10;
        count ++;
    }
    while (y > 0) {
        int d = y % 10;
        summ += pow(d, count);
        y /= 10;
    }
    return (n == summ);
}

int main() {
    int a, b;
    cin >> a >> b;
    for (; a <= b; a++) {
        if (armstrong(a)) {
            cout << a << endl;
        }
    }
    return 0;
}