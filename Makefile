# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/30 17:25:14 by bschneid          #+#    #+#              #
#    Updated: 2024/07/12 18:15:03 by bschneid         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
HEADER = header/minishell.h
LIBFT = src/Libft_extended/libft.a
SRC_DIR = src
OBJ_DIR = obj
SOURCES = main.c evaluation.c split_tokens.c
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

tokenize: src/tokenize.c
	cc $(FLAGS) src/tokenize.c -o $@ -lreadline -L./src/Libft_extended -lft

split: src/split_tokens.c
	cc $(FLAGS) src/split_tokens.c -o $@ -lreadline -L./src/Libft_extended -lft

clean:
	rm -rf $(OBJ_DIR)

fclean:	clean
	rm -f $(NAME)
	rm -f $(LIBFT)

re:	fclean all

.PHONY:	all bonus clean fclean re
