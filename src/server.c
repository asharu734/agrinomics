//compile instructions: gcc -o server server.c cJSON.c -lcjson

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <time.h>
#include "../include/cJSON.h"

typedef struct {
    int id;
    char name[512];
    int count;
} Card;

#define DECK_SIZE 12
#define PICK_SIZE 3
#define CARD_TYPE_COUNT 6 //dagdag nalang to, ito yung kung ilan unique cards meron

Card card_types[CARD_TYPE_COUNT] = {
    {1, "Typhoon: Destroy plots", 1},
    {2, "Heat Index: Crop growth decreased by a day", 3},
    {3, "Pandemic: Delay 3 turns", 1},
    {4, "Selling price: Decreased", 3},
    {5, "Seed price: Increased", 3},
    {6, "Rain: Crop growth increased by 3 days", 1},
    //dito nalang ilalagay yung cards na naisip na
};

void die_with_error(char *error_msg){
    printf("%s", error_msg);
    exit(-1);
}

void shuffle(Card *deck, int size) {
    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Card temp = deck[i];
        deck[i] = deck[j];
        deck[j] = temp;
    }
}

int main(int argc, char *argv[]){
    int server_sock, client_sock, port_no, client_size, n;
    char buffer[256];
    struct sockaddr_in server_addr, client_addr;
    if (argc < 2) {
        printf("Usage: %s port_no", argv[0]);
        exit(1);
    }

    printf("Server starting ...\n");
    // Create a socket for incoming connections
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) 
       die_with_error("Error: socket() Failed.");
       
    // Bind socket to a port
    bzero((char *) &server_addr, sizeof(server_addr));
    port_no = atoi(argv[1]);
    server_addr.sin_family = AF_INET; // Internet address 
    server_addr.sin_addr.s_addr = INADDR_ANY; // Any incoming interface
    server_addr.sin_port = htons(port_no); // Local port
    
    if (bind(server_sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) 
       die_with_error("Error: bind() Failed.");
       
    // Mark the socket so it will listen for incoming connections
    listen(server_sock, 5);
    printf("Server listening to port %d ...\n", port_no);
    
    printf("Waiting for connection(s) ...\n");

    // Accept new connection
    client_size = sizeof(client_addr);
    client_sock = accept(server_sock, (struct sockaddr *) &client_addr, &client_size);
    if (client_sock < 0) 
        die_with_error("Error: accept() Failed.");

    printf("Client succesfully connected ...\n");    
    

    Card deck[DECK_SIZE];
    int index = 0;

    srand(time(NULL)); // Seed for randomness

    // Communicate    
    while(1){
        bzero(buffer, 256);
        n = recv(client_sock, buffer, 255, 0);
        if(n < 0){
            die_with_error("Error: recv() Failed");
        }

        // Parse the received JSON data
        cJSON* game_data = cJSON_Parse(buffer);
        if (game_data) {
            // Safely check and extract the "money" field
            cJSON* money_item = cJSON_GetObjectItem(game_data, "money");
            if (money_item && cJSON_IsNumber(money_item)) {
                int money = money_item->valueint;
                printf("Player has %d coins\n", money);
            } else {
                printf("Money data is missing or invalid.\n");
            }

            // Similarly extract other game state values with safety checks
            cJSON* seed_price_item = cJSON_GetObjectItem(game_data, "seed_price");
            if (seed_price_item && cJSON_IsNumber(seed_price_item)) {
                int seed_price = seed_price_item->valueint;
                printf("Seed price is %d\n", seed_price);
            } else {
                printf("Seed price data is missing or invalid.\n");
            }

            cJSON* crop_price_item = cJSON_GetObjectItem(game_data, "crop_price");
            if (crop_price_item && cJSON_IsNumber(crop_price_item)) {
                int crop_price = crop_price_item->valueint;
                printf("Crop price is %d\n", crop_price);
            } else {
                printf("Crop price data is missing or invalid.\n");
            }

            cJSON* plots_owned_item = cJSON_GetObjectItem(game_data, "plots_owned");
            if (plots_owned_item && cJSON_IsNumber(plots_owned_item)) {
                int plots_owned = plots_owned_item->valueint;
                printf("Player owns %d plots\n", plots_owned);
            } else {
                printf("Plots owned data is missing or invalid.\n");
            }

            // Clean up after parsing
            cJSON_Delete(game_data);
        } else {
            // Error handling if the JSON is invalid
            printf("Error parsing JSON data.\n");
        }


        //
        //Server interface process
        //
        for(int i = 0; i < CARD_TYPE_COUNT; i++){
            for(int j = 0; j < card_types[i].count; j++){
                deck[index].id = card_types[i].id;
                strncpy(deck[index].name, card_types[i].name, sizeof(deck[index].name));
                index++;
            }
        }
        shuffle(deck, DECK_SIZE);
        
        printf("\n--- New Cycle Begins ---\n");

    	for (int i = 0; i < DECK_SIZE; i += PICK_SIZE) {
            printf("\nYour options:\n");
        
            for (int j = 0; j < PICK_SIZE && (i + j) < DECK_SIZE; j++) {
                printf("%d: %s\n", j + 1, deck[i + j].name);
            }
        
            int choice = 0;
            while (choice < 1 || choice > PICK_SIZE || (i + choice - 1) >= DECK_SIZE) {
                printf("Pick a card (1-%d): ", PICK_SIZE);
                scanf("%d", &choice);
            }
        
            Card chosen = deck[i + choice - 1];
            printf("You picked Card %s!\n", chosen.name);
        
            // Send chosen card to the client
            char card_msg[256];
            snprintf(card_msg, sizeof(card_msg), "Chosen Card: #%d\n", chosen.id);
            n = send(client_sock, card_msg, strlen(card_msg), 0);
            if (n < 0) 
                die_with_error("Error: send() Failed (card_msg).");

                bzero(buffer, 256);
            n = recv(client_sock, buffer, 255, 0);
            if (n < 0) {
                die_with_error("Error: recv() Failed (waiting for TURN_END)");
            }

            buffer[n] = '\0'; // Ensure null-terminated string

            if (strncmp(buffer, "TURN_END", 8) == 0) {
                printf("Client ended turn. Proceeding...\n");
            } else {
                printf("Unexpected message while waiting for TURN_END: %s\n", buffer);
            }
        }        
    } 

    printf("Closing connection ...\n");
    close(client_sock);
    
    close(server_sock);
    
    return 0; 
}
