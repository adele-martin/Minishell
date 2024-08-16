/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 12:11:30 by bschneid          #+#    #+#             */
/*   Updated: 2024/08/16 18:25:22 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

int	parse_and_or(t_ast *node, s_data *data);

// return 0 on success, 1/? on failure
int	parse_ast(t_ast *node, s_data *data)
{
	if (!ft_strncmp(node->value, "&&", 3) || !ft_strncmp(node->value, "||", 3))
		parse_and_or(node, data);
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
			parse_ast(node->left, data);
			return (0);
		}
		else
		{
			close(fd[1]);
			dup2(fd[0], STDIN_FILENO);
			close(fd[0]);
			waitpid(id, NULL, 0);	 // NEW
			// ft_printf("waited for left pipe with id %d\n", id);
			parse_ast(node->right, data);
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
			redirect(tmp_node->value, tmp_node->right->left->value, data);	
			tmp_node = tmp_node->right;
		}
		redirect(tmp_node->value, tmp_node->right->value, data);
		parse_ast(node->left, data);
	}
	else
		return (execute(node->value));
	return (0);
}

int	parse_and_or(t_ast *node, s_data *data)
{
	pid_t	id;
	int		status;

	id = fork();
	if (id == -1)
		return (1);
	if (id == 0)
		exit(parse_ast(node->left, data));
	else
		waitpid(id, &status, 0);
	if (ft_strncmp(node->value, "&&", 3) == 0)	// if left execute successfully, execute right
	{
		if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
			parse_ast(node->right, data);
	}
	else if (ft_strncmp(node->value, "||", 3) == 0)	// if left DIDN'T execute successfully, execute right
	{
		if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
			parse_ast(node->right, data);
	}
	return (0);
}
