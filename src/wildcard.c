

#include "../header/minishell.h"

t_list	*get_files_list()
{
	t_list			*files_list;
	t_list			*file_node;
	char			*file_str;
	DIR				*dir;
	struct dirent	*entry;

	files_list = NULL;
	dir = opendir(".");
	if (!dir)
		return 0;
	entry = readdir(dir);
	while (entry) // Read and print each entry in the directory
	{
		if (*entry->d_name != '.')
		{
			file_str = ft_strdup(entry->d_name);
			if (!file_str)
				return (0);
			file_node = ft_lstnew(file_str);
			if (!file_node)
				return (free(file_str), NULL);
			ft_lstadd_back(&files_list, file_node);
		}
		entry = readdir(dir);
	}
	closedir(dir);	// Close the directory
	return (files_list);
}

// compile with:  cc src/lexing.c -L./src/Libft_extended -lft
int main()
{
	t_list	*files_list;

	files_list = get_files_list();
	printf("The list has %i entries:\n", ft_lstsize(files_list));
	while (files_list)
	{
		printf("path: %s\n", files_list->content);
		files_list = files_list->next;
	}
    return EXIT_SUCCESS;
}

/*
WILDCARD:
in the directory:
header  _infos  Makefile  minishell  obj  README.md  src
" ls * "
expands to
" ls header _infos/ Makefile minishell obj README.md src "

Idea to recreate wildcard: 
1. Use opendir and readdir to open and read the directory specified by the token.
2. Match the wildcard pattern with the entries in the directory.
3. Append matching filenames to the arg list.
*/