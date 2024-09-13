/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ademarti <ademarti@student.42berlin.de     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 11:43:42 by bschneid          #+#    #+#             */
/*   Updated: 2024/09/13 15:50:23 by ademarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

//Util function for the export built-in. It sorts the list in alphabetical order.
// TO DO: also do the difference between 'H' and 'h'
void	sort_list(t_list *head)
{
	t_list	*i;
	t_list	*j;
	char	*temp;

	if (head == NULL)
	{
		return ;
	}
	i = head;
	while (i->next != NULL)
	{
		j = i->next;
		while (j != NULL)
		{
			if (strcmp(i->content, j->content) > 0)
			{
				temp = i->content;
				i->content = j->content;
				j->content = temp;
			}
			j = j->next;
		}
		i = i->next;
	}
}

t_list	*update_exportlist(char *variable, t_list *head)
{
	t_list	*temp;
	t_list	*prev;
	char	*str;
	int		i;

	prev = NULL;
	temp = head;
	while (temp != NULL)
	{
		str = (char *)temp->content;
		i = 0;
		while (variable[i] && variable[i] != '=')
			i++;
		if (!(ft_strncmp(str, variable, i)))
		{
			if (prev == NULL)
				head = temp->next;
			else
				prev->next = temp->next;
			free(temp->content);
			append_node(&head, variable);
			return (head);
		}
		prev = temp;
		temp = temp->next;
	}
	append_node(&head, variable);
	return (head);
}

// Second export list util
void	fill_exportlist(char *arg, t_list *export_list)
{
	size_t	len;
	char	*str;

	len = strlen("declare -x ") + strlen(arg) + 1;
	str = (char *)malloc(len);
	if (!str)
		return ;
	ft_strlcpy(str, "declare -x ", len);
	ft_strcat(str, arg);
	update_exportlist(str, export_list);
}


int	is_valid_identifier(const char *arg)
{
	int	i;

	i = 0;
	if (arg[0] == '=')
		return (0);
	while (arg[i] && arg[i] != '=')
	{
		if (!isalpha_space(arg[i]))
			return (0);
		i++;
	}
	if (arg[i] == '=')
	{
		i++;
		if (arg[i] && !isalnum_space(arg[i]))
			return (0);
	}
	return (1);
}

int	is_wrong_syntax(char **arg)
{
	int	arg_idx;

	arg_idx = 1;
	while (arg[arg_idx])
	{
		if (!is_valid_identifier(arg[arg_idx]))
		{
			error_message("export", arg[arg_idx], "not a valid identifier");
			return (1);
		}
		arg_idx++;
	}
	return (0);
}

int	builtin_export(char **argv, int argc, char **list_envs, t_list *export_list)
{
	int	i;

	i = 1;
	if (is_wrong_syntax(argv))
		return (1);
	if (argc >= 2)
	{
		while (argv[i])
		{
			if (with_value(argv[i]))
			{
				update_list(argv[i], list_envs);
				fill_exportlist(argv[i], export_list);
			}
			else if (!(with_value(argv[i])))
				fill_exportlist(argv[i], export_list);
			i++;
		}
	}
	else
	{
		sort_list(export_list);
		print_list(export_list);
	}
	return (0);
}
