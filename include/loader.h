#ifndef LOADER_H
#define LOADER_H

#include "seeds.h"
#include "crops.h"

int loadSeedsFromCSV(const char* filename, Seed* seedsArray, int maxSeeds);

int loadCropsFromCSV(const char* filename, Crop* cropsArray, int maxCrops);

#endif