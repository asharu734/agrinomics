#ifndef CROPS_H
#define CROPS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Crop
{
    char* name;
    int basePrice;
    int sellingPrice;
    int supply;
    int demand;
} 
Crop;

Crop initCrop(char* name, int basePrice);

char* getCropName(Crop* crop);

int getCropBasePrice(Crop* crop);

int getCropSupply(Crop* crop);

int getCropDemand(Crop* crop);

void setCropSupply(Crop* crop, int newCropSupply);

void setCropDemand(Crop* crop, int newCropDemand);

void setCropSellingPrice(Crop* crop, int newCropSellingPrice);

void freeCrop(Crop* crop);

#endif