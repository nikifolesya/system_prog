#include <stdio.h>

void Error(){
    printf("Error");
}

int main(){
    int n;
    scanf("%d", &n);
    if (n < 0){
        Error();
    }
    while (n > 0) {
        printf("%d\n", n%10);
        n /=10;
    }
}