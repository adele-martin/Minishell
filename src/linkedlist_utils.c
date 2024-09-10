
#include "../header/minishell.h"

t_list *createNode(const char *data)
{
    t_list *newNode = (t_list *)malloc(sizeof(t_list));
    if (!newNode) {
        exit(EXIT_FAILURE);
    }
    newNode->content = strdup(data);
    free(data);
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

void appendNode(t_list **head, const char *data) {
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
//Util function for the export built-in. It sorts the list in alphabetical order.
//TO DO: also do the difference between 'H' and 'h'
void sortList(t_list *head)
{
    t_list *i;
    t_list *j;
    char *temp;

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
				j->content = ft_strdup(temp);
				free(temp);
			}
			j = j->next;
		}
		i = i->next;
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

//Second export list util
void fill_exportlist(char *argv, t_list **head)
{
	size_t len = strlen("declare -x ") + strlen(argv) + 1;
    char *str = (char *)malloc(len);

    if (!str)
        return;

    ft_strlcpy(str, "declare -x ",len);
    ft_strcat(str, argv);
	appendNode(head, str);
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