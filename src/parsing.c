/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 12:11:30 by bschneid          #+#    #+#             */
/*   Updated: 2024/08/06 17:38:19 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

// return 0 on success, 1/? on failure
int	parse_ast(t_ast *node)
{
	t_ast	*tmp_node;
	int		fd_in_cpy;
	int		fd_out_cpy;
	
	if (ft_strncmp(node->value, "&&", 3) == 0)	// if left executes successfully, execute right
	{
		// DONT FORK HERE - JUST CHECK IF LEFT EXECUTES SUCCESSFULLY
		fd_in_cpy = dup(0);
		fd_out_cpy = dup(1);
		if (parse_ast(node->left) == 0)
		{
			dup2(fd_in_cpy, 0);
			dup2(fd_out_cpy, 1);
			parse_ast(node->right);
		}
		close(fd_in_cpy);
		close(fd_out_cpy);
	}
	else if (ft_strncmp(node->value, "||", 3) == 0)
	{
		fd_in_cpy = dup(0);
		fd_out_cpy = dup(1);
		if (parse_ast(node->left) != 0)
		{
			dup2(fd_in_cpy, 0);
			dup2(fd_out_cpy, 1);
			parse_ast(node->right);
		}
		close(fd_in_cpy);
		close(fd_out_cpy);
	}
	else if (ft_strncmp(node->value, "|", 2) == 0)
	{
		// fd[0] is the read end
		// fd[1] is the write end
		int 	fd[2];
		pid_t	id;
		
		if (pipe(fd) == -1)
			return (1);
		id = fork();
		if (id == -1)
			return (1);
		if (id == 0)
		{
			close(fd[0]);
			dup2(fd[1], STDOUT_FILENO);
			close(fd[1]);
			parse_ast(node->left);
			return (0);
		}
		else
		{
			close(fd[1]);
			dup2(fd[0], STDIN_FILENO);
			close(fd[0]);
			parse_ast(node->right);
			return (0);
		}
		close(fd[0]);
		close(fd[1]);
		waitpid(id, NULL, 0);
	}
	// {
	// 	// fd[0] is the read end
	// 	// fd[1] is the write end
	// 	int 	fd[2];
	// 	pid_t	id1, id2;
		
	// 	if (pipe(fd) == -1)
	// 		return (1);
	// 	id1 = fork();
	// 	if (id1 == -1)
	// 		return (1);
	// 	if (id1 == 0)
	// 	{
	// 		close(fd[0]);
	// 		dup2(fd[1], STDOUT_FILENO);
	// 		close(fd[1]);
	// 		parse_ast(node->left);
	// 		return (0);
	// 	}
	// 	else
	// 	{
	// 		id2 = fork();
	// 		if (id2 == -1)
	// 			return (1);
	// 		if (id2 == 0)
	// 		{
	// 			close(fd[1]);
	// 			dup2(fd[0], STDIN_FILENO);
	// 			close(fd[0]);
	// 			parse_ast(node->right);
	// 			return (0);
	// 		}
	// 	}
	// 	close(fd[0]);
	// 	close(fd[1]);
	// 	waitpid(id1, NULL, 0);
	// 	waitpid(id2, NULL, 0);
	// }
	else if (is_redirection(node->value))
	{
		tmp_node = node;
		while (is_redirection(tmp_node->right->value))
		{
			redirect(tmp_node->value, tmp_node->right->left->value);	
			tmp_node = tmp_node->right;
		}
		redirect(tmp_node->value, tmp_node->right->value);
		parse_ast(node->left);
	}
	else
		return (execute(node->value));
	// {
	// 	pid_t	id;
	// 	id = fork();
	// 	if (id == -1)
	// 		return (1);
	// 	if (id == 0)
	// 	{
	// 		ft_printf("Run the cmd: %s\n", node->value);
	// 		execute(node->value);
	// 		exit (0);
	// 	}
	// 	else
	// 		waitpid(id, NULL, 0);
	// }
	return (0);
}
