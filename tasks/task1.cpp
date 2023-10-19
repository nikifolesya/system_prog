#include <iostream>
#include <cmath>

using namespace std;

void Error() {
    cout << "Error" << endl;
}

int task4(int n) {
    if(n<0) Error();
    while(n>0){
        cout << n%10 << endl;
        n /=10;
    }
    return 0;
}

int main() {
    int n;
    cin >> n;
    cout << endl;
    cout << task4(n) << endl;
    return 0;
}