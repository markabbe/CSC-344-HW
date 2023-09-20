#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
    /*int x = 100;
    float y = 7.86;
    char ch ='A';
    printf("x = %d\n", x);
    printf("Address of x = %u\n", &x);
    printf("y = %2f\n", y);
    printf("Address of y = %u\n", &y);
    printf("ch = %c\n", ch);
    printf("Address of ch = %u\n", &ch);

    for (int i = 0; i < 10; ++i) {
        if (i % 2 == 1)
        {
            printf("The value of i is: %d\n", i);
        }
        else if(i % 2 == 0)
        {
            printf("Even \n");
        }
    }

    //Nested if statement
    int m,n,z,max;
    printf("Enter any three values:\n");
    scanf("%d%d%d", &m, &n, &z);
    if(m > n)
    {
        if(m > z)
        {
            max = m;
        }
        else
        {
            max = z;
        }
    }
    else
    {
        if (n > z)
        {
            max = n;
        }
        else
        {
            max = z;
        }
    }
    printf("Max value is: %d", max);*/

    int x,y,z,max;
    char answer[3];
    char input[100];
    while (1) {
        printf("Enter three values:\n");
        fgets(input, sizeof(input), stdin);

        if (sscanf(input, "%d%d%d", &x, &y, &z) != 3) {
            printf("Invalid input. Please enter exactly three integers.\n");
            continue; // Skip the rest of the loop and start over
        }

        if (x > y && x > z) {
            max = x;
        } else if (y > x && y > z) {
            max = y;
        } else {
            max = z;
        }

        printf("The max value is: %d\n", max);
        printf("Do you wish to continue (Type 'no' or 'yes')? \n");
        scanf("%s", answer);

        if(strcmp(answer, "no") == 0)
        {
            break;
        }
    }
}
