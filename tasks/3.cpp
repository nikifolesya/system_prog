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
    while(n>0){
        cout << n%10 << endl;
        n /=10;
    }
    return 0;
}