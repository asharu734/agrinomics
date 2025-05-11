#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "seeds.h"
#include "crops.h"
#include "plot.h"

void initPlot(Plot* plot)
{
    plot->growingSeed = NULL;
    plot->daysSincePlanted = 0;
    plot->isReadyForHarvest = 0;
}

void plantSeed(Plot* plot, Seed* plantedSeed)
{
    if (plot->growingSeed != NULL)
    {
        printf("This plot has something in it.\n");
        return;
    }

    plot->growingSeed = plantedSeed;
    plot->daysSincePlanted = 0;
    plot->isReadyForHarvest = 0;
}

void simulateGrowth(Plot* plot)
{
    if (plot->growingSeed == NULL)
        return;

    plot->daysSincePlanted++;
    checkIfReadyForHarvest(plot);
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

int harvestCrops(Plot* plot, Crop* cropsList, size_t cropsListSize)
{
    if (!checkIfReadyForHarvest(plot))
    {
        printf("Not ready.\n");

        return 0;
    }

    int income = 0;
    char* seedName = getSeedName(plot->growingSeed);
    // size_t cropsListSize = sizeof(cropsList) / sizeof(cropsList[0]);
    // This is unpossible

    for (int i = 0; i < cropsListSize; i++)
    {
        if (strcmp(cropsList[i].name, seedName) == 0)
        {
            income = cropsList[i].sellingPrice * 30; 
            // 30 can be changed to the harvest amount but idk
            break;
        }
    }

    // Initalize a temporary crop, put releveant crop 
    // data based on similar seed name
    // then return the current price of that crop * 30

    initPlot(plot);

    printf("Successful harvest!\n");

    return income;
}