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