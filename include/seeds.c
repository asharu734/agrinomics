#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "seeds.h"

Seed initSeed(char* name, int basePrice, int daysBeforeHarvest)
{
    Seed seed;
    seed.name = strdup(name);
    seed.basePrice = basePrice;
    seed.daysBeforeHarvest = daysBeforeHarvest;
    seed.baseHarvestAmount = 30;

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
