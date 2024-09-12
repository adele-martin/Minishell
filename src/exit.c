#include "../header/minishell.h"

int	handle_plus_or_minus(char c)
{
	if (c == '+' || c == '-')
		return (1);
	else
		return (0);
}

// with no arguments, exits with status 0
// with args, checks for numeric (first) argument, optional error message
// if more than one argument, prints error
// if one argument, exits with that status
int	builtin_exit(char **argv, int argc, t_data *data)
{
	int	i;

	i = 0;
	if (argc == 1)
		exit(0);
	if (handle_plus_or_minus(argv[1][i]))
		i++;
	ft_printf("exit\n");
	while (argv[1][i])
	{
		if (!ft_isdigit(argv[1][i]))
		{
			error_message("exit", argv[1], "numeric argument required");
			exit (ft_free(data, 2));
		}
		i++;
	}
	if (argc > 2)
		return (error_message("exit", NULL, "too many arguments"), 1);
	exit(ft_free(data, ft_atoi(argv[1])));
}
