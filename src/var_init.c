
#include "../header/minishell.h"

//This function creates the initial env array of strings from the main.
char	**envs_list(char **envp)
{
	char	**list_envs;
	int	i;

	list_envs = malloc(sizeof(char *) * 400);
	if (!list_envs)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		list_envs[i] = ft_strdup(envp[i]);
		if (!list_envs[i])
			return NULL;
		i++;
	}
	while (i < 400)
	{
		list_envs[i] = NULL;
		i++;
	}
	return (list_envs);
}

//TODO: protect malloc in this function
int expand_list(char **argv, t_list *head)
{
	t_list *temp;
	temp = head;
	char *str;
	int len;
	int found;
	while (temp != NULL)
	{
		str = (char *)temp->content;
		len = 0;
		while (argv[1][len] != '=')
			len++;
		if (ft_strncmp(str, argv[1], len) == 0)
		{
			found = 1;
			free(temp->content);
			temp->content = ft_strdup(argv[1]);
			if (!temp->content)
				return 0;
		}
		temp = temp->next;
	}
	if (!found)
	{
		t_list *new_node = malloc(sizeof(t_list));
		if (!new_node)
			return 0;
		new_node->content = ft_strdup(argv[1]);
		if (!new_node->content)
			return 0;
		new_node->next = NULL;
		if (head == NULL) {
			head = new_node;
		} else {
			temp = head;
			while (temp->next != NULL)
				temp = temp->next;
			temp->next = new_node;
		}
		return 1;
	}
	else
		return 0;
}