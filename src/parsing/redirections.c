/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 16:52:14 by bschneid          #+#    #+#             */
/*   Updated: 2024/09/16 22:38:24 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

// not really sure what this function does and not right!
int	redirect(char *operator, char **word, t_data *data)
{
	t_list	*wordlist;

	clean_quotations(*word, *word, 0, 0);
	if (!ft_strncmp(operator, "<<", 3))
		return (heredoc(*word, data));
	
	wordlist = ft_lstnew(ft_strdup(*word));

	if (!add_wildcards(data, &wordlist))
		return (error_message(NULL, NULL, "Error in wildcards"), 1);
	if (wordlist->next)
	{
		ft_lstclear(&wordlist, free);
		return (error_message(NULL, NULL, "ambiguous redirect"), 1);
	}
	if (!expand_variables(wordlist, data))
		return (error_message(NULL, NULL, "Error in expanding variables"), 1);
	*word = ft_strdup(wordlist->content);
	free(wordlist);
	if (!ft_strncmp(operator, ">", 2))
		return (redirect_output(*word, data));
	else if (!ft_strncmp(operator, ">>", 3))
		return (append_output(*word, data));
	else if (!ft_strncmp(operator, "<", 2))
		return (redirect_input(*word, data));
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
		// error_message(NULL, filename, "No such file or directory");
		if (data->in_child)
			exit(ft_free(data, 1));
		return (1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		error_message(NULL, NULL, "Error in dup2");
		close(fd);
		if (data->in_child)
			exit(ft_free(data, 1));
		return (1);
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
		// error_message(NULL, filename, "No such file or directory");
		if (data->in_child)
			exit(ft_free(data, 1));
		return (1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		error_message(NULL, NULL, "Error in dup2");
		close(fd);
		if (data->in_child)
			exit(ft_free(data, 1));
		return (1);
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
		// error_message(NULL, filename, "No such file or directory");
		if (data->in_child)
			exit(ft_free(data, 1));
		return (1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		error_message(NULL, NULL, "Error in dup2");
		close(fd);
		if (data->in_child)
			exit(ft_free(data, 1));
		return (1);
	}
	close(fd);
	return (0);
}
