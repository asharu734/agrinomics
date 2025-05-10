#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/crops.h"

Crop initCrop(char* name, int basePrice)
{
    Crop crop;
    crop.name = strdup(name);
    crop.basePrice = basePrice;
    crop.sellingPrice = basePrice;
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

void setCropSellingPrice(Crop* crop, int newCropSellingPrice)
{
    crop->sellingPrice = newCropSellingPrice;
}

void freeCrop(Crop* crop)
{
    free(crop->name);
}