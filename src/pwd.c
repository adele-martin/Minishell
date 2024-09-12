#include "../header/minishell.h"

// prints out the current working directory
int	builtin_pwd(void)
{
	char	cwd[1024];

	getcwd(cwd, sizeof(cwd));
	ft_printf("%s\n", cwd);
	return (0);
}