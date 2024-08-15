/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ademarti <ademarti@student.42berlin.de     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 17:34:05 by bschneid          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2024/08/15 13:42:08 by ademarti         ###   ########.fr       */
=======
/*   Updated: 2024/08/09 18:56:40 by bschneid         ###   ########.fr       */
>>>>>>> 0f26f591eecd9922b2083a19f20675a9278bc748
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

// TODO: Why is there the function getenv if we get it through the envp ? Is there a difference? --> cd-build-in !
int	main(int argc, char **argv, char **envp)
{
	// t_info	info;
<<<<<<< HEAD
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
=======
	char	*input;
	char	**tokens;
	char	**end_tokens;
	t_ast	*astRoot;
	pid_t	id;

	(void)argc;
	(void)argv;
	(void)envp;
	if (argc != 1)
	{
		errno = EINVAL;
		perror("main");
		exit (EXIT_FAILURE);
	}
	while (1)
	{
		input = readline("minishell > ");
		if (!input)
			break ;
		else
		{
			// printf("INPUT: %s\n", input);
			add_history(input);
			id = fork();
			if (id == -1)
				return (1);
			if (id == 0)
			{
				tokens = split_tokens(input);
				end_tokens = tokens;
				while (*end_tokens)
					end_tokens++;
				end_tokens--;
				astRoot = create_ast(tokens, end_tokens); // Parse the tokens to build the AST
				parse_ast(astRoot); // Parse the AST to execute the commands
			}
			else
				waitpid(id, NULL, 0);
			free(input);
		}
	}
	printf("Shell ended!\n");
	return (EXIT_SUCCESS);
>>>>>>> 0f26f591eecd9922b2083a19f20675a9278bc748
}
