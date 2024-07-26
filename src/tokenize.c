

#include "../header/minishell.h"

// Function to create a new AST node
t_ast	*create_node(char *value)
{
	t_ast	*node;
	
	node = (t_ast*)malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->value = ft_strdup(value); // Duplicate the string to store it in the node
	if (!node->value)
	{
		free(node);
		return (NULL);
	}
	node->left = NULL;
	node->right = NULL;
	return node;
}

t_ast	*parsePrimary(char **tokens, int *index, int numTokens);
t_ast	*parsePipe(char **tokens, int *index, int numTokens);
t_ast	*parseLogical(char **tokens, int *index, int numTokens);
t_ast	*parseCommand(char **tokens, int *index, int numTokens);

// Parse basic commands and redirections
t_ast	*parsePrimary(char **tokens, int *index, int numTokens)
{
	if (*index >= numTokens) return NULL;

	if (ft_strncmp(tokens[*index], "(", 1) == 0)
	{
		// Handle subexpression in parentheses
		(*index)++;
		t_ast *subExprNode = parseCommand(tokens, index, numTokens);
		if (*index < numTokens && ft_strncmp(tokens[*index], ")", 1) == 0)
			(*index)++;
		else
		{
			// Error: unmatched parenthesis
			printf("Error: unmatched parenthesis\n");
			return NULL;
		}
		return subExprNode;
	}

	t_ast *commandNode = create_node(tokens[*index]);
	(*index)++;

	// Handle arguments or redirections following the command
	while (*index < numTokens) {
		char *token = tokens[*index];

		if (ft_strncmp(token, ">", 1) == 0 || ft_strncmp(token, ">>", 2) == 0 ||
			ft_strncmp(token, "<", 1) == 0 || ft_strncmp(token, "<<", 2) == 0)
		{
			t_ast *operatorNode = create_node(token);
			(*index)++;
			if (*index < numTokens)
			{
				operatorNode->right = create_node(tokens[*index]);
				(*index)++;
			}
			operatorNode->left = commandNode;
			commandNode = operatorNode;
		}
		else if (ft_strncmp(token, "|", 1) == 0 || ft_strncmp(token, "||", 2) == 0 ||
				ft_strncmp(token, "&&", 2) == 0 || ft_strncmp(token, "(", 1) == 0 || ft_strncmp(token, ")", 1) == 0) {
			break;
		}
		else
		{
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
t_ast	*parsePipe(char **tokens, int *index, int numTokens)
{
	t_ast *leftNode = parsePrimary(tokens, index, numTokens);

	while (*index < numTokens)
	{
		if (ft_strncmp(tokens[*index], "|", 1) == 0)
		{
			t_ast *operatorNode = create_node(tokens[*index]);
			(*index)++;
			operatorNode->left = leftNode;
			operatorNode->right = parsePrimary(tokens, index, numTokens);
			leftNode = operatorNode;
		}
		else
			break;
	}
	return leftNode;
}

// Parse logical operators
t_ast	*parseLogical(char **tokens, int *index, int numTokens)
{
	t_ast *leftNode = parsePipe(tokens, index, numTokens);

	while (*index < numTokens)
	{
		if (ft_strncmp(tokens[*index], "&&", 2) == 0 || ft_strncmp(tokens[*index], "||", 2) == 0)
		{
			t_ast *operatorNode = create_node(tokens[*index]);
			(*index)++;
			operatorNode->left = leftNode;
			operatorNode->right = parsePipe(tokens, index, numTokens);
			leftNode = operatorNode;
		}
		else
			break;
	}
	return leftNode;
}

// Parse commands and subcommands within parentheses
t_ast* parseCommand(char **tokens, int *index, int numTokens) {
	return parseLogical(tokens, index, numTokens);
}

// Print the AST structure
void printAST(t_ast *node, int level) {
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
int main(int argc, char **argv)
{
	char command[] = "(echo start&&(echo continue||echo fallback1)|grep check>here.txt)&&echo end1>out1.txt>out2.txt||echo end2>out2.txt&&cat < input.txt>> out3.txt";
	char	**tokens;
	int		numTokens = 0;

	if (argc == 1)
		tokens = split_tokens(command);
	else
		tokens = split_tokens(argv[1]);
	while (tokens[numTokens])
		numTokens++;
	if (argc == 1)
		printf("\nINPUT: %s\n\n", command);
	else
		printf("\nINPUT: %s\n\n", argv[1]);
	printf("There are %i tokens!\nBut you cannot see parenthesis ()\n", numTokens);

	int index = 0;
	t_ast *astRoot = parseCommand(tokens, &index, numTokens); // Parse the tokens to build the AST

	printf("\t---> AST Structure:\n");
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
