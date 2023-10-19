#include <stdio.h>

void hanoi(int n, char a, char b, char c) {
    if (n > 0) {
        hanoi(n - 1, a, c, b);
        // printf("%d с %c на %c\n", n, a, b);
        hanoi(n - 1, c, b, a);
    }
}

int main() {
    int n;
    scanf("%d", &n);
    // printf("\n");
    hanoi(n, 'A', 'B', 'C');
    return 0;
}