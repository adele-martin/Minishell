/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 17:14:13 by bschneid          #+#    #+#             */
/*   Updated: 2024/08/16 17:14:16 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

static void	print_ast_level(t_ast *node, int level);

// helper function to print out the elements of a linked list
void	print_args(char *str, t_list *linked_args)
{
	ft_printf("\t%s\n", str);
	while (linked_args)
	{
		ft_printf("%s\n", linked_args->content);
		linked_args = linked_args->next;
	}
}

// removes quotations from a string (but no quotes inside quotes) 
void	clean_quotations(char *str)
{
	char	*writer;
	char	in_sgl;
	char	in_dbl;

	in_sgl = 0;
	in_dbl = 0;
	writer = str;
	while (*str)
	{
		if (in_sgl || in_dbl)
		{
			if ((in_sgl && *str == '\'') || (in_dbl && *str == '"'))
			{
				in_sgl = 0;
				in_dbl = 0;
				str++;
				continue ;
			}
		}
		else if (*str == '\'' || *str == '"')
		{
			if (*str == '\'')
				in_sgl = 1;
			else if (*str == '"')
				in_dbl = 1;
			str++;
			continue ;
		}
		*(writer++) = *(str++);
	}
	*writer = '\0';
}

// Print the AST structure
void	print_ast(t_ast *root)
{
	if (!root)
		return ;
	printf("%s\n", root->value);
	print_ast_level(root->left, 1);
	print_ast_level(root->right, 1);
}

static void	print_ast_level(t_ast *node, int level)
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

// checks for right usage of parenthesis in token strings
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
