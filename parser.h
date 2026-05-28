/**
 * ============================================================================
 * PARSER HEADER
 * ============================================================================
 * The parser sits between the user's raw text input and the database engine.
 * It is responsible for tokenizing strings and validating command syntax.
 */

#ifndef PARSER_H
#define PARSER_H

/**
 * Parses the raw input string buffer from the user.
 * 
 * @param input A mutable string buffer (strtok modifies the string in-place)
 * @return 0 if the EXIT command is given, 1 to continue the REPL loop
 */
int parse_command(char *input);

// Prints the interactive help menu
void print_help();

#endif // PARSER_H
