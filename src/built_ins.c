/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ademarti <ademarti@student.42berlin.de     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 12:32:57 by ademarti          #+#    #+#             */
/*   Updated: 2024/08/27 17:35:51 by ademarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//All built­in functions except printenv are executed by the parent process
//Figure out how to deal with the quotes
#include "../header/minishell.h"

/*TODO: return values of the built-ins
0: Success
1: General error
2: Misuse of shell built-ins (e.g., syntax errors)
126: Command invoked cannot execute
127: Command not found
130: Script terminated by Ctrl+C (SIGINT)
We can store in into a global variable or into

*/

// int built_ins()
// {
// 	if ()
// 		return (builtin_echo(argv, argc))
// 	else if ()
// 	else
// 		return (127);
// }

int	builtin_echo(char **argv, int argc)
{
	int	i;
	int option_n;

	option_n = 0;
	if (argc > 1 && ft_strncmp(argv[1], "-n", 2) == 0)
	{
		option_n = 1;
		i = 2;
	}
	else
		i = 1;
	while (argv[i])
	{
		ft_putstr_fd(argv[i], 1);
		if (argv[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (!option_n)
		write(1, "\n", 1);
	return (0);
}

char	**create_list(char **list)
{
	list = malloc(sizeof(char *) * 400);
	if (!list)
		return (NULL);
	return (list);
}

int builtin_unset(char **argv, char **list_envs, t_list *head)
{
	int	i;

	i = 1;
	while (argv[i])
	{
		delete_env(argv[i], list_envs);
		i++;
	}
	i = 0;
	while (argv[i])
	{
		if (search_var(argv[i], head) != NULL)
			delete_var(argv[i], head);
		i++;
	}
	return (0);
}

int builtin_env(char **argv, int argc, char **list_envs)
{
	int	i;
	(void)argv;
	(void)argc;

	i = 0;
	while (list_envs[i])
	{
		ft_printf("%s\n", list_envs[i]);
		i++;
	}
	return (0);
}

int has_equalsign(char *string)
{
	int i = 0;
	while (string[i])
	{
		if (string[i] == '=')
			return (1);
		i++;
	}
	return (0);
}


t_list *createNode(const char *data) {
    t_list *newNode = (t_list *)malloc(sizeof(t_list));
    if (!newNode) {
        perror("Failed to allocate memory for new node");
        exit(EXIT_FAILURE);
    }
    newNode->content = strdup(data);
    if (!newNode->content) {
        perror("Failed to duplicate string");
        free(newNode);
        exit(EXIT_FAILURE);
    }
    newNode->next = NULL;
    return newNode;
}

void appendNode(t_list **head, const char *data) {
    t_list *newNode = createNode(data);
    if (*head == NULL) {
        *head = newNode;
    } else {
        t_list *temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

//also do the difference between 'H' and 'h'
void sortList(t_list *head)
{
    t_list *i;
    t_list *j;
    char temp[100];

    if (head == NULL) {
        return;
    }
    i = head;
    while (i->next != NULL)
	{
		j = i->next;
		while (j != NULL) {
			if (strcmp(i->content, j->content) > 0)
			{
				strcpy(temp, i->content);
				strcpy(i->content, j->content);
				strcpy(j->content, temp);
			}
			j = j->next;
		}
		i = i->next;
	}
}
void printList(t_list *head)
{
    t_list *temp = head;
    while (temp != NULL) {
        printf("%s\n", (char *)temp->content);
        temp = temp->next;
    }
}

void fill_exportlist(char *argv, t_list **head)
{
	char *str = ft_strcat("declare -x ", argv);
	appendNode(head, str);
}

//fill it with the env variables
t_list *arrayToLinkedList(char *arr[])
{
    if (arr[0] == NULL) return NULL;

    t_list *head = createNode(arr[0]);
    t_list *current = head;
    int i = 1;

    while (arr[i] != NULL)
	{
        current->next = createNode(arr[i]);
        current = current->next;
        i++;
    }
    return head;
}

int builtin_export(char **argv, int argc, char **list_envs, t_list *export_list)
{
	int i;

	i = 1;
	if (argc >= 2)
	{
		while (argv[i])
		{
			if (has_equalsign(argv[i]) == 1)
			{
				update_list(argv[i], list_envs);
				fill_exportlist(argv[i], &export_list);
			}
			else if (has_equalsign(argv[i]) == 0)
				fill_exportlist(argv[i], &export_list);
			i++;
		}
	}
	else
	{
	sortList(export_list);
	printList(export_list);
	}
	return (0);
}

int builtin_pwd(char **argv, char **list_envs)
{
	(void)argv;
	update_list(getenv("PWD"), list_envs);
	ft_printf("\n");
	return (0);
}

int builtin_exit(char **argv, int argc)
{
	(void)argv;
	(void)argc;
	ft_printf("exit\n");
	exit(1);
	return (0);
}

char *ft_strcat(char* dest, const char* src)
{
	char* ptr = dest;
	while (*ptr != '\0') {
		ptr++;
	}
	while (*src != '\0') {
		*ptr = *src;
		ptr++;
		src++;
	}
	*ptr = '\0';
	return dest;
}

int builtin_cd (char **argv, int argc, char **list_envs)
{
	char *home;
	char cwd[1024];
	char *current_dir;
	char old_pwd[1024];

	if (argc == 1)
	{
		home = getenv("HOME");
		if (!home)
			ft_printf("bash: cd: HOME not set\n");
		if (chdir(home) == -1)
			ft_printf("error");
	}
	else if (argc == 2)
	{
	if (chdir(argv[1]) == -1)
		ft_printf("cd: %s: No such file or directory\n", argv[1]);
	}
	else if (argc > 2)
	{
		ft_printf("bash: cd: too many arguments\n");
	}
	current_dir = getcwd(cwd, sizeof(cwd));
	ft_strlcpy(old_pwd, "OLDPWD=", sizeof(old_pwd));
	ft_strcat(old_pwd, current_dir);
	update_list(old_pwd, list_envs);
	update_list(getenv("PWD"), list_envs);
	return (0);
}
