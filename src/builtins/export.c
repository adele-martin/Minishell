/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 11:43:42 by bschneid          #+#    #+#             */
/*   Updated: 2024/09/18 20:09:16 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

t_list	*update_exportlist(char *variable, t_list *head)
{
	t_list	*temp;
	// t_list	*prev;
	char	*str;
	int		i;

	// prev = NULL;
	temp = head;
	while (temp != NULL)
	{
		str = (char *)temp->content;
		i = 0;
		while (variable[i] && variable[i] != '=')
			i++;
		if (!ft_strncmp(str, variable, i) && (!str[i] || str[i] == '='))
		{
			// prev->next = temp->next;
			ft_printf("Test1");
			// free(str);
			ft_printf("Test2");
			temp->content = variable;
			// append_node(&head, variable);
			return (head);
		}
		// prev = temp;
		temp = temp->next;
	}
	append_node(&head, variable);
	return (head);
}

// Gets a new string (arg) and adds it to the export list
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
	// free(str);
}

int	is_valid_identifier(const char *arg)
{
	int	i;

	if (!ft_isalpha(arg[0]) && arg[0] != '_')
		return (0);
	i = 1;
	while (arg[i] && arg[i] != '=')
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
			return (0);
		i++;
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
