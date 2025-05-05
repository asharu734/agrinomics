#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Crop
{
    char* name;
    int growthTimeInDays;
    int basePrice;
    int buyingPrice;
    int sellingPrice;
    int supply;
    int demand;
} 
Crop;

Crop initCrop(char* name, int basePrice, int growthTimeInDays)
{
    Crop crop;
    crop.name = strdup(name);
    crop.basePrice = basePrice;
    crop.buyingPrice = 0.4 * basePrice;
    crop.sellingPrice = basePrice;
    crop.growthTimeInDays = growthTimeInDays;
    crop.supply = 0;
    crop.demand = 0;

    return crop;
}

char* getCropName(Crop* crop)
{
    return crop->name;
}

int getCropBasePrice(Crop* crop)
{
    return crop->basePrice;
}

int getCropSupply(Crop* crop)
{
    return crop->supply;
}

int getCropDemand(Crop* crop)
{
    return crop->demand;
}

void setCropSupply(Crop* crop, int newCropSupply)
{
    crop->supply = newCropSupply;
}

void setCropDemand(Crop* crop, int newCropDemand)
{
    crop->demand = newCropDemand;
}