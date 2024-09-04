/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 12:11:30 by bschneid          #+#    #+#             */
/*   Updated: 2024/09/04 13:20:02 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

int	parse_and_or(t_ast *node, t_data *data);

// return 0 on success, 1/? on failure
int	parse_ast(t_ast *node, t_data *data)
{
	if (!ft_strncmp(node->value, "&&", 3) || !ft_strncmp(node->value, "||", 3))
		return(parse_and_or(node, data));
	else if (ft_strncmp(node->value, "|", 2) == 0)
	{
		int		fd[2];

		if (pipe(fd) == -1)
			return (1);
		data->in_pipe = 1;
		data->id = fork();
		if (data->id == -1)
		{
			close(fd[0]);
			close(fd[1]);
			return (1);
		}
		if (data->id == 0)	// child parses left side
		{
			close(fd[0]);
			data->signal_fd = fd[1];
			dup2(fd[1], STDOUT_FILENO);
			exit(parse_ast(node->left, data));
		}					// parent parses right side
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		read(fd[0], &data->in_pipe, sizeof(char));
		close(fd[0]);
		// ft_printf("waited for left pipe with id %d\n", data->id);
		return (parse_ast(node->right, data));
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
		return (parse_ast(node->left, data));
	}
	return (execute(node->value, data));
}

int	parse_and_or(t_ast *node, t_data *data)
{
	g_signal = parse_ast(node->left, data);
	free(data->status_str);
	data->status_str = ft_itoa(g_signal);
	if (ft_strncmp(node->value, "&&", 3) == 0)
	{
		if (!data->status)
			return (parse_ast(node->right, data));
		else
			return (data->status);
	}
	else if (ft_strncmp(node->value, "||", 3) == 0)
	{
		if (data->status)
			return (parse_ast(node->right, data));
		else
			return (data->status);
	}
	return (0);
}

// int	parse_and_or(t_ast *node, t_data *data)
// {
// 	data->id = fork();
// 	if (data->id == -1)
// 		return (1);
// 	if (data->id == 0)
// 		exit(parse_ast(node->left, data));
// 	else
// 		waitpid(data->id, &data->status, 0);
// 	if (ft_strncmp(node->value, "&&", 3) == 0)
// 	{
// 		if (WIFEXITED(data->status) && !WEXITSTATUS(data->status))
// 			parse_ast(node->right, data);
// 	}
// 	else if (ft_strncmp(node->value, "||", 3) == 0)
// 	{
// 		if (WIFEXITED(data->status) && WEXITSTATUS(data->status))
// 			parse_ast(node->right, data);
// 	}
// 	return (0);
// }
