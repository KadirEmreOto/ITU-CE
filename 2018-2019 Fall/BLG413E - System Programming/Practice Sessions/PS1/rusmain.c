#include <stdio.h>

int russian(int x, int y);

int main(void)
{
    int x, y, z;

    printf("Enter numbers: ");
    scanf("%d %d", &x, &y);
    z = russian(x, y);
    printf("The product is: %d\n", z);
    return 0;
}
