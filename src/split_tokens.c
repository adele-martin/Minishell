

#include "../header/minishell.h"

static size_t	count_tokens(char *str);
static	char	write_all_tokens(char *str, char **tokens_out, size_t tokens);

// Allocates (with malloc(3)) and returns an array of token-strings 
// obtained by splitting on operators: ( ) | || && < << > >> . 
// The array ends with a NULL pointer.
char	**split_tokens(char *str)
{
	char	**tokens_out;
	size_t	tokens;

	if (!str)
		return (NULL);
	tokens = count_tokens(str);
	tokens_out = (char **)malloc((tokens + 1) * sizeof(char *));
	if (!tokens_out)
		return (NULL);
	if (!write_all_tokens(str, tokens_out, tokens))
		return (NULL);
	return (tokens_out);
}

// 0 : no operator
// 1 : ( ) | < >
// 2 : && || << >>
static size_t	is_operator(char *str)
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

static size_t	count_tokens(char *str)
{
	size_t	amount;
	char	in_single;
	char	in_double;
	
	amount = 0;
	in_single = 0;
	in_double = 0;
	while (*str)
	{
		if (*str == ' ')
			str++;
		else if (is_operator(str))				// cases with operators
		{
			amount++;
			str += is_operator(str);
		}
		else									// inside token-string
		{
			amount++;
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
				else if (is_operator(str))
					break ;
				str++;
			}
		}
	}
	return (amount);
}

static	char	*token_write(char const *str, size_t len)
{
	char	*out;
	char	*writer;

	out = (char *)malloc((len + 1) * sizeof(char));
	if (!out)
		return (NULL);
	writer = out;
	while (len--)
		*(writer++) = *(str++);
	*writer = '\0';
	return (out);
}

static	char	write_all_tokens(char *str, char **tokens_out, size_t tokens)
{
	char	**writer;
	size_t	len;
	char	in_single;
	char	in_double;

	writer = tokens_out;
	while (tokens--)
	{
		while (*str == ' ')
			str++;
		if (is_operator(str))					// cases with operators
			len = is_operator(str);
		else									// inside token-string
		{
			len = 0;
			in_single = 0;
			in_double = 0;
			while (str[len])
			{
				if (in_single || in_double)
				{
					if (in_single && str[len] == '\'')
						in_single = 0;
					else if (in_double && str[len] == '"')
						in_double = 0;
				}
				else if (str[len] == '\'')
					in_single = 1;
				else if (str[len] == '"')
					in_double = 1;
				else if (is_operator(str + len))
					break ;
				len++;
			}
			while (str[len - 1] == ' ')
				len--;
		}
		*writer = token_write(str, len);
		if (!(*(writer++)))
		{
			ft_split_free(tokens_out);
			return (0);
		}
		str += len;
	}
	*writer = 0;
	return (1);
}


int	main(int argc, char **argv)
{
	char	**splited;

	if (argc == 2)
	{
		splited = split_tokens(argv[1]);
		while (*splited)
		{
			printf("TOKEN: %s\n", *splited);
			splited++;
		}
	}
	return (0);
}
