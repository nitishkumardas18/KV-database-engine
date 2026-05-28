/**
 * ============================================================================
 * STORAGE HEADER
 * ============================================================================
 * Defines the persistence layer of the database.
 * Without this, all data in RAM (heap memory) would be lost upon EXIT.
 */

#ifndef STORAGE_H
#define STORAGE_H

// Writes the entire linked list to a binary file
void save_database();

// Reconstructs the linked list from a binary file
void load_database();

#endif // STORAGE_H
