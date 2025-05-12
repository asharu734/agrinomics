#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../include/crops.h"
#include "../include/seeds.h"
#include "../include/economy.h"

float calculateRatioFromSupplyAndDemand(int supply, int demand)
{
    if (demand == 0.0)
        return 0.0;

    float priceRatio = (float)demand / (float)supply;

    return priceRatio;
}

void randomizeCropSupplyAndDemand(Crop* crop)
{
    int max = 100;
    int min = 1;

    crop->supply = rand() % (max + 1 - min) + min;
    crop->demand = rand() % (max + 1 - min) + min;
}

void updateCropCurrentPrice(Crop* crop)
{
    int newCurrentPrice = 
    crop->basePrice 
    * calculateRatioFromSupplyAndDemand(crop->supply, crop->demand);
}