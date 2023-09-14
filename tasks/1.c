#include <stdio.h>

int main() {
    int n;
    int summ = 0;
    scanf("%d", &n);
    for(int i = 1; i < n; i++) {
        summ += i;
    }
    printf("%d", summ);
    return 0;
}
