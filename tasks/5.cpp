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
    for (int i = 1; i <= n/2; i++){
        if (n % i == 0){
            cout << i <<  " ";
        }
    }
    cout << n << endl;
    return 0;
}