/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 12:11:30 by bschneid          #+#    #+#             */
/*   Updated: 2024/08/05 15:04:03 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

/*
struct s_ast {
	char	*value;	// command, operator, filename
	t_ast	*left;	// Left child node
	t_ast	*right;	// Right child node
};
INPUT: (echo start&&(echo continue||echo fallback1)|grep check>here.txt)&&echo end1>
			out1.txt>out2.txt||echo end2>out2.txt&&cat < input.txt>> out3.txt
&&
    &&
        echo start
        |
            ||
                echo continue
                echo fallback1
            >
                grep check
                here.txt
    ||
        >
            echo end1
            >
                out1.txt
                out2.txt
        &&
            >
                echo end2
                out2.txt
            <
                cat
                >>
                    input.txt
                    out3.txt

*/
// return 0 on success, 1/? on failure
int	parse_ast(t_ast *node)
{
	if (ft_strncmp(node->value, "&&", 3) == 0)	// if left executes successfully, execute right
	{
		// fork once first and check if left executes successfully; run right if left succeeds
		if (parse_ast(node->left) == 0)
			parse_ast(node->right);
	}
	else if (ft_strncmp(node->value, "||", 3) == 0)
	{
		// fork once first and check if left executes successfully; run right if left fails
		if (parse_ast(node->left) != 0)
			parse_ast(node->right);
	}
	else if (ft_strncmp(node->value, "|", 2) == 0)
	{
		// fd_in and fd_out is set before
		// create the pipe:
		// int fd[2]
		// int pipe(fd)
		// fd[0] is the read end
		// fd[1] is the write end
		// on left child: close(fd[0]) and dup2(fd[1], STDOUT_FILENO)
		// on right child: close(fd[1]) and dup2(fd[0], STDIN_FILENO)
		
	}
	else if (ft_strncmp(node->value, ">", 2) == 0)
	{
		// dup the file descriptor to STDOUT_FILENO
		// get the file descriptor of the file in right node and dup2 it to STDOUT_FILENO
			// if there is another redirection in the right node, go recursively
		// execute the command in the left node
		// restore the STDOUT_FILENO
	}
	else if (ft_strncmp(node->value, ">>", 3) == 0)
	{
		// same as > but with O_APPEND flag
	}
	else if (ft_strncmp(node->value, "<", 2) == 0)
	{
		// dup the file descriptor to STDIN_FILENO
		// get the file descriptor of the file in right node and dup2 it to STDIN_FILENO
			// if there is another redirection in the right node, go recursively
		// execute the command in the left node
		// restore the STDIN_FILENO
	}
	else
	{
		// fork and execute the command
	}
}
