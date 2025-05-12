#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "loader.h"

int loadSeedsFromCSV(const char* filename, Seed* seedsArray, int maxSeeds)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Error opening seeds file");
        return -1;
    }

    char line[256];
    int count = 0;

    // Ignores the first line
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file) && count < maxSeeds)
    {
        char* token = strtok(line, ",");
        if (token != NULL)
        {
            seedsArray[count].name = strdup(token);

            token = strtok(NULL, ",");
            seedsArray[count].basePrice = atoi(token);

            token = strtok(NULL, ",");
            seedsArray[count].daysBeforeHarvest = atoi(token);

            token = strtok(NULL, ",");
            seedsArray[count].baseHarvestAmount = atoi(token);

            count++;
        }
    }

    fclose(file);

    return count;
}

int loadCropsFromCSV(const char* filename, Crop* cropsArray, int maxCrops)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Error opening crops file");
        return -1;
    }

    char line[256];
    int count = 0;

    // Ignores the first line
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file) && count < maxCrops)
    {
        char* token = strtok(line, ",");
        if (token != NULL)
        {
            cropsArray[count].name = strdup(token);

            token = strtok(NULL, ",");
            cropsArray[count].basePrice = atoi(token);

            count++;
        }
    }

    fclose(file);

    return count;
}