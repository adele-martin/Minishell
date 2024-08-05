/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ_var.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ademarti <ademarti@student.42berlin.de     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 16:21:26 by ademarti          #+#    #+#             */
/*   Updated: 2024/08/05 17:17:31 by ademarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "../header/minishell.h"


// void	append_node(t_list	**head, char *variable)
// {
// 	t_list *new_node;
// 	(void)head;
// 	new_node = create_newnode(variable);
// 	printf("%s", new_node->content);


// }

// t_list *create_newnode(char *variable)
// {
// 	t_list *new_node;
// 	new_node = (t_list *)malloc(sizeof(t_list));
// 	if (new_node == NULL)
// 		return (NULL);
// 	new_node->content = ft_strdup(variable);
// 	new_node->next = NULL;

// 	return (new_node);
// }

// void store_env(t_list	**list_env, char **envp)
// {
// 	int	i;

// 	i = 0;
// 	while (envp[i] != NULL)
// 	{
// 		append_node(list_env, envp[i]);
// 		// i++;
// 	}

// 	while
// }

//All built­in functions except printenv are executed by the parent process
//Figure out how to deal with the quotes