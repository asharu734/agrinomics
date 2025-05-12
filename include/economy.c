#ifndef ECONOMY_C
#define ECONOMY_C

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

void updateCropSellingPrice(Crop* crop)
{
    int newSellingPrice = 
    crop->basePrice 
    * calculateRatioFromSupplyAndDemand(crop->supply, crop->demand);

    crop->sellingPrice = newSellingPrice;
}

void buySeed(Seed* seed, int* playerMoney)
{
    printf("Bought %s for %d Gendabloons\n", seed->name, seed->basePrice);
    *playerMoney -= seed->basePrice;
}

void addToPlayerMoney(int* playerMoney, int amount)
{
    *playerMoney += amount;
    printf("Earned %d Gendabloons.\n", amount);
}

#endif