#include <stdio.h>
#include <stdlib.h>

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