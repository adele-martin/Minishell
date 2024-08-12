/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ_var.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ademarti <ademarti@student.42berlin.de     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 16:21:26 by ademarti          #+#    #+#             */
/*   Updated: 2024/08/12 19:11:52 by ademarti         ###   ########.fr       */
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
	int found;

	j = 0;
	i = 0;
	found = 0;
	while (list[i])
	{
		j = 0;
		while (list[i][j] != '=')
			j++;
		if (ft_strncmp(list[i], variable, j) == 0)
		{
			found = 1;
			free(list[i]);
			list[i] = ft_strdup(variable);
			if (!list[i])
				return NULL; //Add a shield function
			break;
		}
		i++;
	}
	if (!found)
		list[i] = ft_strdup(variable);
	return (list);
}

//TODO: protect malloc in this function
void expand(char **argv, int argc, t_list *head)
{
	(void)argc;
	t_list *temp;
	temp = head;
	char *str;
	int len;
	int found;
	found = 0;
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
			return;
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
	}
}

char	**delete_var(char *variable, char **list)
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


