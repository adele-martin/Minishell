
#include "../header/minishell.h"

t_list *createNode(const char *data)
{
    t_list *newNode = (t_list *)malloc(sizeof(t_list));
    if (!newNode) {
        exit(EXIT_FAILURE);
    }
    newNode->content = strdup(data);
    if (!newNode->content)
    {
        free(newNode);
        exit(EXIT_FAILURE);
    }
    newNode->next = NULL;
    return (newNode);
}

t_list *createNodeexport(const char *str)
{
    t_list *node = (t_list *)malloc(sizeof(t_list));
    if (!node)
		return NULL;
    node->content = (char *)malloc(strlen("declare -x ") + strlen(str) + 1);
    if (!node->content)
    {
        free(node);
        return NULL;
    }
    strcpy(node->content, "declare -x ");
    strcat(node->content, str);
    node->next = NULL;
    return node;
}

char	**create_list(char **list)
{
	list = malloc(sizeof(char *) * 400);
	if (!list)
		return (NULL);
	return (list);
}

void appendNode(t_list **head, const char *data) 
{
    t_list *newNode = createNode(data);
    if (*head == NULL)
	{
        *head = newNode;
    } else {
        t_list *temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

//Second export list util
void printList(t_list *head)
{
    t_list *temp;

    temp = head;
    while (temp != NULL) {
    ft_printf("%s\n", (char *)temp->content);
        temp = temp->next;
    }
}

//Function to create the second "export" list
t_list *arrayToLinkedList(char *arr[])
{
	if (arr[0] == NULL) return NULL;

	t_list *head = createNodeexport(arr[0]);
	t_list *current = head;
	int i = 1;

	while (arr[i] != NULL)
	{
		current->next = createNodeexport(arr[i]);
		current = current->next;
		i++;
	}
    return head;
}
