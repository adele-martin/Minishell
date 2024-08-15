/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 17:23:35 by bschneid          #+#    #+#             */
/*   Updated: 2024/08/14 11:48:38 by bschneid         ###   ########.fr       */
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

#endif
