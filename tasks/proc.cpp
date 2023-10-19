#include <iostream>
using namespace std;

void Error() {
    cout << "Error" << endl;
}

int main() {
    int n;
    cin >> n;
    cout << endl;
    if(n<0) Error();
    for(int i = 1; i <= n; i++) {
        cout << i << endl;
    }
    return 0;
}