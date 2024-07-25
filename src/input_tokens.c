

#include "../header/minishell.h"

// 0 : no operator
// 1 : ( ) | < >
// 2 : && || << >>
char	is_operator(char *str)
{
	if (!ft_strncmp(str, "&&", 2) || !ft_strncmp(str, "||", 2))
		return (2);
	else if (!ft_strncmp(str, ">>", 2) || !ft_strncmp(str, "<<", 2))
		return (2);
	else if (!ft_strncmp(str, "(", 1) || !ft_strncmp(str, ")", 1))
		return (1);
	else if (!ft_strncmp(str, ">", 1) || !ft_strncmp(str, "<", 1))
		return (1);
	else if (!ft_strncmp(str, "|", 1))
		return (1);
	return (0);		
}

size_t	add_len(char *str)
{
	size_t	add_len;
	char	in_single;
	char	in_double;

	in_single = 0;
	in_double = 0;
	add_len = 0;
	while (*str)
	{
		if ((in_single && *str != '\'') || (in_double && *str != '"'))
			;
		else if (in_single && *str == '\'')
			in_single = 0;
		else if (in_double && *str == '"')
			in_double = 0;
		else if (*str == '\'')
			in_single = 1;
		else if (*str == '"')
			in_double = 1;
		else if (!is_operator(str) && *str != ' ' && is_operator(str + 1))
			add_len++;
		else if (is_operator(str))
		{
			while (is_operator(str))
			{
				str += is_operator(str);
				if (*str && *str != ' ')
					add_len++;
			}
			str--;
		}
		str++;
	}
	return (add_len);
}

char	*clean_input(char *str)
{
	char	*out;
	char	*copy;
	char	in_single;
	char	in_double;
	
	out = (char *)malloc((ft_strlen(str) + add_len(str) + 1) * sizeof(char));
	if (!out)
		return (NULL);
	copy = out;

	in_single = 0;
	in_double = 0;
	while (*str)
	{
		if (in_single || in_double)
		{
			if (in_single && *str == '\'')
				in_single = 0;
			else if (in_double && *str == '"')
				in_double = 0;
		}
		else if (*str == '\'')
			in_single = 1;
		else if (*str == '"')
			in_double = 1;
		else if (!is_operator(str) && *str != ' ' && is_operator(str + 1))
		{
			*(copy++) = *(str++);
			*(copy++) = ' ';
			continue ;
		}
		else if (is_operator(str))
		{
			if (is_operator(str) == 1)
				*(copy++) = *(str++);
			else
			{
				*(copy++) = *(str++);
				*(copy++) = *(str++);
			}
			if (*str && *str != ' ')
				*(copy++) = ' ';
			continue ;
		}
		*(copy++) = *(str++);
	}
	*copy = '\0';
	return (out);
}

int	main(int argc, char **argv)
{
	if (argc == 2)
		printf("%s", clean_input(argv[1]));
	return (0);
}
