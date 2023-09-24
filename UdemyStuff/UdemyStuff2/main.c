#include <stdio.h>

int findMax(int i, int j, int k)
{
    int max;
    if(i > j && i > k)
    {
        max = i;
    }
    else if(j > i && j > k)
    {
        max = j;
    }
    else
    {
        max = k;
    }

    return max;
}

int main(){
//   int a = 1;
//   int b = 2;
//    switch (b - a)
//    {
//        case 1:
//            printf("one\n");
//            break;
//
//        case 2:
//            printf("two\n");
//            break;
//
//        case 3:
//            printf("three\n");
//            break;
//
//        default:
//            printf("A value other than one, two, or three\n");
//    }
//    int i = 1;
//    while (i < 11)
//    {
//        if(i % 2 == 0)
//        {
//            printf("%4d", i);
//        }
//        i++;
//    }

    int x, y, z, maxo;
    printf("Please enter three numbers: \n");
    scanf("%d%d%d", &x, &y, &z);

    maxo = findMax(x, y, z);

    printf("The max number is: %d\n", maxo);
}