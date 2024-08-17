/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   evaluation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 18:11:03 by bschneid          #+#    #+#             */
/*   Updated: 2024/08/16 22:34:55 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

int		run_from_bin_path(char **cmd_argv);
char	**create_argv(t_list *linked_args);


char	replace_vars(t_list *linked_args, t_data *data)
{
	(void)linked_args;
	// alle $-Variablen werden beendet durch 
	// nicht-alphanumerische Zeichen, auch " und '
	// beginnen allerdings nur ohne oder in aeußeren " "
	// zB echo "$PAT"H --> H (da key=PAT nicht existiert)

	ft_printf("%s\n", expanding("PATH", data->list_envs, NULL));
	return (1);
}

// TODO: variable declarations also expand wildcards, variables and clean quotations!

// struct for linked variables list to be replaced:
// typedef struct s_vars
// {
// 	void	*key_start;
// 	int		key_len;
// 	void	*value_start;
// 	int		len_diff;
// 	t_vars	*next;
// }	t_vars;

void	append_replace(char *str, t_vars **list, t_data *data)
{
	while (*list)
		list = &(*list)->next;
	*list = (t_vars *)malloc(sizeof(t_vars));
	if (!*list)
		return ;
	(*list)->next = NULL;
	(*list)->key_start = str++;	// start at $
	if (ft_isdigit(*str) || *str == '?')
		(*list)->key_len = 2;
	if (*str == '0')
		(*list)->value_start = data->shell_name;
	else if (ft_isdigit(*str))
		(*list)->value_start = NULL;
	else if (*str == '?')
		(*list)->value_start = data->status_str;
	else
	{
		str = ft_strdup(str);
		if (!str)
		{
			free(*list);
			*list = NULL;
			return ;
		}
		(*list)->key_len = 0;
		while (ft_isalnum(str[(*list)->key_len]))
			(*list)->key_len++;
		str[(*list)->key_len++] = '\0';
		(*list)->value_start = expanding(str, data->list_envs, NULL);
		free(str);
	}
	if (!(*list)->value_start)
		(*list)->len_diff = 0 - (*list)->key_len;
	else
		(*list)->len_diff = (int)ft_strlen((*list)->value_start) - (*list)->key_len;
}

// $ with alphanum, ? (last status), (!), ($ (ppid) - not possible to get, 0 (name of shell),  
char	has_var(char *str, t_data *data)
{
	char	in_sgl;
	char	in_dbl;
	t_vars	*replace_vars;

	replace_vars = NULL;
	in_sgl = 0;
	in_dbl = 0;
	while (*str)
	{
		if (!in_sgl && !in_dbl && *str == '\'')
			in_sgl = 1;
		else if (!in_sgl && !in_dbl && *str == '"')
			in_dbl = 1;
		else if (in_sgl && *str == '\'')
			in_sgl = 0;
		else if (in_dbl && *str == '"')
			in_dbl = 0;
		else if (!in_sgl && *str == '*' && (ft_isalnum(*(str + 1))
				|| *(str + 1) == '?'))
			append_replace(str++, &replace_vars, data);
		str++;
	}
	return (replace_vars);
}

char	expand_variables(t_list *linked_args, t_data *data)
{
	if (!linked_args)
		return (0);
	while (linked_args)
	{
		if (has_var(linked_args->content, data))
		{
			if (!replace_vars(linked_args, data))
				return (0);
		}
		linked_args = linked_args->next;
	}
	return (1);
}

int	execute(char *input, t_data *data)
{
	char	**cmd_argv;
	t_list	*linked_args;

	if (!input || !*input)
		return (0);
	linked_args = get_args(input);
	if (!add_wildcards(linked_args))
		return (perror("Error in wildcards"), 1);
	// append variables in strings
	if (!expand_variables(linked_args, data))
		return (perror("Error in expanding variables"), 1);
	cmd_argv = create_argv(linked_args);
	if (!cmd_argv)
		return (1);
	if (**cmd_argv == '.')
	{
		if (access(cmd_argv[0], X_OK))
			ft_printf("%s: No execution rights!\n", cmd_argv[0]);
		else
			execve(cmd_argv[0], cmd_argv, NULL);
		exit (2);
	}
	// else if (is_buildin(cmd_argv[0]))
	// 	return (run_buildin(cmd_argv));
	exit (run_from_bin_path(cmd_argv));
}

char	**create_argv(t_list *linked_args)
{
	int		size;
	t_list	*tmp;
	char	**out;
	char	**writer;

	size = ft_lstsize(linked_args);
	out = (char **)malloc((size + 1) * sizeof(char *));
	if (!out)
		return (NULL);
	writer = out;
	while (linked_args)
	{
		tmp = linked_args;
		clean_quotations(linked_args->content);
		*(writer++) = linked_args->content;
		linked_args = linked_args->next;
		free(tmp);
	}
	*writer = NULL;
	return (out);
}

int	run_from_bin_path(char **cmd_argv)
{
	char	*path_str;
	char	**bin_paths;
	char	*filepath;
	char	file_exists;

	path_str = getenv("PATH");
	if (!path_str)
	{
		perror("getenv");
		exit(EXIT_FAILURE);
	}
	bin_paths = ft_split(path_str, ':');
	file_exists = 0;
	while (*bin_paths)
	{
		filepath = ft_strjoin(*bin_paths, "/");
		filepath = ft_strjoin(filepath, cmd_argv[0]);
		if (access(filepath, F_OK) == 0)
			file_exists = 1;
		if (access(filepath, X_OK) == 0)
		{
			execve(filepath, cmd_argv, NULL);
		}
		bin_paths++;
	}
	if (file_exists)
		ft_printf("%s: No execution rights!\n", cmd_argv[0]);
	else
		ft_printf("%s: CMD not found!\n", cmd_argv[0]);
	exit (2); //status
}

