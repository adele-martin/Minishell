/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ademarti <ademarti@student.42berlin.de     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 17:34:05 by bschneid          #+#    #+#             */
/*   Updated: 2024/08/15 14:04:17 by ademarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

// TODO: Why is there the function getenv if we get it through the envp ? Is there a difference? --> cd-build-in !
int	main(int argc, char **argv, char **envp)
{
	// t_info	info;
	// char	*input;
	// char	**tokens;
	// char	**end_tokens;
	// t_ast	*astRoot;

	(void)envp;
	(void)argc;
	(void)argv;
	// char **list_envs = envs_list(envp);

	t_list *head = NULL;

	add_node(&head, "USER=john_doe");
	add_node(&head, "PATH=/usr/bin");
	add_node(&head, "HOME=/home/john");
	add_node(&head, "HELLO=/bin/bash");

	expanding(argv, head);
	// print_list(head);
// 	while (1)
// 	{
// 		input = readline("\033[1;32mminishell > \033[0m");
// 		if (!input)
// 			break ;
// 		else
// 		{
// 			printf("INPUT: %s\n", input);
// 			add_history(input);
// 			tokens = split_tokens(input);
// 			// evaluate(input, &info);

// 			end_tokens = tokens;
// 			while (*end_tokens)
// 				end_tokens++;
// 			end_tokens--;
// 			astRoot = create_ast(tokens, end_tokens); // Parse the tokens to build the AST

// 			print_ast(astRoot);
// 			free(input);
// 		}
// 	}
// 	printf("Shell ended!\n");
// 	return (0);
}
