/**
 * ============================================================================
 * PARSER IMPLEMENTATION
 * ============================================================================
 * This file translates raw terminal strings into executable database actions.
 * It demonstrates string tokenization (strtok) and safe bounds checking.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "database.h"
#include "storage.h"

void print_help() {
    printf("Available Commands:\n");
    printf("  PUT <key> <value>  : Insert or update a record\n");
    printf("  GET <key>          : Retrieve a value by key\n");
    printf("  DELETE <key>       : Remove a record by key\n");
    printf("  SHOWALL            : Print all records\n");
    printf("  SAVE               : Persist records to disk\n");
    printf("  LOAD               : Rebuild database from disk\n");
    printf("  HELP               : Show this help menu\n");
    printf("  EXIT               : Safely terminate the program\n");
}

int parse_command(char *input) {
    // ---------------------------------------------------------
    // STEP 1: Sanitize the raw input
    // ---------------------------------------------------------
    // fgets() grabs the newline character (\n) if the user pressed Enter.
    // We must strip this off, or commands will fail validation.
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0'; // Replace newline with null-terminator
    }

    // ---------------------------------------------------------
    // STEP 2: Tokenize the command
    // ---------------------------------------------------------
    // strtok() splits a string into tokens based on delimiters (spaces/tabs).
    // WARNING: strtok() modifies the original 'input' string by inserting \0.
    char *command = strtok(input, " \t");

    if (command == NULL) {
        return 1; // User pressed Enter without typing anything
    }

    // ---------------------------------------------------------
    // STEP 3: Dispatch and Validate
    // ---------------------------------------------------------
    // We use strcmp() to compare strings. If it returns 0, they match perfectly.
    
    if (strcmp(command, "PUT") == 0) {
        // Extract the next token as the key
        char *key = strtok(NULL, " \t");
        
        // For the value, we extract the REST of the string by passing "\n"
        // This allows values to contain spaces! (e.g., PUT user "John Doe")
        char *value = strtok(NULL, "\n"); 

        if (key == NULL || value == NULL) {
            printf("[ERROR] Invalid PUT syntax. Usage: PUT <key> <value>\n");
        } else {
            // Trim leading spaces from value safely
            while (*value == ' ' || *value == '\t') {
                value++;
            }
            if (strlen(value) == 0) {
                printf("[ERROR] Missing value for PUT.\n");
            } else {
                put_value(key, value);
            }
        }
    } 
    else if (strcmp(command, "GET") == 0) {
        char *key = strtok(NULL, " \t");
        char *extra = strtok(NULL, " \t"); // Check if user typed too much

        if (key == NULL) {
            printf("[ERROR] Missing key. Usage: GET <key>\n");
        } else if (extra != NULL) {
            printf("[ERROR] Extra arguments provided. Usage: GET <key>\n");
        } else {
            get_value(key);
        }
    } 
    else if (strcmp(command, "DELETE") == 0) {
        char *key = strtok(NULL, " \t");
        char *extra = strtok(NULL, " \t");

        if (key == NULL) {
            printf("[ERROR] Missing key. Usage: DELETE <key>\n");
        } else if (extra != NULL) {
            printf("[ERROR] Extra arguments provided. Usage: DELETE <key>\n");
        } else {
            delete_value(key);
        }
    } 
    else if (strcmp(command, "SHOWALL") == 0) {
        char *extra = strtok(NULL, " \t");
        if (extra != NULL) {
            printf("[ERROR] Extra arguments provided. Usage: SHOWALL\n");
        } else {
            show_all();
        }
    } 
    else if (strcmp(command, "SAVE") == 0) {
        char *extra = strtok(NULL, " \t");
        if (extra != NULL) {
            printf("[ERROR] Extra arguments provided. Usage: SAVE\n");
        } else {
            save_database();
        }
    } 
    else if (strcmp(command, "LOAD") == 0) {
        char *extra = strtok(NULL, " \t");
        if (extra != NULL) {
            printf("[ERROR] Extra arguments provided. Usage: LOAD\n");
        } else {
            load_database();
        }
    } 
    else if (strcmp(command, "HELP") == 0) {
        print_help();
    } 
    else if (strcmp(command, "EXIT") == 0) {
        char *extra = strtok(NULL, " \t");
        if (extra != NULL) {
            printf("[ERROR] Extra arguments provided. Usage: EXIT\n");
            return 1;
        }
        return 0; // Returning 0 signals main() to break the REPL loop
    } 
    else {
        printf("[ERROR] Unsupported command: %s\n", command);
        printf("[INFO] Type HELP for a list of valid commands.\n");
    }

    return 1; // Signal to continue the REPL loop
}
