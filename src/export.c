#include "../header/minishell.h"

//Util function for the export built-in. It sorts the list in alphabetical order.
//TO DO: also do the difference between 'H' and 'h'
void	sortList(t_list *head)
{
	t_list	*i;
	t_list	*j;
	char	*temp;

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
				temp = ft_strdup(i->content);
				free(i->content);
				i->content = ft_strdup(j->content);
				free(j->content);
                free(i->content);
				j->content = ft_strdup(temp);
				free(temp);
			}
			j = j->next;
		}
		i = i->next;
	}
}

//Second export list util
void	fill_exportlist(char *argv, t_list **head)
{
	size_t	len = strlen("declare -x ") + strlen(argv) + 1;
	char	*str = (char *)malloc(len);

	if (!str)
		return;
	ft_strlcpy(str, "declare -x ",len);
	ft_strcat(str, argv);
	appendNode(head, str);
}


int wrong_syntax(char **arg)
{
	int i;
	int str;

	str = 1;
	int i = 1;
	while (arg[str])
	{
		while (arg[str][i] && arg[str][i] != '=')
		{
			if (!(ft_isalpha(arg[str][i])))
				{
					error_message("export", arg[str], ": not a valid identifier\n");
					return (1);
				}
			i++;
		}
		if (arg[str][i] == '=')
		{
			i++;
			if (arg[str][i] && !(ft_isalnum(arg[str][i])))
				{
					error_message("export", arg[str], ": not a valid identifier\n");
					return (1);
				}
		}
		str++;
	}
	return (0);
}

int	builtin_export(char **argv, int argc, char **list_envs, t_list *export_list)
{
	int	i;

	i = 1;

	if (wrong_syntax(argv))
		return (1);
	if (argc >= 2)
	{
		while (argv[i])
		{
			if (with_value(argv[i]))
			{
				update_list(argv[i], list_envs);
				fill_exportlist(argv[i], &export_list);
			}
			else if (!(with_value(argv[i])))
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