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
              int baseHarvestAmount)
{
    Seed seed;
    seed.name = strdup(name);
    seed.basePrice = basePrice;
    seed.daysBeforeHarvest = daysBeforeHarvest;
    seed.baseHarvestAmount = baseHarvestAmount;

    return seed;
}

char* getSeedName(Seed* seed)
{
    return seed->name;
}

int getSeedBasePrice(Seed* seed)
{
    return seed->basePrice;
}

int getSeedDaysBeforeHarvest(Seed* seed)
{
    return seed->daysBeforeHarvest;
}

int getSeedHarvestAmount(Seed* seed)
{
    return seed->baseHarvestAmount;
}

void freeSeed(Seed* seed)
{
    free(seed->name);
}

#endif