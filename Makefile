# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/30 17:25:14 by bschneid          #+#    #+#              #
#    Updated: 2024/09/13 22:13:19 by bschneid         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
HEADER = header/minishell.h
LIBFT = src/Libft_extended/libft.a
SRC_DIR = src
OBJ_DIR = obj
SOURCES = ast.c cd.c echo.c env.c error_memory.c exec_help.c exec.c\
	exit.c export.c helpers.c heredoc.c init.c linkedlist_utils.c\
	main.c parsing.c pwd.c redirections.c signals.c split_cmd.c tokens_help.c\
	tokens.c unset.c utils.c var_expand.c var_helpers.c var_init.c variables.c\
	wildcard.c
	 
OBJECTS = $(patsubst %.c, $(OBJ_DIR)/%.o, $(SOURCES))
FLAGS = -Wall -Werror -Wextra -g3 -I/usr/local/opt/readline/include -g -O0
#FLAGS = -Wall -Werror -Wextra -g3

all:	$(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	cc $(FLAGS) -c $< -o $@

$(NAME): $(OBJECTS) $(HEADER) $(LIBFT)
#cc $(FLAGS) $(OBJECTS) -o $@ -lreadline -L./src/Libft_extended -lft
	cc $(FLAGS) $(OBJECTS) -g -o $@ -lreadline -L./src/Libft_extended -L/usr/local/opt/readline/lib -lft -L.
$(LIBFT):
	$(MAKE) -C src/Libft_extended -f Makefile bonus
	$(MAKE) -C src/Libft_extended -f Makefile clean

clean:
	rm -rf $(OBJ_DIR)

bonus:	all

fclean:	clean
	rm -f $(NAME)
	rm -f $(LIBFT)

re:	fclean all

.PHONY:	all bonus clean fclean re
