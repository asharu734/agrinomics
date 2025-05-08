#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

//Reads the entire file into memory
char* read_file(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        perror("Could not open file.");
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    long filelength = ftell(file);
    rewind(file);

    char* data = (char*)malloc(filelength + 1);
    if (!data) {
        fclose(file);
        return NULL;
    }

    fread(data, 1, filelength, file);
    data[filelength] = '\0';
    fclose(file);
    return data;
}

char* run_dialogue(cJSON* root, const char* start_node, int* money) {
    const char* current_node_name = start_node;

    while (1) {
        cJSON* node = cJSON_GetObjectItem(root, current_node_name);
        if (!node) {
            printf("Dialogue node '%s' not found.\n", current_node_name);
            break;
        }

        cJSON* text = cJSON_GetObjectItem(node, "text");
        if (cJSON_IsString(text)) {
            printf("\n%s\n", text->valuestring);
        }

        cJSON* choices = cJSON_GetObjectItem(node, "choices");
        //THIS ONLY HAPPENS IF THERE'S NO CHOICES AND ENDS THE CODE. Will remove later.
        if (!cJSON_IsArray(choices) || cJSON_GetArraySize(choices) == 0) {
            printf("End of dialogue.\n");
            break;
        }
        //Money dialogue
        cJSON* show_money = cJSON_GetObjectItem(node, "show_money");
        if (cJSON_IsBool(show_money) && cJSON_IsTrue(show_money)) {
            printf("You currently have %d coins.\n\nYou...\n", *money);
        }
        //The numbering for the buy
        for (int i = 0; i < cJSON_GetArraySize(choices); ++i) {
            cJSON* choice = cJSON_GetArrayItem(choices, i);
            cJSON* option = cJSON_GetObjectItem(choice, "option");
            if (cJSON_IsString(option)) {
                printf("%d) %s\n", i + 1, option->valuestring);
            }
        }

        int user_choice;
        printf("Enter choice: ");
        scanf("%d", &user_choice);

        if (user_choice < 1 || user_choice > cJSON_GetArraySize(choices)) {
            printf("Invalid choice.\n");
            continue;
        }

        cJSON* chosen = cJSON_GetArrayItem(choices, user_choice - 1);
        cJSON* action = cJSON_GetObjectItem(chosen, "action");
        cJSON* price_json = cJSON_GetObjectItem(chosen, "price");

        //Money constraints
        if (action && strcmp(action->valuestring, "buy") == 0) {
            int price = price_json ? price_json->valueint : 0;
            if (*money >= price) {
                *money -= price;
                printf("You bought the item for %d coins. You now have %d coins.\n", price, *money);
            } else {
                printf("You don't have enough coins! You need %d but have %d.\n", price, *money);
                continue;
            }
        }

        //Goes to the "next" dialogue in the json.
        cJSON* next = cJSON_GetObjectItem(chosen, "next");
        if (cJSON_IsString(next)) {
            current_node_name = next->valuestring;
        } else {
            break;
        }

        //Handle "back"
        if (action && strcmp(action->valuestring, "back") == 0) {
            return "__BACK__";
        }
    }
}

void run_game(const char* filename){
    //Money example. Just add a system for the money cause idk what's the plan for it.
    int player_money = 20;
    
    char* file_content = read_file("filename");
    //Lazy to fix return 1
    if (!file_content) return 1;

    cJSON* root = cJSON_Parse(file_content);
    if (!root) {
        printf("Error parsing JSON.\n");
        free(file_content);
        return 1; //Lazy to fix return 1 sequel
    }

    run_dialogue(root, "start", &player_money);

    cJSON_Delete(root);
    free(file_content);
}

int main() {
    run_game("../include/dialogue.json");
    return 0;
}
