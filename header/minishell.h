/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ademarti <ademarti@student.42berlin.de     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 17:23:35 by bschneid          #+#    #+#             */
/*   Updated: 2024/08/16 14:48:06 by ademarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>					// printf, perror
# include <stdlib.h>				// malloc, free, exit
# include <unistd.h>				// access, read, write, close, fork, getcwd, chdir, unlink, execve, dup, dup2, pipe, isatty, ttyname, ttyslot
# include <sys/wait.h>				// wait, waitpid, wait3, wait4
# include <signal.h>				// signal, sigaction, sigemptyset, sigaddset, kill
# include <fcntl.h>					// open
# include <sys/types.h>				// stat, lstat, fstat, opendir, readdir, closedir
# include <dirent.h>				// opendir, readdir, closedir
# include <string.h>				// strerror
# include <termios.h>				// tcsetattr, tcgetattr, tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs
# include <curses.h>				// tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs
# include <errno.h>					// errno (used by strerror)
# include <readline/history.h>		// add_history
# include <readline/readline.h>		// readline, rl_clear_history, rl_on_new_line, rl_replace_line, rl_redisplay
# include "../src/Libft_extended/include/libft.h"
# include <dirent.h>				// for directory operations


// typedef struct s_list
// {
// 	void			*content;
// 	struct s_list	*next;
// }	t_list;


typedef struct s_ast t_ast;

// struct for the Abstract syntax tree nodes
struct s_ast {
    char	*value;	// command, operator, filename
    t_ast	*left;	// Left child node
    t_ast	*right;	// Right child node
};

typedef struct {
    char **keys;
    char **values;
} EnvVars;

typedef enum s_action {
	WORD1,
	WORD2
}	t_action;

// struct for the information
typedef struct s_info
{
	unsigned int	id;
}	t_info;

// FUNCTIONS:

// SPLIT_TOKENS:
char	**split_tokens(char *str);
// TOKENIZE:
t_ast	*create_ast(char **token_start, char **token_end);\
void	print_ast(t_ast *root);
// EVALUATION:
int		evaluate(char *input, t_info *info);
// FILE 2:

//ENVIRON_VAR
// void store_envs(char **envp);
char	**envs_list(char **envp);
char	**update_list(char *variable, char **list_envs);
char	*return_value_env(char *variable, char **list);
char	**delete_var(char *variable, char **list);
char	*expanding(char *variable, char **list, t_list *head);

//BUILT-INS
void builtin_export(char **argv, int argc, char **list_envs);
void	builtin_echo(char **argv, int argc);
void builtin_env(char **argv, int argc, char **list_envs);
void builtin_unset(char **argv, char **list_envs);
void builtin_cd(char **argv, int argc);
void builtin_pwd(char **argv, int argc);
void builtin_exit(char **argv, int argc);

#endif
