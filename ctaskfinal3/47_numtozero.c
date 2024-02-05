#include <stdio.h>
#include "main.h"

int numtozero() {
    
    int arr[10];

    
    printf("Enter 10 numbers:\n");

    
    for (int i = 0; i < 10; i++) {
        printf("Enter number %d: ", i + 1);
        scanf("%d", &arr[i]);
    }

    
    int numtozero;
    printf("Enter a number to set to zero: ");
    scanf("%d", &numtozero);

    // Find the position of the entered number and set it to zero
    int position = -1;
    for (int i = 0; i < 10; i++) {
        if (arr[i] == numtozero) {
            position = i;
            arr[i] = 0;
            break;
        }
    }

    // Display the modified array and position
    printf("\nModified array:\n");
    for (int i = 0; i < 10; i++) {
        printf("%d ", arr[i]);
    }

    if (position != -1) {
        printf("\nPosition of %d in the array: %d\n", numtozero, position + 1);
    } else {
        printf("\n%d not found in the array.\n", numtozero);
    }

    
}

