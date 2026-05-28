/**
 * ============================================================================
 * DATABASE CORE HEADER
 * ============================================================================
 * This header defines the central data structure of our database: the Record.
 * We use an in-memory Linked List to dynamically store key-value pairs.
 */

#ifndef DATABASE_H
#define DATABASE_H

// Hard limits for our string buffers to prevent buffer overflows
#define MAX_KEY_LEN 50
#define MAX_VAL_LEN 100

/**
 * struct Record
 * ----------------------------------------------------------------------------
 * This struct represents a single node in our Linked List database.
 * 
 * - key: The unique identifier string (up to 49 chars + null terminator)
 * - value: The stored data string (up to 99 chars + null terminator)
 * - next: A pointer to the next Record in the chain. This is what makes it a list!
 */
typedef struct Record {
    char key[MAX_KEY_LEN];
    char value[MAX_VAL_LEN];
    struct Record *next;
} Record;

/**
 * Global Database Head Pointer
 * ----------------------------------------------------------------------------
 * `extern` keyword means this variable is declared here but defined elsewhere (in database.c).
 * Other files (like storage.c) can access `head` to traverse the list for saving/loading.
 */
extern Record *head;

// --- Function Prototypes ---
// Defines the API contract for our database layer

// Insert or update a record
void put_value(const char *key, const char *value);

// Search for and print a record
void get_value(const char *key);

// Delete a record safely from the list
void delete_value(const char *key);

// Print all stored records
void show_all();

// Safely free all heap-allocated memory before shutdown
void free_database();

#endif // DATABASE_H
