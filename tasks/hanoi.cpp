#include <iostream>

using namespace std;

int hanoi(int n, char a, char b, char c) {
    if (n > 0) {
        hanoi(n - 1, a, b, c); // с исходного на вспомогательный
        cout << n << " с " << a << " на " << b << '\n';
        hanoi(n - 1, c, a, b); // с вспомогательного на конечный
    }
    return 0;
}

int main() {
    int n;
    cin >> n;
    cout << '\n';
    hanoi(n, 'A', 'B', 'C');
    return 0;
}
