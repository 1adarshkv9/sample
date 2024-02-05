#include <stdio.h>
#include "main.h"

void larg0f3() {
    int a, b, c;

    printf("Enter three numbers:\n");
    scanf("%d%d%d", &a, &b, &c);

    if (a > b) {
        if (a > c) {
            printf("%d is the largest\n", a);
        } else {
            printf("%d is the largest\n", c);
        }
    } else {
        if (b > c) {
            printf("%d is the largest\n", b);
        } else {
            printf("%d is the largest\n", c);
        }
    }

    
}
