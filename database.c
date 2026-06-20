/**
 * ============================================================================
 * DATABASE CORE IMPLEMENTATION
 * ============================================================================
 * This file handles all the low-level memory management and linked-list logic.
 * It demonstrates dynamic heap allocation (malloc), pointer traversal, and
 * memory cleanup (free).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "database.h"

// The anchor of our Linked List. Starts as NULL indicating an empty list.
Record *head = NULL;

/**
 * put_value()
 * ----------------------------------------------------------------------------
 * Time Complexity: O(n) for search + O(1) for insertion.
 * Inserts a new record or updates an existing one if the key already exists.
 */
void put_value(const char *key, const char *value)
{
    // ---------------------------------------------------------
    // STEP 1: Search for an existing duplicate key (Update logic)
    // ---------------------------------------------------------
    Record *current = head;
    while (current != NULL)
    {
        if (strcmp(current->key, key) == 0)
        {
            // Key already exists! Overwrite the value buffer safely.
            strncpy(current->value, value, MAX_VAL_LEN - 1);
            current->value[MAX_VAL_LEN - 1] = '\0'; // Guarantee null-termination

#ifdef DEBUG
            printf("[DEBUG] Updated existing key: %s\n", key);
#endif
            printf("[SUCCESS] Record Updated\n");
            return; // Exit early since we updated an existing node
        }
        current = current->next;
    }

    // ---------------------------------------------------------
    // STEP 2: Key does not exist. Allocate new memory (Insert logic)
    // ---------------------------------------------------------
    // malloc() grabs raw bytes from the HEAP memory at runtime.
    Record *new_node = (Record *)malloc(sizeof(Record));

    // ALWAYS validate malloc() to prevent Segmentation Faults
    if (new_node == NULL)
    {
        printf("[ERROR] Memory allocation failed!\n");
        return;
    }

    // Safely copy the strings into our new heap-allocated node buffers
    strncpy(new_node->key, key, MAX_KEY_LEN - 1);
    new_node->key[MAX_KEY_LEN - 1] = '\0';

    strncpy(new_node->value, value, MAX_VAL_LEN - 1);
    new_node->value[MAX_VAL_LEN - 1] = '\0';

    // ---------------------------------------------------------
    // STEP 3: Link the new node into the list (Prepend at head)
    // ---------------------------------------------------------
    // new_node now points to the old head
    new_node->next = head;
    // head is updated to point to the new node
    head = new_node;

#ifdef DEBUG
    printf("[DEBUG] Inserted new key: %s at HEAP address %p\n", key, (void *)new_node);
#endif
    // EDUCATIONAL NOTE:
    // PUT only modifies the runtime linked-list structure stored in heap memory (RAM).
    // It does NOT persist data to disk until SAVE is explicitly called.
    printf("[SUCCESS] Record Stored In Memory\n");
}

/**
 * get_value()
 * ----------------------------------------------------------------------------
 * Time Complexity: O(n) linear search.
 * Traverses the list searching for a matching key.
 */
void get_value(const char *key)
{
    Record *current = head; // Start at the beginning

    // Traverse until we hit the end of the list (NULL)
    while (current != NULL)
    {
        if (strcmp(current->key, key) == 0)
        {
            printf("%s\n", current->value); // Match found
            return;
        }
        current = current->next; // Move to the next node
    }

    // If we reach here, the loop finished without finding the key
    printf("[ERROR] Missing key: %s\n", key);
}

/**
 * delete_value()
 * ----------------------------------------------------------------------------
 * Time Complexity: O(n).
 * Removes a node and safely frees its heap memory to prevent leaks.
 */
void delete_value(const char *key)
{
    Record *current = head;
    Record *previous = NULL; // We need to track the previous node to relink the chain

    while (current != NULL)
    {
        if (strcmp(current->key, key) == 0)
        {
            // Match found!

            // Case A: The node to delete is the very first node (head)
            if (previous == NULL)
            {
                head = current->next; // Move head forward
            }
            // Case B: The node is in the middle or end
            else
            {
                previous->next = current->next; // Bypass the current node
            }

            // Memory cleanup: Release the heap block back to the OS
            free(current);
            current = NULL; // Prevent "dangling pointer" vulnerability

#ifdef DEBUG
            printf("[DEBUG] Deleted and freed key: %s\n", key);
#endif
            printf("[SUCCESS] Record Deleted\n");
            return;
        }
        // Move our tracking pointers forward
        previous = current;
        current = current->next;
    }

    printf("[ERROR] Missing key: %s\n", key);
}

/**
 * show_all()
 * ----------------------------------------------------------------------------
 * Time Complexity: O(n).
 * Simply traverses and prints every node.
 */
void show_all()
{
    if (head == NULL)
    {
        printf("[INFO] Database is empty.\n");
        return;
    }

    Record *current = head;
    while (current != NULL)
    {
        printf("%s : %s\n", current->key, current->value);
        current = current->next;
    }
}

/**
 * free_database()
 * ----------------------------------------------------------------------------
 * CRITICAL FUNCTION: Called during program EXIT and before LOAD.
 * Iterates through every node and calls free().
 *
 * EDUCATIONAL NOTE:
 * This frees all volatile heap memory (RAM) used by the linked list.
 * Without this, the program would suffer massive Memory Leaks.
 */
void free_database()
{
    Record *current = head;
    Record *next_node;
    while (current != NULL)
    {
        // We MUST save the next pointer BEFORE freeing current
        // If we free(current) first, reading current->next is undefined behavior!
        next_node = current->next;

        free(current); // Release HEAP memory

        current = next_node; // Move to the saved next node
    }

    head = NULL; // Reset head cleanly to indicate an empty list

#ifdef DEBUG
    printf("[DEBUG] Entire database memory safely freed. No leaks!\n");
#endif
}
