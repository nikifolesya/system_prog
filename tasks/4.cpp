#include <iostream>
#include <cmath>
using namespace std;

void Error() {
    cout << "Error" << endl;
}
int length(int n) {
    int count = 0;
    while(n>0) {
        n /= 10;
        count ++;
    }
    return count;
}

int powf(int n, int ampl){
    int s = 1;
    int i = 0;
    while(i<ampl){
        s*=n;
        i++;
    }
    return s;
}

int main() {
    int n;
    cin >> n;
    cout << endl;
    if(n<0) Error();
    while(n>0){
        cout << n/powf(10, length(n)-1) << endl;
        n %= powf(10, length(n)-1);
    }
    return 0;
}