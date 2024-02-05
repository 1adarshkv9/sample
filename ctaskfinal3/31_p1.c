#include <stdio.h>
#include "main.h"

void p1() 
{
    int row = 1;
    while (row <= 5) 
    {
     for (int column = 1; column <= 5; column++) 
     {
    	 printf("* ");
     }
     printf("\n");
     row++;
    }

    
}

