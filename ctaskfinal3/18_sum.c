#include <stdio.h>
#include "main.h"

void sum() {
    // Declare variables to store the two numbers
    int num1, num2;

    // Input the first number
    printf("Enter the first number: ");
    scanf("%d", &num1);

    // Input the second number
    printf("Enter the second number: ");
    scanf("%d", &num2);

    // Calculate the sum
    int sum = num1 + num2;

    // Display the result
    printf("Sum of %d and %d is: %d\n", num1, num2, sum);

    
}

