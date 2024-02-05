// Online C compiler to run C program online
#include <stdio.h>

int main() {
    int row=5,rowcount=1, starcount;
    
    while(rowcount<=row){
        for(starcount=1; starcount<=rowcount; starcount++){
            printf("*");
        }
        rowcount++;
        printf("\n");
        
    }
    

    return 0;
}
