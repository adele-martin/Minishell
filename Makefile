# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
<<<<<<< HEAD
#    By: ademarti <ademarti@student.42berlin.de     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/30 17:25:14 by bschneid          #+#    #+#              #
#    Updated: 2024/08/15 12:23:58 by ademarti         ###   ########.fr        #
=======
#    By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/30 17:25:14 by bschneid          #+#    #+#              #
#    Updated: 2024/08/09 19:55:15 by bschneid         ###   ########.fr        #
>>>>>>> 0f26f591eecd9922b2083a19f20675a9278bc748
#                                                                              #
# **************************************************************************** #

NAME = minishell
HEADER = header/minishell.h
LIBFT = src/Libft_extended/libft.a
SRC_DIR = src
OBJ_DIR = obj
<<<<<<< HEAD
SOURCES = evaluation.c main.c split_tokens.c tokenize.c wildcard.c environ_var.c built-ins.c

=======
SOURCES = ast.c evaluation.c helpers.c main.c parsing.c redirections.c split_cmd.c tokenization.c wildcard.c
>>>>>>> 0f26f591eecd9922b2083a19f20675a9278bc748
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

<<<<<<< HEAD
# tokenize: src/tokenize.c src/split_tokens.c
# 	cc $(FLAGS) src/tokenize.c src/split_tokens.c -o $@ -lreadline -L./src/Libft_extended -lft

# split: src/split_tokens.c
# 	cc $(FLAGS) src/split_tokens.c -o $@ -lreadline -L./src/Libft_extended -lft

clean:
	rm -rf $(OBJ_DIR)
=======
clean:
	rm -rf $(OBJ_DIR)
	rm -f src/heredoc.txt
>>>>>>> 0f26f591eecd9922b2083a19f20675a9278bc748

fclean:	clean
	rm -f $(NAME)
	rm -f $(LIBFT)

re:	fclean all

.PHONY:	all bonus clean fclean re
