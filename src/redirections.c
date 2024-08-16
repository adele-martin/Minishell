/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 16:52:14 by bschneid          #+#    #+#             */
/*   Updated: 2024/08/16 15:42:35 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

// not really sure what this function does and not right!
int	redirect(char *operator, char *word, char *tty_name)
{
	if (!ft_strncmp(operator, ">", 2))
		return (redirect_output(word));
	else if (!ft_strncmp(operator, ">>", 3))
		return (append_output(word));
	else if (!ft_strncmp(operator, "<", 2))
		return (redirect_input(word));
	else if (!ft_strncmp(operator, "<<", 3))
		return (heredoc(word, tty_name));
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
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		exit(1);
	}
	close(fd);
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
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		exit(1);
	}
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
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2");
		exit(1);
	}
	close(fd);
	return (0);
}

/*
Subshell: When a heredoc is encountered, it signals the shell to create a new child process 
known as a subshell. This subshell is a separate instance of the shell, created specifically 
to handle the commands within the heredoc block. So, a subshell can be imagined as a fork of 
the current shell process with the new shell inheriting the state of the parent shell but 
operating independently there.

Illustration of what happens:
When the shell encounters a heredoc block (<< EOF), it knows that everything between << EOF 
and EOF should be treated as input to a command.
The shell creates a subshell — a separate process that inherits the environment (variables, 
functions, etc.) from the parent shell.
The subshell executes the commands within the heredoc block as if you typed them directly 
into the terminal.
Any changes in the environment (variables, etc.) inside the subshell do not affect the parent 
shell. Once the subshell completes execution, it exits, and the parent shell continues where it left off.
*/

// for << operator
// int	heredoc(char *delimiter)
// {
// 	// (void)delimiter;
// 	// write(0, "This is a test\n", 16);
// 	char	*line;
// 	int	fd;

// 	fd = open(delimiter, O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 	// fd = open("src/heredoc.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 	if (fd < 0)
// 	{
// 		perror("open");
// 		exit(1);
// 	}
// 	while (1)
// 	{
// 		line = readline("> ");
// 		if (!line)
// 			break ;
// 		if (!ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1))
// 			break ;
// 		write(fd, line, ft_strlen(line));
// 		write(fd, "\n", 1);
// 	}
// 	close(fd);
// 	// fd = open("src/heredoc.txt", O_RDONLY);
// 	fd = open(delimiter, O_RDONLY);
// 	if (fd < 0)
// 	{
// 		perror("open");
// 		exit(1);
// 	}
// 	dup2(fd, STDIN_FILENO);
// 	close(fd);
// 	return (0);
// }

// for << operator with child process
int	heredoc(char *delimiter, char *tty_name)
{
	char	*line;
	int		tty_fd;
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
		tty_fd = open(tty_name, O_RDWR);
		if (tty_fd == -1) {
			perror("open terminal");
			return (1);
		}
		dup2(tty_fd, STDIN_FILENO);  // Restore STDIN
		dup2(tty_fd, STDOUT_FILENO); // Restore STDOUT
		close(tty_fd); 
		while (1)
		{
			line = readline("> ");
			if (!line)
				break ;
			if (!ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1))
			{
				free(line);
				break ;
			}
			write(fd[1], line, ft_strlen(line));
			write(fd[1], "\n", 1);
			free(line);
		}
		close(fd[1]);
		exit (0);
	}
	else
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		waitpid(id, NULL, 0);
		return (0);
	}
	close(fd[0]);
	close(fd[1]);
	// waitpid(id, NULL, 0);
	return (0);
}
