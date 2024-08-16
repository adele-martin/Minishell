/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 12:11:30 by bschneid          #+#    #+#             */
/*   Updated: 2024/08/16 15:38:38 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

// return 0 on success, 1/? on failure
int	parse_ast(t_ast *node, char *tty_name)
{
	if (ft_strncmp(node->value, "&&", 3) == 0)	// if left execute successfully, execute right
	{
		pid_t	id;
		int		status;
		
		id = fork();
		if (id == -1)
			return (1);
		if (id == 0)
		{
			parse_ast(node->left, tty_name);
			exit(0);
		}
		else
			waitpid(id, &status, 0);
		if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
			parse_ast(node->right, tty_name);
	}
	else if (ft_strncmp(node->value, "||", 3) == 0)	// if left DIDN'T execute successfully, execute right
	{
		pid_t	id;
		int		status;
		
		id = fork();
		if (id == -1)
			return (1);
		if (id == 0)
		{
			parse_ast(node->left, tty_name);
			exit(0);
		}
		else
			waitpid(id, &status, 0);
		if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
			parse_ast(node->right, tty_name);
	}
	else if (ft_strncmp(node->value, "|", 2) == 0)	// TODO: this may interfere with heredoc which needs writing in STDOUT 
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
			parse_ast(node->left, tty_name);
			return (0);
		}
		else
		{
			close(fd[1]);
			dup2(fd[0], STDIN_FILENO);
			close(fd[0]);
			waitpid(id, NULL, 0);	 // NEW
			// ft_printf("waited for left pipe with id %d\n", id);
			parse_ast(node->right, tty_name);
			return (0);
		}
		close(fd[0]);
		close(fd[1]);
		waitpid(id, NULL, 0);
	}
	else if (is_redirection(node->value))
	{
		t_ast	*tmp_node;
		
		tmp_node = node;
		while (is_redirection(tmp_node->right->value))
		{
			redirect(tmp_node->value, tmp_node->right->left->value, tty_name);	
			tmp_node = tmp_node->right;
		}
		redirect(tmp_node->value, tmp_node->right->value, tty_name);
		parse_ast(node->left, tty_name);
	}
	else
		return (execute(node->value));
	return (0);
}
