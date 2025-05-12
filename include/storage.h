// DEPRECEATED!! DON'T USE THIS SHIT!!

#ifndef STORAGE_H
#define STORAGE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "seeds.h"
#include "crops.h"

#define MAX_SPACE 100

typedef struct Storage
{
    Crop crops[MAX_SPACE];
    int cropsInStorage;
    Seed seeds[MAX_SPACE];
    int seedsInStorage;
}
Storage;

void initStorage(Storage* storage)
{
    storage->cropsInStorage = 0;
    storage->seedsInStorage = 0;
}

int isCropsFull(Storage* storage)
{
    return storage->cropsInStorage == MAX_SPACE;
}

int isSeedsFull(Storage* storage)
{
    return storage->seedsInStorage == MAX_SPACE;
}

int addCropToStorage(Storage* storage, Crop crop)
{
    if (isCropsFull(storage))
        return 0;

    storage->crops[storage->cropsInStorage++] = crop;

    return 1;
}

int addSeedToStorage(Storage* storage, Seed seed)
{
    if (isSeedsFull(storage))
        return 0;

    storage->seeds[storage->seedsInStorage++] = seed;

    return 1;
}

void freeStorage(Storage* storage)
{
    for (int i = 0; i < storage->cropsInStorage; i++)
    {
        freeCrop(&storage->crops[i]);
    }

    for (int i = 0; i < storage->seedsInStorage; i++)
    {
        freeSeed(&storage->seeds[i]);
    }
}

#endif