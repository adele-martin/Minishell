/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_memory.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 09:50:50 by bschneid          #+#    #+#             */
/*   Updated: 2024/09/17 16:57:08 by bschneid         ###   ########.fr       */
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

// This function recursively frees the left and right nodes
void	free_ast(t_ast *node)
{
	if (node == NULL)
		return ;
	if (node->left != NULL)
		free_ast(node->left);
	if (node->right != NULL)
		free_ast(node->right);
	free(node);
	node = NULL;
}

// TODO: Implement function to free the AST and linked_args
int	ft_free(t_data *data, int exit)
{
	if (data->input)
		free(data->input);
	if (data->tokens)
		ft_split_free(data->tokens);
	if (data->bin_paths)
		ft_split_free(data->bin_paths);
	if (data->linked_args)
		ft_lstclear(&data->linked_args, free);
	if (data->files_list)
		ft_lstclear(&data->files_list, free);
	if (data->argv)
		free_array(&data->argv);
	free_ast(data->ast_root);
	if (data->export_list)
		ft_lstclear(&data->export_list, free);
	if (data->status_str)
		free(data->status_str);
	if (data->list_envs)
		free_array(&data->list_envs);
	free_waitlist(&data->child_pids);
	free(data->shell_name);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	rl_clear_history();
	return (exit);
}
