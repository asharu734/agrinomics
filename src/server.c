//compile instructions: gcc -o server server.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <time.h>

typedef struct {
    int id;
    char name[512];
    int count;
} Card;

#define DECK_SIZE 12
#define PICK_SIZE 3
#define CARD_TYPE_COUNT 6

Card card_types[CARD_TYPE_COUNT] = {
    {1, "Typhoon: Destroy plots", 1},
    {2, "Heat Index: Crop growth decreased by a day", 3},
    {3, "Pandemic: Delay 3 turns", 1},
    {4, "Market Crash: Decrease selling price", 3},
    {5, "Seed Shortage: Increase seed prices", 3},
    {6, "Rain: Crop growth increased by 3 days", 1},
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

    int gameTurn = 0;

    // Communicate    
    while (gameTurn < 120) {
        // Wait for economy data (optional)
        bzero(buffer, 256);
        n = recv(client_sock, buffer, 255, 0);
        if (n < 0)
            die_with_error("Error: recv() Failed (economy data)");
    
        printf("Economy data received from client.\n");
    
        if (strncmp(buffer, "WIN", 3) == 0) {
            printf("The player has reached the goal of 10,000 Gendabloons! Game over.\n");
            break;
        }

        // Prepare PICK_SIZE card choices
        Card options[PICK_SIZE];
    
        index = 0;
        for (int i = 0; i < CARD_TYPE_COUNT; i++) {
            for (int j = 0; j < card_types[i].count; j++) {
                deck[index++] = card_types[i];
            }
        }
    
        shuffle(deck, DECK_SIZE);
        for (int i = 0; i < PICK_SIZE; i++) {
            options[i] = deck[i];
        }
    
        printf("\nYour options:\n");
        for (int i = 0; i < PICK_SIZE; i++) {
            printf("%d: %s\n", i + 1, options[i].name);
        }
    
        int choice = 0;
        while (choice < 1 || choice > PICK_SIZE) {
            printf("Pick a card (1-%d): ", PICK_SIZE);
            scanf("%d", &choice);
        }
    
        Card chosen = options[choice - 1];
        printf("You picked: %s\n", chosen.name);
    
        // Send the card ID to the client
        char card_msg[20];
        snprintf(card_msg, sizeof(card_msg), "%d\n", chosen.id);
        n = send(client_sock, card_msg, strlen(card_msg), 0);
        if (n < 0)
            die_with_error("Error: send() Failed (card_msg)");
    
        // Wait for TURN_END
        bzero(buffer, 256);
        n = recv(client_sock, buffer, 255, 0);
        if (n < 0)
            die_with_error("Error: recv() Failed (TURN_END)");
    
        buffer[n] = '\0';
        if (strncmp(buffer, "TURN_END", 8) != 0) {
            printf("Unexpected message while waiting for TURN_END: %s\n", buffer);
        } else {
            printf("Client ended turn. Proceeding...\n");
        }
    
        gameTurn++;
    }

    printf("Closing connection ...\n");
    close(client_sock);
    
    close(server_sock);
    
    return 0; 
}
