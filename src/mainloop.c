#include <stdio.h>
#include <stdlib.h>

#include "../include/economy.h"
#include "../include/crops.h"
#include "../include/storage.h"
#include "../include/seeds.h"
#include "../include/plot.h"

// Will integrate into client later.

void printTitleScreen()
{
    printf("Hello, world!\n");
}

int main()
{
    // Tests
    printTitleScreen();
    alertOfSuccess();

    FILE *file = fopen("../data/crops.csv", "r");
    if (file == NULL)
    {
        printf("Error opening file\n");
        return EXIT_FAILURE;
    }

    Crop potato = initCrop("Potato", 6, 4);
    printf("%s\n%d\n", getCropName(&potato), getCropBasePrice(&potato));

    printf("What in the goddamn?\n");

    int testPrice = potato.basePrice;

    int testSupply = 46;
    int testDemand = 35;
    printf("Supple-Demand Ratio: %f\n", 
        calculateRatioFromSupplyAndDemand(testSupply, testDemand));

    int testCurrentPrice = testPrice
     * calculateRatioFromSupplyAndDemand(testSupply, testDemand);

    printf("Base price: %d\n New price: %d\n", testPrice, testCurrentPrice);

    Storage silo;
    initStorage(&silo);

    printf("%d\n", isCropsFull(&silo));
    printf("%d\n", isSeedsFull(&silo));
    
    
    return 0;
}