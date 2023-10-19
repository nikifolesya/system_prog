#include <stdio.h>
#include <math.h>
#include <stdbool.h>


bool isPrime(int n) {
    for (int i = 2; i <= sqrt(n); i++){
        if((n % i) == 0){
            return false;
        }
    }
    return true;
}

int main() {
    int a, b;
    scanf("%d %d", &a, &b);
    while (a <= b) {
        if (isPrime(a)) {
            printf("%d\n", a*a);
        }
        a++; 
    }
    return 0;
}