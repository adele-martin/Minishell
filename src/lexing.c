

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
Important for lexing:
	- parenthesis for priority				()
	- single quotes (no interpretation)		''
	- double quotes (only $-interpretation)	""
	- dollar sign (environment variables)	$
	- exit status of foreground pipeline	$?
	- output to next input -> pipes 		|
	- redirect input 						<
	- redirect output 						>
	- read input until delimiter 			<<
	- redirect output in append mode 		>>
	- AND-execution							&&
	- OR-execution							||
	- wildcards in curr. working directory	*

Not interpret unclosed quotes or special characters which are not required by the
subject such as \ (backslash) or ; (semicolon).
*/

/*
GET ENVIRONMENTS: env / printenv
GET VALUE OF VAR: " printenv varname "
EXPORT GLOBAL VAR: " export varname=value "
EXPORT LOCAL VAR: " varname =value " (or " set varname =value "
CREATE NEW ENV VAR: " export varname=value "
REMOVE ENV VAR: " unset varname "
*/

/*
Fehler in der Ausgabe umleiten:
Find everything in root (/) ending in .log, where errors (2) sent to /dev/null
find / -name *.log 2> /dev/null 
*/

/*
What is the differnce between ` and ' ???

In summary, single quotes stop all interpretation -- the string is rendered 
literally. Double quotes leave $ (dollar sign), ` (backquote) as special, 
and \ (backslash) as special when followed by certain other characters. 
And ! will be treated specially
*/
/*
WILDCARD:
ls *
expands to
ls header _infos/ Makefile minishell obj README.md src

in the directory:
header  _infos  Makefile  minishell  obj  README.md  src

Idea to create wildcard: 
1. Use opendir and readdir to open and read the directory specified by the token.
2. Match the wildcard pattern with the entries in the directory.
3. Append matching filenames to the arg list.
*/
