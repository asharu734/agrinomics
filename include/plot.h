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

void initPlot(Plot* plot)
{
    plot->growingCropsAmount = 0;
    for (int i = 0; i < MAX; i++)
        plot->plantedSeedsGrowthDays[i] = 0;
}

int isPlotFull(Plot* plot)
{
    return plot->growingCropsAmount = MAX;
}

int plantSeed(Plot* plot, Seed seed)
{
    if (isPlotFull(plot))
        return 0;

    plot->plantedSeeds[plot->growingCropsAmount] = seed;
    plot->plantedSeedsGrowthDays[plot->growingCropsAmount] = 0;
    plot->growingCropsAmount++;

    return 1;
}

void growCrops(Plot* plot)
{
    for (int i = 0; i < plot->growingCropsAmount; i++)
        plot->plantedSeedsGrowthDays[i]++;
}

Crop getCropByName(char* name, Crop* cropList, int cropCount)
{
    for (int i = 0; i < cropCount; i++)
    {
        if (strcmp(cropList[i].name, name) == 0)
            return cropList[i];
    }

    return initCrop("N/A", 0, 0);
}

void harvestCrops(Plot* plot, Storage* storage,
                  Crop* cropList, int cropCount)
{
    for (int i = 0; i < plot->growingCropsAmount; i++)
    {
        if (plot->plantedSeedsGrowthDays[i] 
            >= plot->plantedSeeds[i].daysBeforeHarvest)
        {
            int harvestAmount = 
            rand()
            % (plot->plantedSeeds[i].baseHarvestAmount + 1);

            Crop templateCrop = getCropByName(plot->plantedSeeds[i].name,
            cropList, cropCount);
            Crop harvestedCrop = initCrop(
                templateCrop.name,
                templateCrop.basePrice,
                templateCrop.growthTimeInDays
            );

            addCropToStorage(storage, harvestedCrop);

            for (int j = i; j < plot->growingCropsAmount - 1; j++)
            {
                plot->plantedSeeds[j] = plot->plantedSeeds[j + 1];
                plot->plantedSeedsGrowthDays[j] = 
                plot->plantedSeedsGrowthDays[j + 1];
            }

            plot->growingCropsAmount--;
            i--;
        }
    }
}

#endif