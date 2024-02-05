// main.c


#include <stdio.h>
#include <stdlib.h>

#include "main.h"

int main() {
    int option;

    printf("Options:\n");
    printf("  1: Perform to find area of rectangle\n");
    printf("  2: Perform to find perimeter of circle\n");
    printf("  3: Perform to find perimeter of square\n");
    printf("  4: Perform to find area of square\n");
    printf("  5: Perform to find average of 3 numbers\n");
    printf("  6: Perform to find sum of 5 subjects\n");
    printf("  7: Perform to find resistance value\n");
    printf("  8: Perform to find power value\n");
    printf("  9: Perform to find sum using kirchoff law\n");
    printf("  10: Perform to find x value \n");
    printf("  11: Perform to find largest of two numbers \n");
    printf("  12: Perform to check a number positive or negative \n");
    printf("  13: Perform to find largest of three numbers \n");
    printf("  14: Perform to calculate grade \n");
    printf("  15: Perform to check comes under kirchoffs law \n");
    printf("  16: Perform to find eligibility for drivinglicense \n");
    printf("  17: Perform to get greetings according to age \n");
    printf("  18: Perform to find sum of two numbers \n");
    printf("  19: Perform to find even or odd \n");
    printf("  20: Perform to print first 20 natural numbers \n");
    printf("  21: Perform to print n even numbers \n");
    printf("  22: Perform to print first 30 odd numbers \n");
    printf("  23: Perform to print even numbers in range \n");
    printf("  24: Perform to print even numbers in reverse \n");
    printf("  25: Perform to print n numbers in reverse \n");
    printf("  26: Perform to print n even numbers in reverse \n");
    printf("  27: Perform to print series1 \n");
    printf("  28: Perform to print series2 \n");
    printf("  29: Perform to print 3's multiplication table \n");
    printf("  30: Perform to print all multiplication tables \n");
    printf("  31: Perform to print pattern 1 \n");
    printf("  32: Perform to print pattern 2 \n");
    printf("  33: Perform to print pattern 3 \n");
    printf("  34: Perform to print pattern 4 \n");
    printf("  35: Perform to print pattern 5 \n");
    printf("  36: Perform to print pattern 6 \n");
    printf("  37: Perform to find factorial \n");
    printf("  38: Perform to find factors of a number \n");
    printf("  39: Perform to check prime or not \n");
    printf("  40: Perform to reverse the digits \n");
    printf("  41: Perform to print fibanocci series \n");
    printf("  42: Perform to find gcd \n");
    printf("  43: Perform to print billing for items \n");
    printf("  44: Perform to check palindrome \n");
    printf("  45: Perform to find bcd  \n");
    printf("  46: Perform to print any 10 nums in ascending  \n");
    printf("  47: Perform to print position of a randomly picked no. in array which changed to zero  \n");
    printf("  48: Perform to print n prime no's  \n");
    
    printf("  49: Perform Hex to Decimal Conversion\n");
    printf("  50: Perform Decimal to Hex Conversion\n");
    
	
    printf("Enter your option: ");
    scanf("%d", &option);

    switch (option) {
        
        case 1:            
            area();
            break;
            
         case 2:            
            circleperi();
            break;   
            
         case 3:           
            squareperi();
            break; 
         
         case 4:           
            sqarea();
            break; 
         
         case 5:           
            average();
            break; 
            
         case 6:           
            subsum();
            break; 
            
         case 7:           
            resistance();
            break;
         
         case 8:           
            power();
            break; 
         
         case 9:           
            kirsum();
            break; 
         
         case 10:           
            xval();
            break;
            
         case 11:           
            larg();
            break;
            
         case 12:           
            positive();
            break;
         
         case 13:           
            larg0f3();
            break;
            
         case 14:           
            grade();
            break; 
         
         case 15:           
            kircheck();
            break;
         
         case 16:           
            license();
            break; 
            
         case 17:           
            greetings();
            break; 
            
         case 18:           
            sum();
            break;
         
         case 19:           
            even();
            break;
            
         case 20:           
            natural20nums();
            break; 
         
         case 21:           
            neven();
            break; 
            
         case 22:           
            thirtyodd();
            break;  
         
         case 23:           
            eveninrange();
            break; 
            
         case 24:           
            evenrev();
            break; 
            
         case 25:           
            nnumrev();
            break;  
            
         case 26:           
            nevenrev();
            break; 
            
         case 27:           
            series1();
            break; 
            
         case 28:           
            series2();
            break;
            
         case 29:           
            mul3();
            break;           
            
         case 30:           
            allmul();
            break;  
         
         case 31:           
            p1();
            break;  
            
         case 32:           
            p2();
            break;  
            
         case 33:           
            p3();
            break;   
            
         case 34:           
            p4();
            break;
            
         case 35:           
            p5();
            break;  
            
         case 36:           
            p6();
            break;  
            
         case 37:           
            fact();
            break;   
            
         case 38:           
            factors();
            break;  
            
         case 39:           
            prime();
            break;  
            
         case 40:           
            revnum();
            break;     
                       
         case 41:           
            fibanocci();
            break; 
            
         case 42:           
            gcd();
            break;                     
        
         case 43:           
            billing();
            break;  
            
         case 44:           
            palindrome();
            break; 
            
         case 45:           
            bcd();
            break;
            
         case 46:           
            asc();
            break;   
         
         case 47:           
            numtozero();
            break; 
            
         case 48:           
            nprime();
            break;            
                      
        case 49:
            hexToDecimal();
            break;
        case 50:            
            decimalToHex();
            break;  
             
        default:
            printf("Invalid option. Please choose 1 to 50.\n");
            return 1;
    }

    return 0;
}

