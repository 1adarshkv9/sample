// decimalToHex.c

#include <stdio.h>
#include "main.h"

void decimalToHex() {
    int n, i, count = 0, hexadecimal[5], a;

    // Input decimal number
    printf("Enter a decimal number: ");
    scanf("%d", &n);

    while (n != 0) {
        a = n % 16;

        // Convert remainder to hexadecimal
        if (a < 10) {
            hexadecimal[count++] = a + '0';
        } else {
            hexadecimal[count++] = a - 10 + 'A';
        }

        n = n / 16;
    }

    // Display the result in reverse order
    printf("Hexadecimal equivalent: 0x");
    for (i = count - 1; i >= 0; i--) {
        printf("%c", hexadecimal[i]);
    }

    printf("\n");
}

