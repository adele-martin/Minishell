# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/30 17:25:14 by bschneid          #+#    #+#              #
#    Updated: 2024/08/09 19:55:15 by bschneid         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
HEADER = header/minishell.h
LIBFT = src/Libft_extended/libft.a
SRC_DIR = src
OBJ_DIR = obj
SOURCES = ast.c evaluation.c helpers.c main.c parsing.c redirections.c split_cmd.c tokenization.c wildcard.c
OBJECTS = $(patsubst %.c, $(OBJ_DIR)/%.o, $(SOURCES))
FLAGS = -Wall -Werror -Wextra -g3

all:	$(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	cc $(FLAGS) -c $< -o $@

$(NAME): $(OBJECTS) $(HEADER) $(LIBFT)
	cc $(FLAGS) $(OBJECTS) -o $@ -lreadline -L./src/Libft_extended -lft

$(LIBFT):
	$(MAKE) -C src/Libft_extended -f Makefile bonus
	$(MAKE) -C src/Libft_extended -f Makefile clean

clean:
	rm -rf $(OBJ_DIR)
	rm -f src/heredoc.txt

fclean:	clean
	rm -f $(NAME)
	rm -f $(LIBFT)

re:	fclean all

.PHONY:	all bonus clean fclean re
