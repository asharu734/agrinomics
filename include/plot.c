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

    plot->daysSincePlanted++; // setter
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