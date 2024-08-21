# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ademarti <ademarti@student.42berlin.de     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/30 17:25:14 by bschneid          #+#    #+#              #
#    Updated: 2024/08/21 12:49:32 by ademarti         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
HEADER = header/minishell.h
LIBFT = src/Libft_extended/libft.a
SRC_DIR = src
OBJ_DIR = obj
SOURCES = evaluation.c main.c split_tokens.c tokenize.c wildcard.c environ_var.c built_ins.c

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

# tokenize: src/tokenize.c src/split_tokens.c
# 	cc $(FLAGS) src/tokenize.c src/split_tokens.c -o $@ -lreadline -L./src/Libft_extended -lft

# split: src/split_tokens.c
# 	cc $(FLAGS) src/split_tokens.c -o $@ -lreadline -L./src/Libft_extended -lft

clean:
	rm -rf $(OBJ_DIR)

fclean:	clean
	rm -f $(NAME)
	rm -f $(LIBFT)

re:	fclean all

.PHONY:	all bonus clean fclean re
