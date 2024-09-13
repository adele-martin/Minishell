/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 12:11:30 by bschneid          #+#    #+#             */
/*   Updated: 2024/09/13 19:01:29 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

static int	parse_and_or(t_ast *node, t_data *data);
static int	handle_redirection(t_ast *node, t_data *data);

// parse_ast is the main function for parsing the AST
int	parse_ast(t_ast *node, t_data *data)
{
	if (!node)
		return (0);
	if (!ft_strncmp(node->value, "&&", 3) || !ft_strncmp(node->value, "||", 3))
		return (parse_and_or(node, data));
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
		if (data->id == 0)
		{
			data->in_child = 1;
			handle_signals(3);
			close(fd[0]);
			data->signal_fd = fd[1];
			dup2(fd[1], STDOUT_FILENO);
			exit(ft_free(data, parse_ast(node->left, data)));
		}
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		read(fd[0], &data->in_pipe, sizeof(char));
		close(fd[0]);
		return (parse_ast(node->right, data));
	}
	else if (is_redirection(node->value))
		return (handle_redirection(node, data));
	return (execute(node->value, data));
}

static int	parse_and_or(t_ast *node, t_data *data)
{
	g_signal = parse_ast(node->left, data);
	if (ft_strncmp(node->value, "&&", 3) == 0)
	{
		if (!g_signal)
			return (parse_ast(node->right, data));
		else
			return (g_signal);
	}
	else if (ft_strncmp(node->value, "||", 3) == 0)
	{
		if (g_signal)
			return (parse_ast(node->right, data));
		else
			return (g_signal);
	}
	return (0);
}

static int	handle_redirection(t_ast *node, t_data *data)
{
	t_ast	*tmp;

	tmp = node;
	while (is_redirection(tmp->right->value))
	{
		data->status = redirect(tmp->value, tmp->right->left->value, data);
		if (data->status)
			return (data->status);
		tmp = tmp->right;
	}
	data->status = redirect(tmp->value, tmp->right->value, data);
	if (data->status)
		return (data->status);
	return (parse_ast(node->left, data));
}
