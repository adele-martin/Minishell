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
BI = src/builtins
EX = src/execution
PA = src/parsing
ST = src/syntax_tree
OT = src/others
OBJ_DIR = obj
SOURCES = $(BI)/cd.c $(BI)/echo.c $(BI)/env.c $(BI)/exit.c\
	$(BI)/export.c $(BI)/pwd.c $(BI)/unset.c\
	$(EX)/exec_help.c $(EX)/exec.c $(EX)/split_cmd.c \
	$(EX)/var_expand.c $(EX)/var_helpers.c $(EX)/wildcard.c\
	$(ST)/ast.c $(ST)/tokens_help.c $(ST)/tokens.c\
	$(OT)/debug_funcs.c $(OT)/error_memory.c $(OT)/init.c $(OT)/signals.c\
	$(OT)/utils.c\
	$(PA)/helpers.c $(PA)/heredoc.c $(PA)/parsing.c $(PA)/redirections.c\
	src/linkedlist_utils.c src/main.c src/var_init.c src/variables.c
	 
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))
FLAGS = -Wall -Werror -Wextra -g3 -I/usr/local/opt/readline/include -g -O0
#FLAGS = -Wall -Werror -Wextra -g3

all:	$(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
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
