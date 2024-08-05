/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 15:16:19 by bschneid          #+#    #+#             */
/*   Updated: 2024/08/05 18:32:14 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
void	print_ast_level(t_ast *node, int level)
{
	int	i;

	i = 0;
	if (node)
	{
		while (i++ < level)
			printf("    ");
		printf("%s\n", node->value);
		print_ast_level(node->left, level + 1);
		print_ast_level(node->right, level + 1);
	}
}

void	print_ast(t_ast *root)
{
	if (!root)
		return ;
	printf("%s\n", root->value);
	print_ast_level(root->left, 1);
	print_ast_level(root->right, 1);
}
// int main(int argc, char **argv)
// {
// 	char command[] = "(echo start&&(echo continue||echo fallback1)|grep check>here.txt)&&echo end1>out1.txt>out2.txt||echo end2>out2.txt&&cat < input.txt>> out3.txt";
// 	char	**tokens;
// 	char	**end_tokens;
// 	int		numTokens = 0;

// 	if (argc == 1)
// 		tokens = split_tokens(command);
// 	else
// 		tokens = split_tokens(argv[1]);
// 	while (tokens[numTokens])
// 		numTokens++;
// 	if (argc == 1)
// 		printf("\nINPUT: %s\n\n", command);
// 	else
// 		printf("\nINPUT: %s\n\n", argv[1]);
// 	printf("There are %i tokens!\nBut you cannot see parenthesis ()\n", numTokens);

// 	end_tokens = tokens;
// 	while (*end_tokens)
// 		end_tokens++;
// 	end_tokens--;
// 	if (!check_parenthesis(tokens, end_tokens))
// 		printf("\nPARENTHESIS ERROR !!!\n\n");
// 	printf("Creating AST ...\n");
// 	t_ast *astRoot = create_ast(tokens, end_tokens); // Parse the tokens to build the AST

// 	printf("\t---> AST Structure:\n");
// 	printAST(astRoot, 0); // Print the AST structure

// 	// Free memory (not shown for brevity)
// 	return 0;
// }

/*
Explanation

    Tokenize the Input: This remains unchanged, splitting the command string into individual tokens.
    parsePrimary: Now includes handling for parentheses. If a ( is encountered, it calls parseCommand to parse the subexpression within the parentheses.
    parsePipe: Handles the pipe operator (|) as before.
    parseLogical: Handles logical operators (&&, ||) as before.
    parseCommand: Entry point function for parsing commands and subcommands within parentheses. It calls parseLogical to parse the command.
*/

/*
CARE PRECEDENCE OF OPERATORS!

1. Redirections (<, >, >>, <>, <&. >& and >>-, as well as here-docs
	<<delimiter and here-strings <<<word) are roughly the same as 
	command-line arguments, and can appear anywhere in a simple command, 
	including before the command word. Effectively, they bind most tightly, 
	as with postfix operators in most languages.
2. Pipes (|) are the strongest binary operator. They associate to the left.
3. Finally come the short-circuiting booleans (&& and ||). 
	Unlike many languages, these have the same precedence. 
	They also associate to the left.



control operator: A token that performs a control function. 
	It is a newline or one of the following: 
	‘||’, ‘&&’, ‘&’, ‘;’, ‘;;’, ‘;&’, ‘;;&’, ‘|’, ‘|&’, ‘(’, or ‘)’.
metacharacter: A character that, when unquoted, separates words. 
	A metacharacter is a space, tab, newline, or one of the following characters: 
	‘|’, ‘&’, ‘;’, ‘(’, ‘)’, ‘<’, or ‘>’. 
token: A sequence of characters considered a single unit by the shell. 
	It is either a word or an operator.
word: A sequence of characters treated as a unit by the shell.
	Words may not include unquoted metacharacters. 


Important for lexing:
	- parenthesis for priority				()
	- single quotes (no interpretation)		''
	- double quotes (only $-interpretation)	""
	- dollar sign (environment variables)	$
	- exit status of foreground pipeline	$?
	- wildcards in curr. working directory	*

	- redirect input 						<
	- redirect output 						>
	- read input until delimiter 			<<
	- redirect output in append mode 		>>
	- output to next input -> pipes 		|
	- AND-execution							&&
	- OR-execution							||

Not interpret unclosed quotes or special characters which are not required by the
subject such as \ (backslash) or ; (semicolon).
*/

/*
GET ENVIRONMENTS: env / printenv
GET VALUE OF VAR: " printenv varname "
EXPORT GLOBAL VAR: " export varname=value "
EXPORT LOCAL VAR: " varname =value " (or " set varname =value "
CREATE NEW ENV VAR: " export varname=value "
REMOVE ENV VAR: " unset varname "
*/

/*
Fehler in der Ausgabe umleiten:
Find everything in root (/) ending in .log, where errors (2) sent to /dev/null
find / -name *.log 2> /dev/null 
*/

/*
What is the differnce between ` and ' ???

In summary, single quotes stop all interpretation -- the string is rendered 
literally. Double quotes leave $ (dollar sign), ` (backquote) as special, 
and \ (backslash) as special when followed by certain other characters. 
And ! will be treated specially
*/

