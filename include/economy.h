#include <stdio.h>
#include <stdlib.h>

void alertOfSuccess()
{
    printf("economy.h: Yippies!\n");
}

float calculateRatioFromSupplyAndDemand(int supply, int demand)
{
    if (demand == 0.0)
        return 0.0;

    float priceRatio = (float)demand / (float)supply;

    return priceRatio;
}
