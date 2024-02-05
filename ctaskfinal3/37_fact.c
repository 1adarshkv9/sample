#include <stdio.h>
#include "main.h"

void fact() {
    int num=5, i, fact=1;

        for (i = 1; i <= num; ++i) {
            fact *= i;
        }

        
        printf("Factorial of %d = %d\n", num, fact);
    

    
}

