/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ademarti <ademarti@student.42berlin.de     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 17:34:05 by bschneid          #+#    #+#             */
/*   Updated: 2024/08/09 18:56:40 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

// TODO: Why is there the function getenv if we get it through the envp ? Is there a difference? --> cd-build-in !
int	main(int argc, char **argv, char **envp)
{
	// t_info	info;
	char	*input;
	char	**tokens;
	char	**end_tokens;
	t_ast	*astRoot;
	pid_t	id;

	(void)argv;
	(void)envp;
	if (argc != 1)
	{
		errno = EINVAL;
		perror("main");
		exit (EXIT_FAILURE);
	}
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
				parse_ast(astRoot); // Parse the AST to execute the commands
			}
			else
				waitpid(id, NULL, 0);
			free(input);
		}
	}
	printf("Shell ended!\n");
	return (EXIT_SUCCESS);
}
