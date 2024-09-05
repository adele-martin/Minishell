# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/30 17:25:14 by bschneid          #+#    #+#              #
#    Updated: 2024/09/05 14:09:07 by bschneid         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
HEADER = header/minishell.h
LIBFT = src/Libft_extended/libft.a
SRC_DIR = src
OBJ_DIR = obj
SOURCES = ast.c built_ins.c evaluation.c helpers.c heredoc.c init.c main.c\
	parsing.c redirections.c signals.c split_cmd.c tokenization.c var_expand.c\
	var_helpers.c wildcard.c linkedlist_utils.c variables.c var_init.c utils.c
OBJECTS = $(patsubst %.c, $(OBJ_DIR)/%.o, $(SOURCES))
#FLAGS = -Wall -Werror -Wextra -g3 -I/usr/local/opt/readline/include
FLAGS = -Wall -Werror -Wextra -g3

all:	$(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	gcc $(FLAGS) -c $< -o $@

$(NAME): $(OBJECTS) $(HEADER) $(LIBFT)
	cc $(FLAGS) $(OBJECTS) -o $@ -lreadline -L./src/Libft_extended -lft
#cc $(FLAGS) $(OBJECTS) -o $@ -lreadline -L./src/Libft_extended -L/usr/local/opt/readline/lib -lft
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
