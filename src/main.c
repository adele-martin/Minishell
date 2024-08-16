/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 17:34:05 by bschneid          #+#    #+#             */
/*   Updated: 2024/08/16 17:07:14 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

// TODO: Why is there the function getenv if we get it through the envp ? Is there a difference? --> cd-build-in !
int	main(int argc, char **argv, char **envp)
{
	char	*input;
	char	**tokens;
	char	**end_tokens;
	t_ast	*astRoot;
	pid_t	id;
	// t_info	info;
	// char **list_envs = envs_list(envp);

	(void)argv;
	(void)envp;
	if (argc != 1)
	{
		errno = EINVAL;
		perror("main");
		exit (EXIT_FAILURE);
	}

	// t_list *head = NULL;

	// add_node(&head, "USER=john_doe");
	// add_node(&head, "PATH=/usr/bin");
	// add_node(&head, "HOME=/home/john");
	// add_node(&head, "HELLO=/bin/bash");

	// expanding(argv, head);
	// print_list(head);

	handle_signals();
	while (1)
	{
		input = readline("minishell > ");
		if (!input)
			break ;
		else
		{
			// printf("INPUT: %s\n", input);
			add_history(input);
			// free(input);
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
				parse_ast(astRoot, ttyname(STDIN_FILENO)); // Parse the AST to execute the commands
			}
			else
				waitpid(id, NULL, 0);
			free(input);
		}
	}
	printf("Shell ended!\n");
	return (EXIT_SUCCESS);
}
