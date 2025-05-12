#ifndef ECONOMY_H
#define ECONOMY_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../include/crops.h"
#include "../include/seeds.h"

float calculateRatioFromSupplyAndDemand(int supply, int demand);

// MUST be called before updating prices when the day progresses
void randomizeCropSupplyAndDemand(Crop* crop);

// Will be used in Crop array
void updateCropMarketPrice(Crop* crop);

#endif