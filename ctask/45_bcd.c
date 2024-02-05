#include <stdio.h>

void printBinary(int num, int bits) {
    for (int i = bits - 1; i >= 0; --i) {
        int bit = (num >> i) & 1;
        printf("%d", bit);
    }
}

void decimalToBCD(int num) {
    int bcdValue = 0, digit, shift = 0;

    printf("BCD representation of %d: ", num);

    while (num > 0) {
        digit = num % 10;

        
        bcdValue |= (digit << shift);
        shift += 4;

        num /= 10;
    }

    
    printBinary(bcdValue, 4);

    printf("\n");
}

int main() {
    int decimalNumber;

    
    printf("Enter a decimal number: ");
    scanf("%d", &decimalNumber);

    
    decimalToBCD(decimalNumber);

    return 0;
}

