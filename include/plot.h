#ifndef PLOT_H
#define PLOT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "seeds.h"
#include "crops.h"
#include "storage.h"

#define MAX 30

typedef struct Plot
{
    Seed plantedSeeds[MAX];
    int growingCropsAmount;
    int plantedSeedsGrowthDays[MAX];
}
Plot;

#endif