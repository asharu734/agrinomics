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
    initCrop(&testCrop, "Potato", 30, 20, 20);
    printf("%s\n%d\n", getCropName(&testCrop), getCropBasePrice(&testCrop));

    printf("What in the goddamn?\n");
    
    return 0;
}