# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: lsinke <lsinke@student.codam.nl>             +#+                      #
#                                                    +#+                       #
#    Created: 2022/06/30 18:29:13 by lsinke        #+#    #+#                  #
#    Updated: 2022/07/01 13:53:55 by pmolnar       ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

NAME := minishell

CC := gcc

CFLAGS += -Wall -Werror -Wextra
CFLAGS += -I $(INCD)

# SOURCE FILES
SRCD := src/
SRCS := main.c																\
		minishell.c															\
		handle_input.c														\
		error.c																\
		environ/environment.c												\
		environ/get_variable.c												\
		environ/set_variable.c												\
		environ/variable_utils.c											\
		signals/signal.c													\
		signals/rl_signal.c													\
		lexer/tokenize.c													\
		lexer/lex/lex_operator.c											\
		lexer/lex/lex_whitespace.c											\
		lexer/lex/lex_word.c												\
		lexer/token_utils.c													\
		parser/parse.c														\
		parser/init_node.c													\
		parser/destroy_node.c												\
		parser/parse/parse_subshell.c										\
		parser/parse/parse_command.c										\
		parser/parse/parse_pipeline.c										\
		parser/parse/parse_logic.c											\
		parser/parse/parse_redir.c											\
		redirect/create_heredoc.c											\
		redirect/read_heredoc.c												\
		redirect/write_heredoc.c											\
		redirect/redirect.c													\
		executor/execute.c													\
		executor/fork_utils.c												\
		executor/execute_subshell.c											\
		executor/execute_command.c											\
		executor/execute_pipeline.c											\
		executor/execute_logic.c											\
		executor/execute_binary.c											\
		executor/builtins/builtins.c										\
		executor/builtins/builtin_cd.c										\
		executor/builtins/builtin_echo.c									\
		executor/builtins/builtin_env.c										\
		executor/builtins/builtin_exit.c									\
		executor/builtins/builtin_export.c									\
		executor/builtins/builtin_unset.c									\
		executor/builtins/builtin_pwd.c										\
		executor/expand/expand_variables.c									\
		executor/expand/split_words.c										\
		executor/expand/expand_filenames.c									\
		executor/expand/remove_quotes.c										\
		executor/expand/glob_utils.c										\
		type_utils/token_utils.c											\
		type_utils/wordlist_utils.c

SRCP := $(addprefix $(SRCD), $(SRCS))

# OBJECT FILES
OBJD := obj/
OBJS := $(SRCS:.c=.o)
OBJP := $(addprefix $(OBJD), $(OBJS))

# HEADER FILES
INCD := include/
INCS := minishell.h															\
		input.h																\
		token.h																\
		parse.h																\
		execute.h															\
		redir.h																\
		builtins.h															\
		environ.h															\
		signals.h															\
		ms_types.h															\
		error.h

INCP := $(addprefix $(INCD), $(INCS))

HEADERS += $(INCP)

# LIBRARIES

#		LIBFT
LIBFT_D := libft/
LIBFT_N := libft.a
LIBFT_H := libft.h															\
		   get_next_line.h													\
		   dynarr.h
LIBFT_I := $(addprefix $(LIBFT_D), $(INCD))
LIBFT_L := $(addprefix $(LIBFT_D), $(LIBFT_N))

CFLAGS += -I $(LIBFT_I)
LIBS += $(LIBFT_L)
HEADERS += $(addprefix $(LIBFT_I), $(LIBFT_H))

#		READLINE
LIBS += -lreadline -lhistory

# 		MAC SPECIFIC LINKING
ifeq ($(shell uname), Darwin)
	LIBS += -L$(shell brew --prefix readline)/lib
	CFLAGS += -I$(shell brew --prefix readline)/include
endif

#		RANDOM THINGS
COMPILE := @$(CC) $(CFLAGS)

# RECIPES
all: $(NAME)

$(NAME): $(LIBFT_L) $(OBJP)
	@echo "Compiling main executable!"
	$(COMPILE) $(OBJP) $(LIBS) -o $(NAME)

$(OBJD)%.o: $(SRCD)%.c $(HEADERS)
	@mkdir -p $(@D)
	@echo "Compiling: $<"
	$(COMPILE) -c -o $@ $<

$(LIBFT_L):
	@$(MAKE) -C $(LIBFT_D)

clean:
	@rm -rf $(OBJD)
	@echo "Done cleaning $(CURDIR)/$(OBJD)"
	@$(MAKE) -C $(LIBFT_D) clean

fclean:
	@rm -f $(NAME)
	@$(MAKE) clean
	@$(MAKE) -C $(LIBFT_D) fclean

re: fclean
	@$(MAKE)

.PHONY: all clean fclean re
