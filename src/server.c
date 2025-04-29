#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <time.h>

#define DECK_SIZE 15
#define PICK_SIZE 3

void die_with_error(char *error_msg){
    printf("%s", error_msg);
    exit(-1);
}

void shuffle(int *deck, int size) {
    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = deck[i];
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
    

    int deck[DECK_SIZE];
    int used = 0;

    srand(time(NULL)); // Seed for randomness

    // Communicate    
    while(1){
        for (int i = 0; i < DECK_SIZE; i++) {
            deck[i] = i + 1;
        }
        shuffle(deck, DECK_SIZE);
        
        printf("\n--- New Cycle Begins ---\n");

    	for (int i = 0; i < DECK_SIZE; i += PICK_SIZE) {
            printf("\nYour options:\n");
        
            for (int j = 0; j < PICK_SIZE && (i + j) < DECK_SIZE; j++) {
                printf("%d: Card #%d\n", j + 1, deck[i + j]);
            }
        
            int choice = 0;
            while (choice < 1 || choice > PICK_SIZE || (i + choice - 1) >= DECK_SIZE) {
                printf("Pick a card (1-%d): ", PICK_SIZE);
                scanf("%d", &choice);
            }
        
            int chosen = deck[i + choice - 1];
            printf("You picked Card #%d!\n", chosen);
        
            // Send chosen card to the client
            char card_msg[256];
            snprintf(card_msg, sizeof(card_msg), "Chosen Card: #%d\n", chosen);
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
