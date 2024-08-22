/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ_var.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ademarti <ademarti@student.42berlin.de     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 16:21:26 by ademarti          #+#    #+#             */
/*   Updated: 2024/08/22 15:21:47 by ademarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

char	**envs_list(char **envp)
{
	char	**list_envs;
	int	i;

	list_envs = malloc(sizeof(char *) * 400);
	if (!list_envs)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		list_envs[i] = ft_strdup(envp[i]);
		if (!list_envs[i])
			return NULL;
		i++;
	}
	while (i < 400)
	{
		list_envs[i] = NULL;
		i++;
	}
	return (list_envs);
}

char	**update_list(char *variable, char **list)
{
	int	i;
	int	j;

	j = 0;
	i = 0;
	int found_value;
	found_value = 0;
	while (list[i])
	{
		j = 0;
		while (list[i][j] != '=')
			j++;
		if (ft_strncmp(list[i], variable, j) == 0)
		{
			found_value = 1;
			free(list[i]);
			list[i] = ft_strdup(variable);
			if (!list[i])
				return NULL; //Add a shield function
			break;
		}
		i++;
	}
	if (!found_value)
	{
		list[i] = ft_strdup(variable);
		found_value = 0;
	}
	return (list);
}

char	**delete_env(char *variable, char **list)
{
	int	i;
	int	j;

	j = 0;
	i = 0;
	while (list[i])
	{
		j = 0;
		while (list[i][j] != '=')
			j++;
		if (ft_strncmp(list[i], variable, j) == 0)
		{
			free(list[i]);
			while (list[i + 1])
			{
				list[i] = list[i + 1];
				i++;
			}
			list[i] = NULL;
			break;
		}
		i++;
	}
	return (list);
}
t_list *delete_var(char *variable, t_list *head)
{
	t_list *temp;
	t_list *prev;

	prev = NULL;
	temp = head;
	char *str;
	int i;
	while (temp != NULL)
	{
		str = (char *)temp->content;
		i = 0;
		while (variable[i] &&variable[i] != '=')
			i++;
		if (ft_strncmp(str, variable, i) == 0)
		{
			if (prev == NULL)
				head = temp->next;
			else
				prev->next = temp->next;
			free(temp->content);
			free(temp);
			return head;
		}
		prev = temp;
        temp = temp->next;
	}
	return (head);
}

char	*return_value_env(char *variable, char **list)
{
	int	i;
	int	j;

	j = 0;
	i = 0;
	while (list[i])
	{
		j = 0;
		while (list[i][j] != '=')
			j++;
		if (ft_strncmp(list[i], variable, ft_strlen(variable)) == 0)
		{
			j = j + 1;
			return (&list[i][j]);
		}
		i++;
	}
		return (NULL);
}

char	*return_value_var(char *variable, t_list *head)
{
	t_list *temp;
	temp = head;
	char *str;
	int i;
	while (temp != NULL)
	{
		str = (char *)temp->content;
		i = 0;
		while (str[i] != '=')
			i++;
		if (ft_strncmp(str, variable, ft_strlen(variable)) == 0)
		{
			i = i + 1;
			return(&variable[i]);
		}
		temp = temp->next;
	}
	return (NULL);
}

//TODO: protect malloc in this function
int expand_list(char **argv, t_list *head)
{
	t_list *temp;
	temp = head;
	char *str;
	int len;
	int found;
	while (temp != NULL)
	{
		str = (char *)temp->content;
		len = 0;
		while (argv[1][len] != '=')
			len++;
		if (ft_strncmp(str, argv[1], len) == 0)
		{
			found = 1;
			free(temp->content);
			temp->content = ft_strdup(argv[1]);
		}
		temp = temp->next;
	}
	if (!found)
	{
		t_list *new_node = malloc(sizeof(t_list));
		if (!new_node)
			return 0; //protect malloc
		new_node->content = ft_strdup(argv[1]);
		new_node->next = NULL;
		if (head == NULL) {
			head = new_node;
		} else {
			temp = head;
			while (temp->next != NULL)
				temp = temp->next;
			temp->next = new_node;
		}
	return 1;
	}
	else
		return 0;
}

//Function searches for a key and returns its value in list of variables.
char	*search_var(char *variable, char **list, t_list *head)
{
	char	*out;

	out = return_value_var(variable, list);
	if (out)
		return (out);
	return (NULL);
}
//Function searches for a key and returns its value in the env list.
char	*search_env(char *variable, char **list, t_list *head)
{
	char	*out;

	out = return_value_env(variable, list);
	if (out)
		return (out);
	return (NULL);
}

//TODO : Test
	// t_list *head = NULL;

	// add_node(&head, "USER=john_doe");
	// add_node(&head, "PATH=/usr/bin");
	// add_node(&head, "HOME=/home/john");
	// // print_list(head);
	// // head = delete_var("PATH=/usr/bin", head);
	// char *hey= search("HOME", list_envs, head);
	// ft_printf("%s", hey);
	// // print_list(head);