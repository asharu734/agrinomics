//compile instructions: gcc -o client client.c


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

#include "../include/crops.h"
#include "../include/seeds.h"
#include "../include/economy.h"
#include "../include/plot.h"
#include "../include/loader.h"

#include "../include/crops.c"
#include "../include/seeds.c"
#include "../include/economy.c"
#include "../include/plot.c"
#include "../include/loader.c"

#define MAX_CROPS 100
#define MAX_SEEDS 100
#define MAX_PLOTS 6

// Global simulation state
int playerMoney = 1000;
Crop cropArray[MAX_CROPS];
Seed seedArray[MAX_SEEDS];
Plot plotArray[MAX_PLOTS];
int numCrops = 0, numSeeds = 0;
int initialized = 0;

void die_with_error(char *error_msg){
    printf("%s", error_msg);
    exit(-1);
}

void display_plot_states() {
    printf("\n--- Plot Status ---\n");
    for (int i = 0; i < MAX_PLOTS; i++) {
        printf("Plot %d: ", i + 1);
        if (plotArray[i].hasSeed) {
            if (plotArray[i].daysSincePlanted == 1){
               printf("Growing %s (Planted for %d day) (%d%%)\n", 
                  plotArray[i].growingSeed->name, 
                  plotArray[i].daysSincePlanted,
                  (plotArray[i].daysSincePlanted / seedArray[i].daysBeforeHarvest) * 100);//NEED TO BE FIXED ASAP
            }
            else {
                printf("Growing %s (Planted for %d days) %d (%d%%)\n", 
                   plotArray[i].growingSeed->name, 
                   plotArray[i].daysSincePlanted,
                   seedArray[i].daysBeforeHarvest,
                   (plotArray[i].daysSincePlanted / seedArray[i].daysBeforeHarvest) * 100);
            }
        } else {
            printf("Empty\n");
        }
    }
}

void display_seed_shop() {
    printf("\n--- Seed Shop ---\n");
    for (int i = 0; i < numSeeds; i++) {
        printf("%d) %s - %d Gendabloons (Grow Time: %d days, Harvest: %d crops)\n",
               i + 1,
               seedArray[i].name,
               seedArray[i].basePrice,
               seedArray[i].daysBeforeHarvest,
               seedArray[i].baseHarvestAmount);
    }
    printf("------------------\n");
}

void simulate_growth_phase() {
    for (int i = 0; i < MAX_PLOTS; i++) {
        if (plotArray[i].hasSeed) {
            // Simulate growth for this plot (only if it has a seed planted)
            simulateGrowth(&plotArray[i]);

            // Apply supply and demand for the specific crop planted in this plot
            // We are assuming the crop planted is linked to the plot, not just cropArray[0]
            for (int j = 0; j < numCrops; j++) {
                if (strcmp(plotArray[i].growingSeed->name, cropArray[j].name) == 0) {
                    randomizeCropSupplyAndDemand(&cropArray[j]); // Adjust the crop based on this plot
                    updateCropSellingPrice(&cropArray[j]);
                    break;  // We found the matching crop, no need to check others
                }
            }
        }
    }
}

void farmer_actions(int id, int client_sock) {
    if (!initialized) return;

    int plotIndex;

    switch(id) {
        case 1: // Plant
            display_plot_states();
            printf("Enter plot index to plant (1–%d): ", MAX_PLOTS);
            scanf("%d", &plotIndex);
            plotIndex -= 1;
            while (getchar() != '\n'); // clear input buffer
        
            if (plotIndex < 0 || plotIndex >= MAX_PLOTS) {
                printf("Invalid plot index.\n");
                break;
            }
        
            if (plotArray[plotIndex].hasSeed) {
                printf("This plot is already occupied.\n");
                break;
            }

            display_seed_shop();
            printf("Enter seed number to buy and plant: ");
            int seedChoice;
            scanf("%d", &seedChoice);
            seedChoice -= 1;
            while (getchar() != '\n'); // clear input buffer
        
            if (seedChoice < 0 || seedChoice >= numSeeds) {
                printf("Invalid seed choice.\n");
                break;
            }
        
            if (playerMoney < seedArray[seedChoice].basePrice) {
                printf("Not enough Gendabloons to buy %s.\n", seedArray[seedChoice].name);
                break;
            }
        
            playerMoney -= seedArray[seedChoice].basePrice;
            plantSeed(&plotArray[plotIndex], &seedArray[seedChoice], &playerMoney);
            printf("Planted %s in plot %d.\n", seedArray[seedChoice].name, plotIndex + 1);
            break;

        case 2: // Harvest
            display_plot_states();
            printf("Enter plot index to harvest (1–%d): ", MAX_PLOTS);
            scanf("%d", &plotIndex);
            plotIndex = plotIndex - 1;
            while(getchar() != '\n'); // clear input buffer

            if (plotIndex < 0 || plotIndex >= MAX_PLOTS) {
                printf("Invalid plot index.\n");
                break;
            }

            harvestCrops(&plotArray[plotIndex], cropArray, MAX_CROPS, &playerMoney);
            break;

        case 3:
            printf("Gendabloons: %d\n", playerMoney);
            break;

        default:
            printf("Unknown action ID: %d\n", id);
            break;
    }
}

void evaluate_and_execute(int id) {
    switch (id) {
        case 1: // Typhoon: Destroy plots
            printf("[EVENT] Typhoon! All plots have been destroyed.\n");
            for (int i = 0; i < MAX_PLOTS; i++) {
                plotArray[i].growingSeed = NULL;
                plotArray[i].daysSincePlanted = 0;
                plotArray[i].hasSeed = 0;
            }
            break;

        case 2: // Heat Index: Crop growth decreased by a day
            printf("[EVENT] Heat Index! Crops grow slower (growth decreased by 1 day).\n");
            for (int i = 0; i < MAX_PLOTS; i++) {
                if (plotArray[i].hasSeed && plotArray[i].daysSincePlanted > 0) {
                    plotArray[i].daysSincePlanted--;
                }
            }
            break;

        case 3: // Pandemic: Delay 3 turns
            printf("[EVENT] Pandemic! You are forced to skip 3 turns.\n");
            for (int i = 0; i < 3; i++) {
                simulate_growth_phase();
            }
            break;

        case 4: // Selling price decreased
            printf("[EVENT] Market Crash! Selling prices have decreased.\n");
            for (int i = 0; i < numCrops; i++) {
                cropArray[i].sellingPrice = (int)(cropArray[i].sellingPrice * 0.75); // reduce by 25%
            }
            break;

        case 5: // Seed price increased
            printf("[EVENT] Seed Shortage! Seed prices have increased.\n");
            for (int i = 0; i < numSeeds; i++) {
                seedArray[i].basePrice = (int)(seedArray[i].basePrice * 1.5); // increase by 50%
            }
            break;

        case 6: // Rain: Crop growth increased by 3 days
            printf("[EVENT] Rainy Week! Crops grow faster (growth increased by 3 days).\n");
            for (int i = 0; i < MAX_PLOTS; i++) {
                if (plotArray[i].hasSeed) {
                    plotArray[i].daysSincePlanted += 3;
                }
            }
            break;

        default:
            printf("[EVENT] Unknown event ID: %d\n", id);
            break;
    }
}

#define WINNING_GOAL 10000 // Gendabloon goal

int main(int argc,  char *argv[]){
    
    int client_sock,  port_no,  n;
    struct sockaddr_in server_addr;
    struct hostent *server;

    char buffer[256];
    if (argc < 3) {
        printf("Usage: %s hostname port_no",  argv[0]);
        exit(1);
    }

    // Load crop & seed data
    numSeeds = loadSeedsFromCSV("../data/cropsAndSeeds.csv", seedArray, MAX_SEEDS);
    numCrops = loadCropsFromCSV("../data/cropsAndSeeds.csv", cropArray, MAX_CROPS);
    if (numSeeds < 0 || numCrops < 0) {
        printf("Failed to load seed/crop data.\n");
        return 1;
    }

    // Init plots
    for (int i = 0; i < MAX_PLOTS; i++) {
        initPlot(&plotArray[i]);
    }

    initialized = 1; // Allow action_farmerd to run


    printf("Client starting ...\n");
    // Create a socket using TCP
    client_sock = socket(AF_INET,  SOCK_STREAM,  0);
    if (client_sock < 0) 
        die_with_error("Error: socket() Failed.");

    printf("Looking for host '%s'...\n", argv[1]);
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        die_with_error("Error: No such host.");
    }
    printf("Host found!\n");

    // Establish a connection to server
    port_no = atoi(argv[2]);
    bzero((char *) &server_addr,  sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,  
         (char *)&server_addr.sin_addr.s_addr, 
         server->h_length);
         
    server_addr.sin_port = htons(port_no);

    printf("Connecting to server at port %d...\n", port_no);
    if (connect(client_sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) 
        die_with_error("Error: connect() Failed.");

    printf("Connection successful!\n");

   int gameTurn = 0;

    // Communicate
    while(gameTurn < 120){
        int endTurn = 0;
        while (!endTurn) {
            printf("\n--- Your Turn ---\n");
            printf("Choose an action:\n");
            printf("1 = Plant\n");
            printf("2 = Harvest\n");
            printf("3 = Show Gendabloons\n");
            printf("0 = End Turn\n");
            printf("Enter action ID: ");

            bzero(buffer, 256);
            fgets(buffer, 255, stdin);
            int action_id = atoi(buffer);

            if (action_id == 0) {
                gameTurn++;
                n = send(client_sock, "TURN_END", strlen("TURN_END"), 0);
                endTurn = 1;  // Exit inner loop to end turn
            } else {
                farmer_actions(action_id, client_sock);
            }

            if (playerMoney >= WINNING_GOAL) {
                printf("\nCongratulations! You have reached %d Gendabloons and won the game!\n", WINNING_GOAL);
                send(client_sock, "WIN", strlen("WIN"), 0); // Notify the server about the win
                close(client_sock);
                return 0;
            }
        }

        bzero(buffer, 256);
        n = recv(client_sock, buffer, 255, 0);
        if (n < 0)
            die_with_error("Error: recv() Failed (Chosen Card)");
        printf("[server] > %s", buffer);

        simulate_growth_phase();

        int received_id = atoi(buffer);
        evaluate_and_execute(received_id);

        send(client_sock, "TURN_END", strlen("TURN_END"), 0);
    }

    printf("Game Finish\n");
    
    close(client_sock);
    
    for (int i = 0; i < numSeeds; i++) free(seedArray[i].name);
    for (int i = 0; i < numCrops; i++) free(cropArray[i].name);

    return 0;
}
