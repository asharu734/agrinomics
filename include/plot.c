#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "seeds.h"
#include "crops.h"
#include "economy.h"
#include "plot.h"

#include "economy.c"

void initPlot(Plot* plot)
{
    plot->growingSeed = NULL;
    plot->daysSincePlanted = 0;
    plot->isReadyForHarvest = 0;
    plot->hasSeed = 0;
}

void plantSeed(Plot* plot, Seed* plantedSeed, int* playerMoney)
{
    if (plot->growingSeed != NULL)
    {
        printf("This plot has something in it.\n");
        return;
    }

    buySeed(plantedSeed, playerMoney);

    plot->growingSeed = plantedSeed;
    plot->daysSincePlanted = 0;
    plot->isReadyForHarvest = 0;
    plot->hasSeed = 1;
}

void simulateGrowth(Plot* plot)
{
    if (plot->growingSeed == NULL)
        return;

    plot->daysSincePlanted++;
    checkIfReadyForHarvest(plot);
    
    printf("Growing %s for %d days.\n", plot->growingSeed->name, plot->daysSincePlanted);
}

int checkIfReadyForHarvest(Plot* plot)
{
    if (plot->growingSeed == NULL)
        return 0;

    if (plot->daysSincePlanted >= plot->growingSeed->daysBeforeHarvest)
    {
        plot->isReadyForHarvest = 1;

        return 1;
    }

    return 0;
}

void harvestCrops(Plot* plot, Crop* cropsList, size_t cropsListSize, int* playerMoney)
{
    if (!checkIfReadyForHarvest(plot))
    {
        if (plot->growingSeed == NULL) {
            printf("This plot is empty! Nothing to harvest.\n");
        } else {
            printf("%s isn't done growing.\n", plot->growingSeed->name);
        }
        return;
    }

    int income = 0;
    char* seedName = getSeedName(plot->growingSeed);

    for (int i = 0; i < cropsListSize; i++)
    {
        if (strcmp(cropsList[i].name, seedName) == 0)
        {
            income = cropsList[i].sellingPrice * plot->growingSeed->baseHarvestAmount;
            addToPlayerMoney(playerMoney, income);

            break;
        }
    }

    initPlot(plot);

    printf("Successful harvest!\n");
}