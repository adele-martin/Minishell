/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ademarti <ademarti@student.42berlin.de     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 17:23:35 by bschneid          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2024/08/15 13:42:22 by ademarti         ###   ########.fr       */
=======
/*   Updated: 2024/08/14 11:48:38 by bschneid         ###   ########.fr       */
>>>>>>> 0f26f591eecd9922b2083a19f20675a9278bc748
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

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

// typedef enum s_action {
// 	WORD1,
// 	WORD2
// }	t_action;

// struct for the information
typedef struct s_info
{
	unsigned int	id;
}	t_info;

// AST:
t_ast	*create_ast(char **token_start, char **token_end);
// EVALUATION:
int		evaluate(char *input, t_info *info);
// REDIRECTIONS:
int		redirect(char *operator, char *word);
int		redirect_output(char *filename);
int		append_output(char *filename);
int		redirect_input(char *filename);
int		heredoc(char *delimiter);
// EXECUTION:
int		execute(char *input);
char	**create_argv(t_list *linked_args);
// HELPERS:
void	print_args(char *str, t_list *linked_args);
void	clean_quotations(char *str);
void	print_ast(t_ast *root);
// PARSING:
int		parse_ast(t_ast *node);
// UTILS:
char	is_redirection(char *str);
// SPLIT_CMD
t_list	*get_args(char *str);
// TOKENIZATION:
char	**split_tokens(char *str);
// WILDCARD:
char	add_wildcards(t_list *linked_args);

//ENVIRON_VAR
// void store_envs(char **envp);
char	**envs_list(char **envp);
char	**update_list(char *variable, char **list_envs);
char	**delete_var(char *variable, char **list);
int expand_list(char **argv, t_list *head);
void	expanding(char **argv, t_list *head);

//BUILT-INS
void builtin_export(char **argv, int argc, char **list_envs);
void	builtin_echo(char **argv, int argc);
void builtin_env(char **argv, int argc, char **list_envs);
void builtin_unset(char **argv, char **list_envs);
void builtin_cd(char **argv, int argc);
void builtin_pwd(char **argv, int argc);
void builtin_exit(char **argv, int argc);

#endif
