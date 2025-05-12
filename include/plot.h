#ifndef PLOT_H
#define PLOT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "seeds.h"
#include "crops.h"

typedef struct Plot
{
    Seed* growingSeed;
    int daysSincePlanted;
    int isReadyForHarvest;
    int hasSeed; 
}
Plot;

void initPlot(Plot* plot);

void plantSeed(Plot* plot, Seed* plantedSeed, int* playerMoney);

void simulateGrowth(Plot* plot);

int checkIfReadyForHarvest(Plot* plot);

void harvestCrops(Plot* plot, Crop* cropsList, size_t cropsListSize, int* playerMoney);

#endif