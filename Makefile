# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ademarti <adelemartin@student.42.fr>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/30 17:25:14 by bschneid          #+#    #+#              #
#    Updated: 2024/09/11 14:54:47 by ademarti         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
HEADER = header/minishell.h
LIBFT = src/Libft_extended/libft.a
SRC_DIR = src
OBJ_DIR = obj
SOURCES = ast.c built_ins.c evaluation.c helpers.c heredoc.c init.c main.c\
	parsing.c redirections.c signals.c split_cmd.c tokenization.c var_expand.c\
	var_helpers.c wildcard.c linkedlist_utils.c variables.c var_init.c utils.c\
	error_memory.c cd.c exit.c export.c
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

fclean:	clean
	rm -f $(NAME)
	rm -f $(LIBFT)

re:	fclean all

.PHONY:	all bonus clean fclean re
