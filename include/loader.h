#ifndef LOADER_H
#define LOADER_H

#include "../include/crops.h"
#include "../include/seeds.h"

int loadSeedsFromCSV(const char* filename, Seed* seedsArray, int maxSeeds);

int loadCropsFromCSV(const char* filename, Crop* cropsArray, int maxCrops);

#endif