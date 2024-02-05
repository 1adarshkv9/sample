#include <stdio.h>

int main() {
    // Declare variables
    int i = 0, n, total = 0;

    // Ask the user to enter the number of items
    printf("Enter the number of items: ");
    scanf("%d", &n);

    // Check if the number of items is within a reasonable range (e.g., less than or equal to 100)
    if (n > 0 && n <= 100) {
        // Declare arrays to store prices and quantities of items
        int prices[100], quantities[100];

        // Start a loop to iterate through each item
        for (i = 0; i < n; i++) {
            // Ask the user to enter the price and quantity for each item
            printf("Enter price for Item %d: ", i + 1);
            scanf("%d", &prices[i]);

            printf("Enter quantity for Item %d: ", i + 1);
            scanf("%d", &quantities[i]);

            // Calculate the total price for the current item
            int itemPrice = prices[i] * quantities[i];

            // Update the total cost
            total += itemPrice;

            // Print the shopping list for the current item
            printf("\nShopping List:");
            printf("\nItem %d - Price: %d, Quantity: %d, Total: %d\n", i + 1, prices[i], quantities[i], itemPrice);
        }

        // Print the total cost
        printf("\nTotal Cost: %d\n", total);
    } else {
        // Handle the case where the number of items is not within a reasonable range
        printf("Invalid number of items. Please enter a value between 1 and 100.\n");
    }

    return 0;
}

