#include <stdio.h>

long long fibonacci(int n) {
    long long numbers[n+1];
    numbers[0] = 0;
    numbers[1] = 1;
    for (int i = 2; i <= n; i++) {
        numbers[i] = numbers[i - 1] + numbers[i - 2];
    }
    for (int i = 0; i <= n; i++) {
        printf("%lld\n", numbers[i]);
    }
    return 0;
}

int main() {
    int n;
    scanf("%d", &n);
    fibonacci(n);
    return 0;
}