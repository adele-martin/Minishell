/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 16:52:14 by bschneid          #+#    #+#             */
/*   Updated: 2024/08/06 17:37:58 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

// not really sure what this function does and not right!
int	redirect(char *operator, char *word)
{
	if (!ft_strncmp(operator, ">", 2)) 
		return (redirect_output(word));
	else if (!ft_strncmp(operator, ">>", 3))
		return (append_output(word));
	else if (!ft_strncmp(operator, "<", 2))
		return (redirect_input(word));
	else if (!ft_strncmp(operator, "<<", 3))
		return (heredoc(word));
	return (1);
}


// for > operator
int	redirect_output(char *filename)
{
	int	fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0755);
	if (fd < 0)
	{
		perror("open");
		exit(1);
	}
	// ft_printf("Opened file %s with fd %d\n", filename, fd);
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		exit(1);
	}
	// ft_printf("DUPPED...\n");
	close(fd);
	// ft_printf("Redirected output to %s\n", filename);
	return (0);
}

// for >> operator
int	append_output(char *filename)
{
	int	fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		perror("open");
		exit(1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

// for < operator
int	redirect_input(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		perror("open");
		exit(1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

// for << operator
int	heredoc(char *delimiter)
{
	char	*line;
	int	fd;

	fd = open("src/heredoc.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("open");
		exit(1);
	}
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (!ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1))
			break ;
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
	}
	close(fd);
	fd = open("src/heredoc.txt", O_RDONLY);
	if (fd < 0)
	{
		perror("open");
		exit(1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}
