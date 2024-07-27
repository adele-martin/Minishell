

#include "../header/minishell.h"

// Function to create a new AST node
t_ast	*create_node(char *value)
{
	t_ast	*node;
	
	node = (t_ast*)malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->value = value;
	node->left = NULL;
	node->right = NULL;
	return node;
}

// returns 1 on right usage of parenthesis, else 0
char	check_parenthesis(char **token_start, char **token_end)
{
	int		par;

	par = 0;
	while (token_start <= token_end)
	{
		if (ft_strncmp(*token_start, "(", 1) == 0)
			par++;
		else if (ft_strncmp(*token_start, ")", 1) == 0)
			par--;
		if (par < 0)
			return (0);
		token_start++;
	}
	if (par)
		return (0);
	return (1);
}

t_ast	*create_ast(char **token_start, char **token_end)
{
	t_ast	*node;
	char	**token_search;
	char	par;

	par = 0;
	if (token_start > token_end)
		return (NULL);
	else if (token_start == token_end)
		return (create_node(*token_start));
	token_search = token_start;
	while (token_search <= token_end)
	{
		if (ft_strncmp(*token_search, "(", 1) == 0)
			par++;
		else if (ft_strncmp(*token_search, ")", 1) == 0)
			par--;
		else if (!par && (!ft_strncmp(*token_search, "&&", 2) || !ft_strncmp(*token_search, "||", 2)))
		{
			node = create_node(*token_search);
			node->left = create_ast(token_start, token_search - 1);
			node->right = create_ast(token_search + 1, token_end);
			return (node);
		}
		token_search++;
	}
	token_search = token_start;
	while (token_search <= token_end)
	{
		if (ft_strncmp(*token_search, "(", 1) == 0)
			par++;
		else if (ft_strncmp(*token_search, ")", 1) == 0)
			par--;
		else if (!par && !ft_strncmp(*token_search, "|", 1))
		{
			node = create_node(*token_search);
			node->left = create_ast(token_start, token_search - 1);
			node->right = create_ast(token_search + 1, token_end);
			return (node);
		}
		token_search++;
	}
	token_search = token_start;
	while (token_search <= token_end)
	{
		if (ft_strncmp(*token_search, "(", 1) == 0)
			par++;
		else if (ft_strncmp(*token_search, ")", 1) == 0)
			par--;
		else if (!par && (!ft_strncmp(*token_search, ">", 1) || !ft_strncmp(*token_search, ">>", 2) || 
				!ft_strncmp(*token_search, "<", 1) || !ft_strncmp(*token_search, "<<", 2)))
		{
			node = create_node(*token_search);
			node->left = create_ast(token_start, token_search - 1);
			node->right = create_ast(token_search + 1, token_end);
			return (node);
		}
		token_search++;
	}
	return (create_ast(token_start + 1, token_end - 1));
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

int main(int argc, char **argv)
{
	char command[] = "(echo start&&(echo continue||echo fallback1)|grep check>here.txt)&&echo end1>out1.txt>out2.txt||echo end2>out2.txt&&cat < input.txt>> out3.txt";
	char	**tokens;
	char	**end_tokens;
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

	end_tokens = tokens;
	while (*end_tokens)
		end_tokens++;
	end_tokens--;
	if (!check_parenthesis(tokens, end_tokens))
		printf("\nPARENTHESIS ERROR !!!\n\n");
	printf("Creating AST ...\n");
	t_ast *astRoot = create_ast(tokens, end_tokens); // Parse the tokens to build the AST

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
