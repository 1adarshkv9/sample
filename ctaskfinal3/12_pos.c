#include <stdio.h>
#include "main.h"

void positive() {
    int a;
   

    printf("Enter the number:\n");
    scanf("%d", &a);

    
    if(a>0){
        printf("%dis  positive number",a);
    }else{

    printf("%dis negative number",a);
    }
    
}
