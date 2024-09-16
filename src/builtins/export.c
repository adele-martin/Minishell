/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 11:43:42 by bschneid          #+#    #+#             */
/*   Updated: 2024/09/16 22:44:01 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

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
		if (!(isalpha(arg[i])) && arg[i] != ' ')
			return (0);
		i++;
	}
	if (arg[i] == '=')
	{
		i++;
		if (arg[i] && (!(isalnum(arg[i])) && arg[i] != ' '))
			return (0);
	}
	return (1);
}

int	builtin_export(char **argv, int argc, char **list_envs, t_list *export_list)
{
	int	i;
	int	invalid;

	invalid = 0;
	if (argc == 1)
		print_list(export_list);
	i = 1;
	while (argv[i])
	{
		if (!is_valid_identifier(argv[i]))
		{
			error_message("export", argv[i], "not a valid identifier");
			invalid = 1;
		}
		else if (with_value(argv[i]))
		{
			update_list(argv[i], list_envs);
			fill_exportlist(argv[i], export_list);
		}
		else
			fill_exportlist(argv[i], export_list);
		i++;
	}
	return (invalid);
}
