#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

// Read the entire file into memory
char* read_file(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        perror("Could not open file");
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    long filelength = ftell(file);
    rewind(file);
    char* data = malloc(filelength + 1);
    fread(data, 1, filelength, file);
    data[filelength] = '\0';
    fclose(file);
    return data;
}

void run_dialogue(cJSON* root, const char* start_node) {
    char current_node[150];
    strcpy(current_node, start_node);

    while (1) {
        cJSON* node = cJSON_GetObjectItem(root, current_node);
        if (!node) {
            printf("Dialogue node '%s' not found.\n", current_node);
            break;
        }

        const char* text = cJSON_GetObjectItem(node, "text")->valuestring;
        printf("\n%s\n", text);

        cJSON* choices = cJSON_GetObjectItem(node, "choices");
        int choice_count = cJSON_GetArraySize(choices);

        if (choice_count == 0) {
            printf("End of dialogue.\n");
            break;
        }

        for (int i = 0; i < choice_count; i++) {
            cJSON* choice = cJSON_GetArrayItem(choices, i);
            printf("[%d] %s\n", i + 1, cJSON_GetObjectItem(choice, "option")->valuestring);
        }

        int user_choice;
        printf("Enter choice: ");
        scanf("%d", &user_choice);

        if (user_choice < 1 || user_choice > choice_count) {
            printf("Invalid choice.\n");
            continue;
        }

        cJSON* chosen = cJSON_GetArrayItem(choices, user_choice - 1);
        const char* next = cJSON_GetObjectItem(chosen, "next")->valuestring;
        strcpy(current_node, next);
    }
}

int main() {
    char* file_content = read_file("../include/dialogue.json");
    if (!file_content) return 1;

    cJSON* root = cJSON_Parse(file_content);
    if (!root) {
        printf("Error parsing JSON.\n");
        free(file_content);
        return 1;
    }

    run_dialogue(root, "start");

    cJSON_Delete(root);
    free(file_content);
    return 0;
}
