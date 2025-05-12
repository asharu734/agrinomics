#ifndef ECONOMY_H
#define ECONOMY_H

#include <stdio.h>
#include <stdlib.h>

#include "../include/crops.h"
#include "../include/seeds.h"

float calculateRatioFromSupplyAndDemand(int supply, int demand);

void updateCropMarketPrices(Crop* crops, int cropCount);

#endif