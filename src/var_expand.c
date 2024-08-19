
// TODO: variable declarations also expand wildcards, variables and clean quotations!

#include "../header/minishell.h"

static t_vars	*var_val(char *str, t_data *data);
static void		append_replace(char *str, t_vars **list, t_data *data);
static char		get_alphaval(char *str, t_vars **list, t_data *data);
static void		free_vars(t_vars *vars);

char	expand_variables(t_list *linked_args, t_data *data)
{
	t_vars	*vars;

	if (!linked_args)
		return (0);
	while (linked_args)
	{
		vars = var_val(linked_args->content, data);
		if (vars && !replace_vars(linked_args, vars))
			return (free_vars(vars), 0);
		free_vars(vars);
		linked_args = linked_args->next;
	}
	return (1);
}

// $ with alphanum, ? (last status), ($ (ppid) - not possible, 0 (shellname)
static t_vars	*var_val(char *str, t_data *data)
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
		else if (!in_sgl && *str == '$' && (ft_isalnum(*(str + 1))
				|| *(str + 1) == '?'))
			append_replace(str++, &replace_vars, data);
		str++;
	}
	return (replace_vars);
}

// fills the linked vars list with infos about VARS to replace
// key_start holds pointer to $ from VAR
// key_len has the len of VAR-string including the $
// value_start holds pointer to the replace-string or NULL (!)
// len_diff has the len-difference after replacing (can be negative)
static void	append_replace(char *str, t_vars **list, t_data *data)
{
	while (*list)
		list = &(*list)->next;
	*list = (t_vars *)malloc(sizeof(t_vars));
	if (!*list)
		return ;
	(*list)->next = NULL;
	(*list)->key_start = str++;
	if (ft_isdigit(*str) || *str == '?')
		(*list)->key_len = 2;
	if (*str == '0')
		(*list)->value_start = data->shell_name;
	else if (ft_isdigit(*str))
		(*list)->value_start = NULL;
	else if (*str == '?')
		(*list)->value_start = data->status_str;
	else if (!get_alphaval(ft_strdup(str), list, data))
		return ;
	if (!(*list)->value_start)
		(*list)->len_diff = 0 - (*list)->key_len;
	else
		(*list)->len_diff = (int)ft_strlen((*list)->value_start) - (*list)->key_len;
}

static char	get_alphaval(char *str, t_vars **list, t_data *data)
{
	if (!str)
	{
		free(*list);
		*list = NULL;
		return (0);
	}
	(*list)->key_len = 0;
	while (ft_isalnum(str[(*list)->key_len]))
		(*list)->key_len++;
	str[(*list)->key_len++] = '\0';
	(*list)->value_start = search(str, data->list_envs, NULL);
	free(str);
	return (1);
}

static void	free_vars(t_vars *vars)
{
	t_vars	*tmp;

	while (vars)
	{
		tmp = vars;
		vars = vars->next;
		free(tmp);
	}
}
