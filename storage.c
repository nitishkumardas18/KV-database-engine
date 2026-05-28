/**
 * ============================================================================
 * STORAGE IMPLEMENTATION (Binary File I/O)
 * ============================================================================
 * This file handles saving the in-memory database to the hard drive and 
 * loading it back into memory upon restart.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "database.h"
#include "storage.h"

#define DB_FILE "database.dat"

/**
 * FileRecord Struct
 * ----------------------------------------------------------------------------
 * CRITICAL CONCEPT: Why do we have a separate struct for files?
 * The standard `Record` struct has a `struct Record *next` pointer.
 * Pointers hold memory addresses specific to the CURRENT run of the program.
 * If we write a pointer to disk, and restart the program tomorrow, that memory
 * address will point to garbage! 
 * 
 * Therefore, we create a strict serialization struct `FileRecord` that ONLY 
 * holds the concrete character arrays (key and value) and strips away pointers.
 */
typedef struct FileRecord {
    char key[MAX_KEY_LEN];
    char value[MAX_VAL_LEN];
} FileRecord;

/**
 * save_database()
 * ----------------------------------------------------------------------------
 * Opens the binary file in "wb" (write-binary) mode. 
 * 
 * EDUCATIONAL NOTE:
 * SAVE serializes all linked-list records into the binary file using fwrite().
 * This operation writes data from RAM -> disk. This is the actual permanent storage operation.
 */
void save_database() {
    FILE *file = fopen(DB_FILE, "wb");
    if (file == NULL) {
        printf("[ERROR] Failed to open database file for writing!\n");
        return;
    }

    Record *current = head;
    FileRecord fr;
    int count = 0;

    // Traverse the entire list
    while (current != NULL) {
        // Safely copy data into our pointer-free serialization struct
        strncpy(fr.key, current->key, MAX_KEY_LEN);
        strncpy(fr.value, current->value, MAX_VAL_LEN);

        // Write the struct block to disk
        if (fwrite(&fr, sizeof(FileRecord), 1, file) != 1) {
            printf("[ERROR] Failed to write record to disk!\n");
            fclose(file);
            return;
        }
        count++;
        current = current->next; // Move forward in memory
    }

    fclose(file); // Ensure file is saved and unlocked
#ifdef DEBUG
    printf("[DEBUG] Saved %d records to %s\n", count, DB_FILE);
#endif
    printf("[SUCCESS] Database Persisted To Disk\n");
}

/**
 * load_database()
 * ----------------------------------------------------------------------------
 * Opens the binary file in "rb" (read-binary) mode. 
 * 
 * EDUCATIONAL NOTE:
 * LOAD reconstructs the database from the persistent binary storage layer.
 * This operation rebuilds RAM from the disk.
 */
void load_database() {
    FILE *file = fopen(DB_FILE, "rb");
    if (file == NULL) {
        // If file doesn't exist, it's just the first time running.
        printf("[INFO] No existing database found. Starting fresh.\n");
        return;
    }

    // Defensive Programming: Clear existing database to prevent duplicating 
    // nodes or leaking memory if LOAD is called in the middle of a session.
    free_database();

    FileRecord fr;
    int count = 0;
    
    // We maintain a tail pointer so we can append efficiently and preserve 
    // the original order of the database exactly as it was saved.
    Record *tail = NULL;

    // fread() reads exactly sizeof(FileRecord) bytes. Returns 1 on success.
    while (fread(&fr, sizeof(FileRecord), 1, file) == 1) {
        
        // Grab new heap memory for the reconstructed node
        Record *new_node = (Record *)malloc(sizeof(Record));
        if (new_node == NULL) {
            printf("[ERROR] Memory allocation failed during load!\n");
            break; // Stop loading, prevent crash
        }

        // Copy strings from the file buffer into heap memory
        strncpy(new_node->key, fr.key, MAX_KEY_LEN);
        strncpy(new_node->value, fr.value, MAX_VAL_LEN);
        new_node->next = NULL; // Newly loaded node points to nothing yet

        if (head == NULL) {
            // First node loaded becomes the head
            head = new_node;
            tail = new_node;
        } else {
            // Link the previous tail to this new node
            tail->next = new_node;
            tail = new_node; // Update tail pointer
        }
        count++;
    }

    fclose(file);
#ifdef DEBUG
    printf("[DEBUG] Loaded %d records from %s\n", count, DB_FILE);
#endif
    printf("[SUCCESS] Database Loaded From Disk\n");
}
