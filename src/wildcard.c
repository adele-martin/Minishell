/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 15:16:24 by bschneid          #+#    #+#             */
/*   Updated: 2024/08/16 22:00:31 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

// * (asterisk) this can represent any number of characters (including zero, 
	// in other words, zero or more characters). If you specified a "cd*" it 
	// would use "cda", "cdrom", "cdrecord" and anything that starts with “cd”
	// also including “cd” itself. "m*l" could by mill, mull, ml, and anything 
	// that starts with an m and ends with an l.

static t_list	*get_files_list(void);
static char		has_wildcards(char *str);
static char		replace_wildcards(t_list ***writer, t_list *files_list);
static char		hits_wc(char *wc, char *str, char in_sgl, char in_dbl);

// appends the arguments-linked-list through wildcard-hits
char	add_wildcards(t_list *linked_args)
{
	t_list	*files_list;
	t_list	*tmp;
	t_list	**writer;

	files_list = get_files_list();
	if (!files_list || !linked_args)
		return (0);
	writer = &(linked_args->next);
	while (*writer)
	{
		if (has_wildcards((*writer)->content))
		{
			tmp = *writer;
			if (replace_wildcards(&writer, files_list))
			{
				*writer = tmp->next;
				free(tmp->content);
				free(tmp);
				continue ;
			}
		}
		writer = &((*writer)->next);
	}
	return (1);
}

// TODO:  should open the current dir in $PWD
// returns a linked list with the file-names in the current directory
static t_list	*get_files_list(void)
{
	t_list			*files_list;
	t_list			*file_node;
	char			*file_str;
	DIR				*dir;
	struct dirent	*entry;

	files_list = NULL;
	dir = opendir(".");
	if (!dir)
		return (ft_printf("ERROR WILDCARD"), NULL);
	entry = readdir(dir);
	while (entry)
	{
		if (*entry->d_name != '.')
		{
			file_str = ft_strdup(entry->d_name);
			if (!file_str)
				return (0);
			file_node = ft_lstnew(file_str);
			if (!file_node)
				return (free(file_str), NULL);
			ft_lstadd_back(&files_list, file_node);
		}
		entry = readdir(dir);
	}
	closedir(dir);
	return (files_list);
}

// checks, if a given string (str) includes a wildcard (*)
static char	has_wildcards(char *str)
{
	char	in_single;
	char	in_double;

	in_single = 0;
	in_double = 0;
	while (*str)
	{
		if (!in_single && !in_double && *str == '\'')
			in_single = 1;
		else if (!in_single && !in_double && *str == '"')
			in_double = 1;
		else if (in_single && *str == '\'')
			in_single = 0;
		else if (in_double && *str == '"')
			in_double = 0;
		else if (!in_single && !in_double && *str == '*')
			return (1);
		str++;
	}
	return (0);
}

// replaces/adds the hit files-names to the linked list (writer)
static char	replace_wildcards(t_list ***writer, t_list *files_list)
{
	char	replaced;
	char	*wc;
	t_list	*new_node;

	replaced = 0;
	wc = (**writer)->content;
	while (files_list)
	{
		if (hits_wc(wc, files_list->content, 0, 0))
		{
			new_node = ft_lstnew(files_list->content);
			if (!new_node)
				return (0);
			replaced = 1;
			**writer = new_node;
			*writer = &(new_node->next);
		}
		files_list = files_list->next;
	}
	if (replaced)
		return (1);
	return (0);
}

// checks, if a wildcard-string (wc) expands to a given string (str)
static char	hits_wc(char *wc, char *str, char in_sgl, char in_dbl)
{
	if (!*wc && !*str)
		return (1);
	else if (!*wc)
		return (0);
	if (!in_sgl && !in_dbl && *wc == '\'')
		return (hits_wc(wc + 1, str, 1, 0));
	else if (!in_sgl && !in_dbl && *wc == '"')
		return (hits_wc(wc + 1, str, 0, 1));
	else if ((in_sgl && *wc == '\'') || (in_dbl && *wc == '"'))
		return (hits_wc(wc + 1, str, 0, 0));
	if (!in_sgl && !in_dbl && *wc == '*')
	{
		if (!*str || *(wc + 1) == '*')
			return (hits_wc(wc + 1, str, 0, 0));
		else if (!*(wc + 1))
			return (1);
		else
			return (hits_wc(wc + 1, str, 0, 0) || hits_wc(wc, str + 1, 0, 0));
	}
	if (*wc == *str)
		return (hits_wc(wc + 1, str + 1, in_sgl, in_dbl));
	return (0);
}
