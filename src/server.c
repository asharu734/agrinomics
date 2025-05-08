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
    char name[256];
    int count;
} Card;

#define DECK_SIZE 15
#define PICK_SIZE 3
#define CARD_TYPE_COUNT 6 //dagdag nalang to, ito yung kung ilan unique cards meron

Card card_types[CARD_TYPE_COUNT] = {
    {1, "Typhoon: Destroy plots", 1},
    {2, "Heat Index: Crop growth decreased by a day", 3},
    {3, "Pandemic: Delay 3 turns", 1},
    {1, "Selling price: Decreased", 3},
    {1, "Seed price: Increased", 3},
    {1, "Rain: Crop growth increased by 3 days", 1},
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

        printf("%s", buffer);
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
            printf("You picked Card #%s!\n", chosen.name);
        
            // Send chosen card to the client
            char card_msg[256];
            snprintf(card_msg, sizeof(card_msg), "Chosen Card: #%d\n", chosen.id);
            n = send(client_sock, card_msg, strlen(card_msg), 0);
            if (n < 0) 
                die_with_error("Error: send() Failed (card_msg).");
        }        
    } 

    printf("Closing connection ...\n");
    close(client_sock);
    
    close(server_sock);
    
    return 0; 
}
