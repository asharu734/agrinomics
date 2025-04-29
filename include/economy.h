#include <stdio.h>
#include <stdlib.h>

void alertOfSuccess()
{
    printf("economy.h: Yippies!\n");
}

int calculateRatioFromSupplyAndDemand(int supply, int demand)
{
    int priceRatio = supply / demand;

    return priceRatio;
}
