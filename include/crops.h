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

Crop initCrop(Crop* crop, char* name, int basePrice)
{
    crop->name = name;
    crop->basePrice = basePrice;
    crop->currentPrice = basePrice;
    crop->supply = 0;
    crop->demand = 0;
}

char* getCropName(Crop* crop)
{
    return crop->name;
}

int getCropBasePrice(Crop* crop)
{
    return crop->basePrice;
}

int getCropCurrentPrice(Crop* crop)
{
    return crop->currentPrice;
}

int getCropSupply(Crop* crop)
{
    return crop->supply;
}

int getCropDemand(Crop* crop)
{
    return crop->demand;
}

void setCropCurrentPrice(Crop* crop, int newCropCurrentPrice)
{
    crop->currentPrice = newCropCurrentPrice;
}

void setCropSupply(Crop* crop, int newCropSupply)
{
    crop->supply = newCropSupply;
}

void setCropDemand(Crop* crop, int newCropDemand)
{
    crop->demand = newCropDemand;
}