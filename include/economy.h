#include <stdio.h>
#include <stdlib.h>

#include "../include/crops.h"
#include "../include/seeds.h"

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

void updateCropMarketPrices(Crop* crops, int cropCount)
{
    for (int i = 0; i < cropCount; i++)
    {
        float ratio = calculateRatioFromSupplyAndDemand(
            crops[i].supply + 1, crops[i].demand + 1);
        float randomFactor = 0.9f + (rand() % 21) / 100.0f;

        crops[i].buyingPrice = crops[i].basePrice * 0.4 * ratio * randomFactor;
        crops[i].sellingPrice = crops[i].basePrice * ratio * randomFactor;
    }
}