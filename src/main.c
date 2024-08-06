/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 17:34:05 by bschneid          #+#    #+#             */
/*   Updated: 2024/08/06 17:38:40 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

int	main(void)
{
	// t_info	info;
	char	*input;
	char	**tokens;
	char	**end_tokens;
	t_ast	*astRoot;
	int		fd_in_cpy;
	int		fd_out_cpy;

	fd_in_cpy = dup(0);
	fd_out_cpy = dup(1);
	while (1)
	{
		dup2(fd_in_cpy, 0);
		dup2(fd_out_cpy, 1);
		input = readline("minishell > ");
		if (!input)
			break ;
		else
		{
			// printf("INPUT: %s\n", input);
			add_history(input);
			tokens = split_tokens(input);
			end_tokens = tokens;
			while (*end_tokens)
				end_tokens++;
			end_tokens--;
			astRoot = create_ast(tokens, end_tokens); // Parse the tokens to build the AST
			// print_ast(astRoot);
			parse_ast(astRoot); // Parse the AST to execute the commands
			free(input);
		}
	}
	printf("Shell ended!\n");
	return (0);
}

// example command:
// (echo start&&(echo continue||echo fallback1)|grep check>here.txt)&&echo end1>out1.txt>out2.txt||echo end2>out2.txt&&cat < input.txt>> out3.txt
