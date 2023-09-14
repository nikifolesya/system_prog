#include <iostream>
using namespace std;

int main() {
    int count = 0;
    int n;
    std::cin >> n;
    while (n > 0) {
        n = n / 10;
        count ++;
    }
    return 0;
    std::cout << count << endl;
}