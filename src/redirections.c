/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 16:52:14 by bschneid          #+#    #+#             */
/*   Updated: 2024/08/05 11:52:39 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

// struct s_ast {
//     char	*value;	// command, operator, filename
//     t_ast	*left;	// Left child node
//     t_ast	*right;	// Right child node
// };

/*
Usage of many > , for example in "echo start > test.txt > Test.txt":
>
    echo start
    >
        test.txt
        Test.txt
outer pipe:
	1. The call "echo start" would write to STOUT
	2. We have to redirect the output inside a temporary file
	3. We have to put the content of the temporary file to both test.txt and Test.txt

It can even get more complex, for example in "INPUT: echo start > test.txt > Test.txt > TTest.txt":
>
    echo start
    >
        test.txt
        >
            Test.txt
            TTest.txt

*/
/*
for every cmd there is a child process and basically a fd-in and fd-out

if there is a pipe, we redirect the output of the left child to the input of the right child

if there is a redirection, we redirect the output of the left child to the file specified in the redirection
if there are many redirections, it only gets to the last one!
*/

// not really sure what this function does and not right!
redirect(t_ast *node)
{
	if (!node)
		return ;
	if (node->left && !ft_strncmp(node->left->value, ">", 2))
		redirect_output(node->left->right->value);
	else if (node->left && !ft_strncmp(node->left->value, ">>", 3))
		redirect_output_append(node->left->right->value);
	else if (node->left && !ft_strncmp(node->left->value, "<", 2))
		redirect_input(node->left->right->value);
	else if (node->left && !ft_strncmp(node->left->value, "<<", 3))
		redirect_input_heredoc(node->left->right->value);
	else
		redirect(node->left);
	redirect(node->right);
}
