/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ademarti <ademarti@student.42berlin.de     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 17:34:05 by bschneid          #+#    #+#             */
/*   Updated: 2024/08/12 19:58:57 by ademarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

t_list *create_node_(char *content)
{
    t_list *new_node = (t_list *)malloc(sizeof(t_list));
    if (!new_node)
        return NULL;
    new_node->content = strdup(content);
    new_node->next = NULL;
    return new_node;
}

void add_node(t_list **head, char *content)
{
    t_list *new_node = create_node_(content);
    if (!new_node)
        return;

    new_node->next = *head;
    *head = new_node;
}

void print_list(t_list *head)
{
    t_list *temp = head;
    while (temp != NULL)
    {
        printf("%s\n", (char *)temp->content);
        temp = temp->next;
    }
}

int	main(int argc, char **argv, char **envp)
{
	// t_info	info;
	char	*input;
	char	**tokens;
	char	**end_tokens;
	t_ast	*astRoot;

	while (1)
	{
		input = readline("\033[1;32mminishell > \033[0m");
		if (!input)
			break ;
		else
		{
			printf("INPUT: %s\n", input);
			add_history(input);
			tokens = split_tokens(input);
			// evaluate(input, &info);

			end_tokens = tokens;
			while (*end_tokens)
				end_tokens++;
			end_tokens--;
			astRoot = create_ast(tokens, end_tokens); // Parse the tokens to build the AST

			print_ast(astRoot);
			free(input);
		}
	}
	printf("Shell ended!\n");
	return (0);
}
