/**
 * ============================================================================
 * MAIN APPLICATION ENTRY (REPL)
 * ============================================================================
 * This file is the orchestrator. It sets up the terminal user interface,
 * loads initial data, enters an infinite loop waiting for commands, and
 * guarantees memory cleanup upon exit.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "database.h"
#include "storage.h"

// Define a safe buffer size for user terminal input
#define MAX_INPUT_LEN 256

int main() {
    // Stack-allocated string buffer for standard I/O
    char input[MAX_INPUT_LEN];

    printf("==========================================\n");
    printf(" Lightweight Key-Value Database Engine\n");
    printf("==========================================\n");

    // ---------------------------------------------------------
    // STEP 1: Lifecycle Startup
    // ---------------------------------------------------------
    // Rebuild the linked list from the hard drive if a file exists.
    load_database();

    printf("[INFO] Type HELP for a list of available commands.\n\n");

    // ---------------------------------------------------------
    // STEP 2: The REPL Loop (Read - Eval - Print - Loop)
    // ---------------------------------------------------------
    while (1) {
        printf("DB > ");
        
        // Use fgets() instead of scanf() for extreme safety.
        // scanf("%s") is prone to buffer overflows if the user types 
        // more than MAX_INPUT_LEN characters. fgets() strictly enforces limits.
        if (fgets(input, sizeof(input), stdin) == NULL) {
            // Reaching EOF (e.g., Ctrl+D) or encountering an I/O error
            printf("\n");
            break;
        }

        // Pass the raw string to the parser. 
        // The parser returns 0 exclusively when it processes the "EXIT" command.
        if (parse_command(input) == 0) {
            break; // Break the infinite loop
        }
    }

    // ---------------------------------------------------------
    // STEP 3: Lifecycle Shutdown
    // ---------------------------------------------------------
    // We have broken out of the loop. Before the OS terminates our process,
    // we must act professionally and release all heap memory back to the system.
    free_database();
    
    printf("[INFO] Database shutdown safely. Goodbye!\n");
    
    // Returning 0 tells the Operating System the program exited without errors.
    return 0; 
}
