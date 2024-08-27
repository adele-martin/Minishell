/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ademarti <ademarti@student.42berlin.de     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 17:23:35 by bschneid          #+#    #+#             */
/*   Updated: 2024/08/27 17:11:39 by ademarti         ###   ########.fr       */
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

// typedef enum s_action {
// 	WORD1,
// 	WORD2
// }	t_action;

// struct for the information
typedef struct s_data
{
	char	*shell_name;
	int		last_status;
	char	*status_str;
	char	*input;
	char 	*tty_name;
	char	**list_envs;
	t_list	*export_list;
	char	**tokens;
	char	**end_tokens;
	char	in_pipe;
	int		signal_fd;
	pid_t	id;
	int		status;
	int		cmd_argc;
	char	**cmd_argv;
	t_list	*linked_args;
	t_ast	*astRoot;
}	t_data;

// global var for received signals
extern volatile __sig_atomic_t	g_signal;

// AST:
t_ast	*create_ast(char **token_start, char **token_end);
// // EVALUATION:
// int		evaluate(char *input, t_info *info);
// REDIRECTIONS:
int	redirect(char *operator, char *word, t_data *data);
int		redirect_output(char *filename);
int		append_output(char *filename);
int		redirect_input(char *filename);
int		heredoc(char *delimiter, char *tty_name);
// EXECUTION:
int		execute(char *input, t_data *data);
char	**create_argv(t_list *linked_args);
// HELPERS:
void	print_args(char *str, t_list *linked_args);
void	clean_quotations(char *str);
void	print_ast(t_ast *root);
// PARSING:
int		parse_ast(t_ast *node, t_data *data);
// UTILS:
char	is_redirection(char *str);
//SIGNALS:
void	handle_signals(void);
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

//ENVIRON_VAR
// void store_envs(char **envp);
char	**envs_list(char **envp);
char	**update_list(char *variable, char **list_envs);
char	*return_value_env(char *variable, char **list);
char	*return_value_var(char *variable, t_list *head);
char	**delete_env(char *variable, char **list);
t_list *delete_var(char *variable, t_list *head);
char	*search_var(char *variable, t_list *head);
char	*search_env(char *variable, char **list);
void bubble_sort(char *arr[], int n);
char	**create_list(char **list);
char	*search(char *variable, char **list, t_list *head);

//BUILT-INS
int builtin_export(char **argv, int argc, char **list_envs, t_list *export_list);
t_list *arrayToLinkedList(char *arr[]);
int	builtin_echo(char **argv, int argc);
int builtin_env(char **argv, int argc, char **list_envs);
int builtin_unset(char **argv, char **list_envs, t_list *head);
int builtin_cd(char **argv, int argc, char **list_envs);
int builtin_pwd(char **argv, char **list_envs);
int builtin_exit(char **argv, int argc);

//HELPER
char *ft_strcat(char* dest, const char* src);

#endif
