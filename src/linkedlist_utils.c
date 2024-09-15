/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linkedlist_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 18:44:00 by bschneid          #+#    #+#             */
/*   Updated: 2024/09/13 18:44:01 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

t_list	*create_node(const char *data)
{
	t_list	*new_node;

	new_node = (t_list *)malloc(sizeof(t_list));
	if (!new_node)
	{
		exit(EXIT_FAILURE);
	}
	new_node->content = strdup(data);
	if (!new_node->content)
	{
		free(new_node);
		exit(EXIT_FAILURE);
	}
	new_node->next = NULL;
	return (new_node);
}

t_list	*create_nodeexport(const char *str)
{
	t_list	*node;

	node = (t_list *)malloc(sizeof(t_list));
	if (!node)
		return (NULL);
	node->content = (char *)malloc(strlen("declare -x ") + strlen(str) + 1);
	if (!node->content)
	{
		free(node);
		return (NULL);
	}
	strcpy(node->content, "declare -x ");
	strcat(node->content, str);
	node->next = NULL;
	return (node);
}

// TODO: Maybe do it larger!?!
char	**create_list(char **list)
{
	list = malloc(sizeof(char *) * 400);
	if (!list)
		return (NULL);
	return (list);
}

void	append_node(t_list **head, const char *data)
{
	t_list	*new_node;
	t_list	*temp;

	new_node = create_node(data);
	if (*head == NULL)
	{
		*head = new_node;
	}
	else
	{
		temp = *head;
		while (temp->next != NULL)
		{
			temp = temp->next;
		}
		temp->next = new_node;
	}
}

//Function to create the second "export" list
t_list	*array_to_linkedlist(char *arr[])
{
	t_list	*head;
	t_list	*current;
	int		i;

	if (arr[0] == NULL)
		return (NULL);
	head = create_nodeexport(arr[0]);
	current = head;
	i = 1;
	while (arr[i] != NULL)
	{
		current->next = create_nodeexport(arr[i]);
		current = current->next;
		i++;
	}
	return (head);
}
