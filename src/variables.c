#include "../header/minishell.h"

//This function apprends new variables to our env list.
char	**update_list(char *variable, char **list)
{
	int	i;
	int	j;
	int	found_value;

	j = 0;
	i = 0;
	found_value = 0;
	while (list[i])
	{
		j = 0;
		while (list[i][j] != '=')
			j++;
		if (ft_strncmp(list[i], variable, j) == 0)
		{
			found_value = 1;
			free(list[i]);
			list[i] = ft_strdup(variable);
			if (!list[i])
				return (NULL);
			break ;
		}
		i++;
	}
	if (!found_value)
	{
		list[i] = ft_strdup(variable);
		found_value = 0;
	}
	return (list);
}

//This function deletes variables from our variables list (expanding).
t_list	*delete_var(char *variable, t_list *head)
{
	t_list	*temp;
	t_list	*prev;
	char	*str;
	int		i;

	prev = NULL;
	temp = head;
	while (temp != NULL)
	{
		str = (char *)temp->content;
		i = 0;
		while (variable[i] && variable[i] != '=')
			i++;
		if (!(ft_strncmp(str, variable, i)))
		{
			if (prev == NULL)
				head = temp->next;
			else
				prev->next = temp->next;
			free(temp->content);
			free(temp);
			return (head);
		}
		prev = temp;
		temp = temp->next;
	}
	return (head);
}

//Search function util
char	*return_value_var(char *variable, t_list *head)
{
	t_list	*temp;
	char	*str;
	int		i;

	temp = head;
	while (temp != NULL)
	{
		str = (char *)temp->content;
		i = 0;
		while (str[i] != '=')
			i++;
		if (!ft_strncmp(str, variable, i) && !variable[i])
		{
			i = i + 1;
			return (&variable[i]);
		}
		temp = temp->next;
	}
	return (NULL);
}

//Function searches for a key and returns its value in list of variables.
char	*search_var(char *variable, t_list *head)
{
	char	*out;

	out = return_value_var(variable, head);
	if (out)
		return (out);
	return (NULL);
}

//Function created for testing purposes.
char	*search(char *variable, char **list, t_list *head)
{
	char	*out;

	out = return_value_env(variable, list);
	if (out)
		return (out);
	out = return_value_var(variable, head);
	if (out)
		return (out);
	return (NULL);
}
