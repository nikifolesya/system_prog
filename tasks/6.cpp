#include <iostream>
#include <cstdlib>

//средний рост игроков больше 180 и меньше 190

using namespace std;

int random(int low, int high) {
    return low + rand()%(high - low + 1);
}

int main() {
    int n;
    cin >> n;
    int arr[n];
    int count = 0;
    for (int i = 0; i < n; i++) {
        arr[i] = random(180, 190);
    }
    for (int i = 0; i < n; i++) {
        cout << arr[i] << '\n';
        count += arr[i];
    }
    
    cout << (int) count / n << '\n';

    return 0;
}