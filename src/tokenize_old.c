

#include "../header/minishell.h"

// Define a structure for AST nodes
typedef struct ASTNode {
    char *value;              // Command, operator, or filename
    struct ASTNode *left;     // Left child node
    struct ASTNode *right;    // Right child node
} ASTNode;

// Function to create a new AST node
ASTNode* createNode(char *value) {
    ASTNode *node = (ASTNode*)malloc(sizeof(ASTNode));
    node->value = strdup(value); // Duplicate the string to store it in the node
    node->left = NULL;
    node->right = NULL;
    return node;
}

#define MAX_TOKENS 100

// Tokenize the input command into an array of tokens
void tokenize(char *command, char **tokens, int *numTokens) {
    char *token = strtok(command, " "); // Split the command string by spaces
    while (token != NULL) {
        tokens[(*numTokens)++] = token; // Add each token to the tokens array
        token = strtok(NULL, " ");      // Continue splitting
    }
}

ASTNode* parsePrimary(char **tokens, int *index, int numTokens);
ASTNode* parsePipe(char **tokens, int *index, int numTokens);
ASTNode* parseLogical(char **tokens, int *index, int numTokens);
ASTNode* parseCommand(char **tokens, int *index, int numTokens);

// Parse basic commands and redirections
ASTNode* parsePrimary(char **tokens, int *index, int numTokens) {
    if (*index >= numTokens) return NULL;

    if (strcmp(tokens[*index], "(") == 0) {
        // Handle subexpression in parentheses
        (*index)++;
        ASTNode *subExprNode = parseCommand(tokens, index, numTokens);
        if (*index < numTokens && strcmp(tokens[*index], ")") == 0) {
            (*index)++;
        } else {
            // Error: unmatched parenthesis
            printf("Error: unmatched parenthesis\n");
            return NULL;
        }
        return subExprNode;
    }

    ASTNode *commandNode = createNode(tokens[*index]);
    (*index)++;

    // Handle arguments or redirections following the command
    while (*index < numTokens) {
        char *token = tokens[*index];

        if (strcmp(token, ">") == 0 || strcmp(token, ">>") == 0 ||
            strcmp(token, "<") == 0 || strcmp(token, "<<") == 0) {

            ASTNode *operatorNode = createNode(token);
            (*index)++;
            if (*index < numTokens) {
                operatorNode->right = createNode(tokens[*index]);
                (*index)++;
            }
            operatorNode->left = commandNode;
            commandNode = operatorNode;

        } else if (strcmp(token, "|") == 0 || strcmp(token, "||") == 0 ||
                   strcmp(token, "&&") == 0 || strcmp(token, "(") == 0 || strcmp(token, ")") == 0) {
            break;
        } else {
            size_t newSize = strlen(commandNode->value) + strlen(token) + 2;
            char *newValue = (char *)malloc(newSize);
            snprintf(newValue, newSize, "%s %s", commandNode->value, token);
            free(commandNode->value);
            commandNode->value = newValue;
            (*index)++;
        }
    }

    return commandNode;
}

// Parse pipes
ASTNode* parsePipe(char **tokens, int *index, int numTokens) {
    ASTNode *leftNode = parsePrimary(tokens, index, numTokens);

    while (*index < numTokens) {
        if (strcmp(tokens[*index], "|") == 0) {
            ASTNode *operatorNode = createNode(tokens[*index]);
            (*index)++;
            operatorNode->left = leftNode;
            operatorNode->right = parsePrimary(tokens, index, numTokens);
            leftNode = operatorNode;
        } else {
            break;
        }
    }

    return leftNode;
}

// Parse logical operators
ASTNode* parseLogical(char **tokens, int *index, int numTokens) {
    ASTNode *leftNode = parsePipe(tokens, index, numTokens);

    while (*index < numTokens) {
        if (strcmp(tokens[*index], "&&") == 0 || strcmp(tokens[*index], "||") == 0) {
            ASTNode *operatorNode = createNode(tokens[*index]);
            (*index)++;
            operatorNode->left = leftNode;
            operatorNode->right = parsePipe(tokens, index, numTokens);
            leftNode = operatorNode;
        } else {
            break;
        }
    }

    return leftNode;
}

// Parse commands and subcommands within parentheses
ASTNode* parseCommand(char **tokens, int *index, int numTokens) {
    return parseLogical(tokens, index, numTokens);
}

// Print the AST structure
void printAST(ASTNode *node, int level) {
    if (node != NULL) {
        for (int i = 0; i < level; i++) {
            printf("    ");
        }
        printf("%s\n", node->value);
        printAST(node->left, level + 1);
        printAST(node->right, level + 1);
    }
}

// Main function
int main() {
    char command[] = "( echo start && ( echo continue || echo fallback1 ) | grep check ) && echo end1 > out1.txt || echo end2 > out2.txt && cat < input.txt >> out3.txt";
    char *tokens[MAX_TOKENS];
    int numTokens = 0;

    tokenize(command, tokens, &numTokens); // Tokenize the input command

    int index = 0;
    ASTNode *astRoot = parseCommand(tokens, &index, numTokens); // Parse the tokens to build the AST

    printf("AST Structure:\n");
    printAST(astRoot, 0); // Print the AST structure

    // Free memory (not shown for brevity)

    return 0;
}

/*
Explanation

    Tokenize the Input: This remains unchanged, splitting the command string into individual tokens.
    parsePrimary: Now includes handling for parentheses. If a ( is encountered, it calls parseCommand to parse the subexpression within the parentheses.
    parsePipe: Handles the pipe operator (|) as before.
    parseLogical: Handles logical operators (&&, ||) as before.
    parseCommand: Entry point function for parsing commands and subcommands within parentheses. It calls parseLogical to parse the command.
*/
