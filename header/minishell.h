/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 17:23:35 by bschneid          #+#    #+#             */
/*   Updated: 2024/09/11 11:55:50 by bschneid         ###   ########.fr       */
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

typedef struct s_ast	t_ast;

// struct for the Abstract syntax tree nodes
struct s_ast
{
	char	*value;	// command, operator, filename
	t_ast	*left;	// Left child node
	t_ast	*right;	// Right child node
};

typedef struct
{
    char **keys;
    char **values;
}	EnvVars;

// struct for linked lists from Libft:
// typedef struct s_list
// {
// 	void			*content;
// 	struct s_list	*next;
// }	t_list;

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
	t_ast	*astRoot;
	// int		last_status;
	int		status;
	char	*status_str;
	char 	*tty_name;
	char	**list_envs;
	t_list	*export_list;
	// char	**end_tokens;
	char	exit;
	char	in_child;
	char	in_pipe;
	int		signal_fd;
	pid_t	id;
	int		cmd_argc;
	char	**cmd_argv;
	t_list	*linked_args;
}	t_data;

// global var for received signals
// extern volatile __sig_atomic_t	g_signal;
extern int	g_signal;
// extern volatile __sig_atomic_t	g_signal;

// AST:
t_ast	*create_ast(char **token_start, char **token_end);
// // EVALUATION:
// int		evaluate(char *input, t_info *info);
// REDIRECTIONS:
int		redirect(char *operator, char *word, t_data *data);
int		redirect_output(char *filename, t_data *data);
int		append_output(char *filename, t_data *data);
int		redirect_input(char *filename, t_data *data);
int		heredoc(char *delimiter, t_data *data);

// EXECUTION:
int		execute(char *input, t_data *data);
char	**create_argv(t_list *linked_args);
void	update_home(t_data *data, char **argv);
// HELPERS:
void	print_args(char *str, t_list *linked_args);
void	clean_quotations(char *str);
void	print_ast(t_ast *root);
// INIT:
int		initialize_data(t_data *data, int argc, char **argv, char **envp);
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
int expand_list(char **argv, t_list *head);

//LINKED LIST UTILS
void fill_exportlist(char *argv, t_list **head);
void printList(t_list *head);
void sortList(t_list *head);

//BUILT-INS
int		builtin_export(char **argv, int argc, char **list_envs, t_list *export_list);
t_list	*arrayToLinkedList(char *arr[]);
int		builtin_echo(char **argv, int argc);	// DONE
int		builtin_env(char **list_envs);			// DONE
int		builtin_unset(char **argv, char **list_envs, t_list *head);
int		builtin_cd (t_data *data);
int		builtin_pwd(void);						// DONE
int builtin_exit(char **argv, int argc, t_data *data);

//MEMORY
void freeList(t_list *head);
void freeArray(char **array);
int exit_error(t_data *data);

//UTILS
char	*ft_strcat(char *dst, const char *src);
int has_equalsign(char *string);
int ft_free(t_data *data, int exit);
int handle_plus_or_minus(char c);

#endif
