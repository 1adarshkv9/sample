#include <stdio.h>
#include "main.h"

void even() {
    // Write C code here
    int num;
    printf("enter the number");
    scanf("%d", &num);
    if(num%2==0){
       printf("even"); 
    }else {
        printf("odd");
    }

    
}
