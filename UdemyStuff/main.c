#include <stdio.h>

int main() {
    int x = 100;
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

}
