#include "../include/dialogue.h"

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

char* run_dialogue(cJSON* root, const char* start_node, GameState* state) {
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
        if (!cJSON_IsArray(choices) || cJSON_GetArraySize(choices) == 0) {
            printf("End of dialogue.\n");
            break;
        }

        cJSON* show_money = cJSON_GetObjectItem(node, "show_money");
        if (cJSON_IsBool(show_money) && cJSON_IsTrue(show_money)) {
            printf("You currently have %d coins.\n\nYou...\n", state->money);
        }

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

        if (action && strcmp(action->valuestring, "buy") == 0) {
            int price = price_json ? price_json->valueint : 0;
            if (state->money >= price) {
                state->money -= price;
                printf("You bought the item for %d coins. You now have %d coins.\n", price, state->money);
            } else {
                printf("You don't have enough coins! You need %d but have %d.\n", price, state->money);
                continue;
            }
        }

        cJSON* next = cJSON_GetObjectItem(chosen, "next");
        if (cJSON_IsString(next)) {
            current_node_name = next->valuestring;
        } else {
            break;
        }

        if (action && strcmp(action->valuestring, "back") == 0) {
            return "__BACK__";
        }
    }

    return NULL;
}

GameState run_game(const char* filename) {
    GameState state = { .money = 20, .seed_price = 10, .crop_price = 5, .plots_owned = 0 };

    char* file_content = read_file(filename);
    if (!file_content) return state;

    cJSON* root = cJSON_Parse(file_content);
    if (!root) {
        printf("Error parsing JSON.\n");
        free(file_content);
        return state;
    }

    run_dialogue(root, "start", &state);

    cJSON_Delete(root);
    free(file_content);

    return state;
}

char* serialize_game_state(GameState state) {
    cJSON* root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "money", state.money);
    cJSON_AddNumberToObject(root, "seed_price", state.seed_price);
    cJSON_AddNumberToObject(root, "crop_price", state.crop_price);
    cJSON_AddNumberToObject(root, "plots_owned", state.plots_owned);

    char* json_str = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);
    return json_str;
}
