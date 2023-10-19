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
    for (int i=1; i<=n ;i++){
        printf("%d\n", i);
    }
}