/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 17:23:35 by bschneid          #+#    #+#             */
/*   Updated: 2024/09/13 20:16:26 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define _GNU_SOURCE

# include "../src/Libft_extended/include/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <signal.h>
# include <fcntl.h>
# include <sys/types.h>
# include <dirent.h>
# include <string.h>
# include <termios.h>
# include <curses.h>
# include <errno.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <dirent.h>
# include <sys/stat.h>
# include <sys/ioctl.h>

typedef struct s_ast	t_ast;

// struct for the Abstract syntax tree nodes
struct s_ast
{
	char	*value;
	t_ast	*left;
	t_ast	*right;
};

// typedef struct s_env_vars
// {
// 	char **keys;
// 	char **values;
// }	t_env_vars;

typedef struct s_vars	t_vars;

// struct for linked variables list to be replaced:
typedef struct s_vars
{
	char	*key_start;
	int		key_len;
	char	*value_start;
	int		len_diff;
	t_vars	*next;
}	t_vars;

// struct for the information
typedef struct s_data
{
	char	*shell_name;
	char	*input;
	char	**tokens;
	t_ast	*ast_root;
	int		stdin;
	int		stdout;
	int		status;
	char	*status_str;
	char	*tty_name;
	char	**list_envs;
	t_list	*export_list;
	char	in_child;
	char	in_pipe;
	int		signal_fd;
	pid_t	id;
	int		cmd_argc;
	char	**cmd_argv;
	t_list	*linked_args;
}	t_data;

// global var for received signals
// extern int	g_signal;
extern volatile __sig_atomic_t	g_signal;

// AST:
t_ast	*create_ast(char **token_start, char **token_end);
// REDIRECTIONS:
int		redirect(char *operator, char *word, t_data *data);
int		redirect_output(char *filename, t_data *data);
int		append_output(char *filename, t_data *data);
int		redirect_input(char *filename, t_data *data);
int		heredoc(char *delimiter, t_data *data);

// EVALUATION:
int		execute(char *input, t_data *data);
char	**create_argv(t_list *linked_args);
void	update_home(t_data *data, char **argv);
// EVALUATION_HELPERS:
char	run_builtin(t_data *data);
char	**create_argv(t_list *linked_args);
int		get_argc(char **argv);
void	clean_args(t_list **args);
// HELPERS:
void	print_args(char *str, t_list *linked_args);
void	clean_quotations(char *str);
void	print_ast(t_ast *root);
// INIT:
int		initialize_data(t_data *data, int argc, char **envp);
int		restore_stdin_stdout(t_data *data, char option);
// PARSING:
int		parse_ast(t_ast *node, t_data *data);
// UTILS:
char	is_redirection(char *str);
//SIGNALS:
void	handle_signals(char option);
void	signal_action(int sig);
// SPLIT_CMD
t_list	*get_args(char *str);
// TOKENIZATION:
char	**split_tokens(char *str);
// VAR_EXPAND:
char	expand_variables(t_list *linked_args, t_data *data);
// VAR_HELPERS:
char	replace_vars(t_list *arg, t_vars *vars);
// WILDCARD:
char	add_wildcards(t_list *linked_args);

//VARIABLES
// void store_envs(char **envp);
char	**envs_list(char **envp);
char	**update_list(char *variable, char **list_envs);
char	*return_value_env(char *variable, char **list);
char	*return_value_var(char *variable, t_list *head);
char	**delete_env(char *variable, char **list);
t_list	*delete_var(char *variable, t_list *head);
char	*search_var(char *variable, t_list *head);
char	*search_env(char *variable, char **list);
void	bubble_sort(char *arr[], int n);
char	**create_list(char **list);
char	*search(char *variable, char **list, t_list *head);

char	**envs_list(char **envp);
int		expand_list(char **argv, t_list *head);

//LINKED LIST UTILS
void	fill_exportlist(char *arg, t_list *export_list);
void	print_list(t_list *head);
void	sort_list(t_list *head);
void	append_node(t_list **head, const char *data);

//BUILT-INS
int		builtin_export(char **argv, int argc, 
			char **list_envs, t_list *export_list);
t_list	*array_to_linkedlist(char *arr[]);
int		builtin_echo(char **argv, int argc);
int		builtin_env(char **list_envs);
int		builtin_unset(char **argv, char **list_envs, t_list *head);
int		builtin_cd(t_data *data);
int		builtin_pwd(void);
int		builtin_exit(char **argv, int argc, t_data *data);

//MEMORY
int		exit_error(t_data *data);

//ERROR HANDLING
void	free_array(char ***array);
void	error_message(char *cmd, char *arg, char *message);

//UTILS
char	*ft_strcat(char *dst, const char *src);
int		with_value(char *string);
int		ft_free(t_data *data, int exit);
int		handle_plus_or_minus(char c);
int		isalnum_space(int c);
int		isalpha_space(int c);

#endif
