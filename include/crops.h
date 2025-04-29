#include <stdio.h>

typedef struct Crop
{
    char* name;
    int basePrice;
    int currentPrice;
    int supply;
    int demand;
} 
Crop;

Crop initCrop(Crop* crop, char* name, int basePrice, int supply, int demand)
{
    crop->name = name;
    crop->basePrice = basePrice;
    crop->currentPrice = basePrice;
    crop->supply = supply;
    crop->demand = demand;
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