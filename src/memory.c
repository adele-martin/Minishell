#include "../header/minishell.h"

//Function to free array of strings
void freeArray(char **array) {
    if (array == NULL) {
        return;
    }
    int i = 0;
    while (array[i] != NULL)
    {
        free(array[i]);
        i++;
    }

    free(array);
}

//Function to free linked list.
void freeList(t_list *head)
{
    t_list *temp;

    while (head != NULL)
    {
        temp = head;
        head = head->next;

        free(temp->content);
        free(temp);
    }
}

int ft_free(t_data *data, int exit)
{
	if (data->list_envs)
		freeArray(data->list_envs);
	if (data->export_list)
		freeList(data->export_list);
    if (data->shell_name)
        free(data->shell_name);
    if (data->status_str)
        free(data->status_str);
    if (data->input)
        free(data->input);
    rl_clear_history();
    return (exit);
}