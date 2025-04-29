#include <stdio.h>
#include <stdlib.h>

#include "../include/economy.h"
#include "../include/crops.h"

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

    Crop testCrop;
    initCrop(&testCrop, "Potato", 30);
    printf("%s\n%d\n", getCropName(&testCrop), getCropBasePrice(&testCrop));

    printf("What in the goddamn?\n");

    int testPrice = testCrop.basePrice;

    int testSupply = 46;
    int testDemand = 0;
    printf("Supple-Demand Ratio: %f\n", 
        calculateRatioFromSupplyAndDemand(testSupply, testDemand));

    int testCurrentPrice = testPrice
     * calculateRatioFromSupplyAndDemand(testSupply, testDemand);

    printf("Base price: %d\n New price: %d\n", testPrice, testCurrentPrice);
    
    return 0;
}