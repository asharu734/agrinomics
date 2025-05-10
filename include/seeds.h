#ifndef SEED_H
#define SEED_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Seed
{
    char* name;
    int basePrice;
    int daysBeforeHarvest;
    int baseHarvestAmount;
}
Seed;                        

Seed initSeed(char* name, int basePrice, int daysBeforeHarvest,
              int baseHarvestAmount);

char* getSeedName(Seed* seed);

int getSeedBasePrice(Seed* seed);

int getSeedDaysBeforeHarvest(Seed* seed);

int getSeedHarvestAmount(Seed* seed);

void freeSeed(Seed* seed);

#endif