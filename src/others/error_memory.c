/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_memory.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ademarti <ademarti@student.42berlin.de     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 09:50:50 by bschneid          #+#    #+#             */
/*   Updated: 2024/09/16 10:02:13 by ademarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

//Function to free array of strings
void	free_array(char ***array)
{
	size_t	i;

	if (!*array)
		return ;
	i = 0;
	while (array[0][i])
		free(array[0][i++]);
	free(*array);
	*array = NULL;
}

// Function to print error messages
void	error_message(char *cmd, char *arg, char *message)
{
	ft_putstr_fd("minishell: ", 2);
	if (cmd)
	{
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": ", 2);
	}
	if (arg)
	{
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(": ", 2);
	}
	ft_putstr_fd(message, 2);
	ft_putstr_fd("\n", 2);
}

// struct for the Abstract syntax tree nodes
struct s_ast
{
	char	*value;
	t_ast	*left;
	t_ast	*right;
};


// This function recursively frees the left and right nodes
void	free_ast(t_ast *node)
{
	if (node == NULL)
		return;
	if (node->left != NULL)
		free_ast(node->left);
	if (node->right != NULL)
		free_ast(node->right);
	free(node);
}

// TODO: Implement function to free the AST and linked_args
int	ft_free(t_data *data, int exit)
{
	free(data->shell_name);
	if (data->list_envs)
		free_array(&data->list_envs);
	if (data->export_list)
		ft_lstclear(&data->export_list, free);
	if (data->status_str)
		free(data->status_str);
	if (data->input)
		free(data->input);
	if (data->tokens)
		free_array(&data->tokens);
	close(data->stdin);
	close(data->stdout);
	rl_clear_history();
	return (exit);
}
