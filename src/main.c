/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 17:34:05 by bschneid          #+#    #+#             */
/*   Updated: 2024/08/16 18:11:16 by bschneid         ###   ########.fr       */
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
	t_data	data;
	// t_list *expand_list;
	(void)argv;
	
	data.list_envs = envs_list(envp);		// Copied envp - is it shielded??
	// for char	*expanding(char *variable, char **list, t_list *head)
	// expanding(char *variable, list_envs, NULL);
	data.tty_name = ttyname(STDIN_FILENO);
	if (argc != 1)
	{
		errno = EINVAL;
		perror("main");
		exit (EXIT_FAILURE);
	}
	handle_signals();
	while (1)
	{
		data.input = readline("minishell > ");
		if (!input)
			break ;
		else
		{
			add_history(data.input);
			// free(input);
			id = fork();
			if (id == -1)
				return (1);
			if (id == 0)
			{
				tokenize_parse(&data);
				tokens = split_tokens(input);
				end_tokens = tokens;
				while (*end_tokens)
					end_tokens++;
				end_tokens--;
				astRoot = create_ast(tokens, end_tokens); // Parse the tokens to build the AST
				parse_ast(astRoot, ); // Parse the AST to execute the commands
			}
			else
			{
				waitpid(id, NULL, 0);
				free(data.input);
			}
		}
	}
	printf("Shell ended!\n");
	return (EXIT_SUCCESS);
}
