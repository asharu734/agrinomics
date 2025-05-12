#include <stdio.h>
#include <stdlib.h>

#include "../include/crops.h"
#include "../include/seeds.h"
#include "../include/economy.h"
#include "../include/plot.h"
#include "../include/loader.h"

#include "../include/crops.c"
#include "../include/seeds.c"
#include "../include/economy.c"
#include "../include/plot.c"
#include "../include/loader.c"

#define MAX_CROPS 100
#define MAX_SEEDS 100
#define MAX_PLOTS 6

int main()
{
    int playerMoney = 1000;

    Crop cropArray[MAX_CROPS];
    Seed seedArray[MAX_SEEDS];

    int numSeeds = loadSeedsFromCSV("../data/cropsAndSeeds.csv", seedArray, MAX_SEEDS);
    if (numSeeds < 0)
    {
        printf("Failed to load seeds.\n");
        return 1;
    }

    int numCrops = loadCropsFromCSV("../data/cropsAndSeeds.csv", cropArray, MAX_CROPS);
    if (numCrops < 0)
    {
        printf("Failed to load crops.\n");
        return 1;
    }

    printf("Loaded Seeds:\n");
    for (int i = 0; i < numSeeds; i++)
    {
        printf("Seed Name: %s, Base Price: %d, Days Before Harvest: %d, Base Harvest Amount: %d\n",
               seedArray[i].name, seedArray[i].basePrice, seedArray[i].daysBeforeHarvest, seedArray[i].baseHarvestAmount);
    }

    printf("\nLoaded Crops:\n");
    for (int i = 0; i < numCrops; i++)
    {
        printf("Crop Name: %s, Base Price: %d\n",
               cropArray[i].name, cropArray[i].basePrice);
    }

    Plot plotArray[MAX_PLOTS];
    initPlot(&plotArray[0]);

    plantSeed(&plotArray[0], &seedArray[0], &playerMoney);

    printf("Gendabloons: %d\n", playerMoney);

    for (int i = 0; i < 10; i++)
    {
        simulateGrowth(&plotArray[0]);
        randomizeCropSupplyAndDemand(&cropArray[0]);
        updateCropSellingPrice(&cropArray[0]);
    }

    harvestCrops(&plotArray[0], cropArray, MAX_CROPS, &playerMoney);

    printf("Gendabloons: %d\n", playerMoney);

    for (int i = 0; i < numSeeds; i++)
    {
        free(seedArray[i].name);
    }
    for (int i = 0; i < numCrops; i++)
    {
        free(cropArray[i].name);
    }

    return 0;
}