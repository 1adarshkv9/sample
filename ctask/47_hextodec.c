#include <stdio.h>
#include <string.h>

int hexToDecimal(char hex[]) {
    int decimal = 0, base = 1, length, i;

    // Find the length of the hex number
    length = strlen(hex);

    // Iterate through each character of the hex number
    for (i = length - 1; i >= 0; i--) {
        // Convert hex character to decimal value
        int digit;
        if (hex[i] >= '0' && hex[i] <= '9') {
            digit = hex[i] - '0';
        } else if (hex[i] >= 'A' && hex[i] <= 'F') {
            digit = hex[i] - 'A' + 10;
        } else if (hex[i] >= 'a' && hex[i] <= 'f') {
            digit = hex[i] - 'a' + 10;
        } else {
            // Invalid hex character
            printf("Invalid hex character: %c\n", hex[i]);
            return -1; // Error code for invalid input
        }

        // Add the contribution of the current hex digit to the decimal value
        decimal += digit * base;

        // Move to the next position (next power of 16)
        base *= 16;
    }

    return decimal;
}

int main() {
    char hex[20];

    // Input hex number
    printf("Enter a hexadecimal number: ");
    scanf("%s", hex);

    // Convert hex to decimal and display the result
    int decimal = hexToDecimal(hex);

    if (decimal != -1) {
        printf("Decimal equivalent: %d\n", decimal);
    } else {
        printf("Conversion failed.\n");
    }

    return 0;
}

