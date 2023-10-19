#include <stdio.h>

int main() {
    int n1, n2;
    scanf("%d %d", &n1, &n2);
    while (n1 > 0 || n2 > 0) {
        if (n1 > n2) {
            n1 -= n2;
        }
        else if (n1 < n2){
            n2 -= n1;
        }
        else {
            break;
        }
    }
    if (n1 == 0 || n1 == n2) {
        printf("%d\n", n2);
    }
    else {
        printf("%d\n", n1);
    }
    return 0;
}