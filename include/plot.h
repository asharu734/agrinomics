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
}
Plot;

void initPlot(Plot* plot);

void plantSeed(Plot* plot, Seed* plantedSeed);

void simulateGrowth(Plot* plot);

int checkIfReadyForHarvest(Plot* plot);

int harvestCrops(Plot* plot);

#endif