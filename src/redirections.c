/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ademarti <adelemartin@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 16:52:14 by bschneid          #+#    #+#             */
/*   Updated: 2024/09/10 16:40:56 by ademarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

// not really sure what this function does and not right!
int	redirect(char *operator, char *word, t_data *data)
{
	if (!ft_strncmp(operator, ">", 2))
		return (redirect_output(word, data));
	else if (!ft_strncmp(operator, ">>", 3))
		return (append_output(word, data));
	else if (!ft_strncmp(operator, "<", 2))
		return (redirect_input(word, data));
	else if (!ft_strncmp(operator, "<<", 3))
		return (heredoc(word, data));
	return (1);
}

// for > operator
int	redirect_output(char *filename, t_data *data)
{
	int	fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0755);
	if (fd < 0)
	{
		perror("open");
		exit(ft_free(data, 1));
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		exit(ft_free(data, 1));
	}
	close(fd);
	return (0);
}

// for >> operator
int	append_output(char *filename, t_data *data)
{
	int	fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		perror("open");
		exit(ft_free(data, 1));
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		exit(ft_free(data, 1));
	}
	close(fd);
	return (0);
}

// for < operator
int	redirect_input(char *filename, t_data *data)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		perror("open");
		exit(ft_free(data, 1));
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		exit(ft_free(data, 1));
	}
	close(fd);
	return (0);
}
