/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ademarti <ademarti@student.42berlin.de     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 17:34:05 by bschneid          #+#    #+#             */
/*   Updated: 2024/08/07 15:46:02 by ademarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	// t_info	info;
	// char	*input;
	// char	**tokens;
	// char	**end_tokens;
	// t_ast	*astRoot;
	(void)argc;
	(void)argv;

	char **list_envs = envs_list(envp);
	list_envs = update_list("ZDOTDIR=hey", list_envs);

	int i = 0;
	while (list_envs[i])
	{
		printf("%s\n", list_envs[i]);
		i++;
	}

	// while (1)
	// {
	// 	input = readline("\033[1;32mminishell > \033[0m");
	// 	if (!input)
	// 		break ;
	// 	else
	// 	{
	// 		printf("INPUT: %s\n", input);
	// 		add_history(input);
	// 		tokens = split_tokens(input);
	// 		// evaluate(input, &info);

	// 		end_tokens = tokens;
	// 		while (*end_tokens)
	// 			end_tokens++;
	// 		end_tokens--;
	// 		astRoot = create_ast(tokens, end_tokens); // Parse the tokens to build the AST

	// 		print_ast(astRoot);
	// 		free(input);
	// 	}
	// }
	// printf("Shell ended!\n");
	return (0);
}

// example command:
// (echo start&&(echo continue||echo fallback1)|grep check>here.txt)&&echo end1>out1.txt>out2.txt||echo end2>out2.txt&&cat < input.txt>> out3.txt
