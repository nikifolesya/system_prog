#include <stdio.h>
#include <stdlib.h>

int random(int low, int high) {
    return low + rand()%(high - low + 1);
}

int main() {
    int n;
    scanf("%d", &n);
    int *arr = (int*) malloc(n * sizeof(int));
    int count = 0;
    for (int i = 0; i < n; i++) {
        arr[i] = random(180, 190);
    }
    for (int i = 0; i < n; i++) {
        count += arr[i];
    }
    printf("%d\n", count / n);
    free(arr);
    return 0;
}