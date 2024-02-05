#include <stdio.h>
#include "main.h"

// Function to check if a number is prime
int isPrime(int num) {
    if (num <= 1) {
        return 0;  // Not a prime number
    }
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) {
            return 0;  // Not a prime number
        }
    }
    return 1;  // Prime number
}

// Function to print all prime numbers up to n
void printPrimes(int n) {
    printf("Prime numbers up to %d are: ", n);
    for (int i = 2; i <= n; i++) {
        if (isPrime(i)) {
            printf("%d ", i);
        }
    }
    printf("\n");
}

int nprime() {
    int n;

    // Input the value of n
    printf("Enter the value of n: ");
    scanf("%d", &n);

    // Print prime numbers up to n
    printPrimes(n);

    
}

