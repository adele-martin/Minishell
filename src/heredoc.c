/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 12:25:35 by bschneid          #+#    #+#             */
/*   Updated: 2024/09/13 17:16:22 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

static char	*heredoc_child(char *delimiter);

// for << operator with child process
int	heredoc(char *delimiter, t_data *data)
{
	int 	fd[2];
	pid_t	id;
	char	*line;

	if (pipe(fd) == -1)
		return (1);
	id = fork();
	if (id == -1)
		return (1);
	if (id == 0)
	{
		handle_signals(0);
		handle_signals(3);
		close(fd[0]);
		line = NULL;
		g_signal = 0;
		if (!restore_stdin_stdout(data, 2))
			exit (ft_free(data, 1));
		line = heredoc_child(delimiter);
		if (line)
			write(fd[1], line, ft_strlen(line));
		free(line);
		ft_printf("END CHILD\n");
		close(fd[1]);
		exit(ft_free(data, 0));
	}
	close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	waitpid(id, &data->status, 0);

	// ft_printf("WAITED FOR CHILD with status %i\n", data->status);
	// if (WIFEXITED(data->status))
	// 	ft_printf("Exited with status: %i\n", WEXITSTATUS(data->status));
	// if (g_signal)
	// 	restore_stdin_stdout(data, 2);
	return (g_signal);
}

static char	*heredoc_child(char *delimiter)
{
	char	*new_line;
	char	*tmp_line;
	char	*out;

	out = ft_strdup("");
	while (1)
	{
		new_line = readline("> ");
		if (g_signal == 130)
		{
			return (free(out), NULL);
		}
		if (!new_line)
		{
			error_message("warning", "here-document delimited by end-of-file - wanted", delimiter);
			break ;
		}
		if (!ft_strncmp(new_line, delimiter, ft_strlen(delimiter) + 1))
		{
			return (free(new_line), out);
		}
		tmp_line = ft_strjoin(out, new_line);
		free(out);
		free(new_line);
		if (!tmp_line)
			return (NULL);
		out = ft_strjoin(tmp_line, "\n");
		free(tmp_line);
		if (!out)
			return (NULL);
	}
	return (out);
}
