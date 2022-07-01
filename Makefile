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

NAME = minishell

CC = gcc

CFLAGS += -Wall -Werror -Wextra
CFLAGS += -I $(INCD)

# SOURCE FILES
SRCD = src/
SRCS = minishell.c															\
	   lexer/tokenize.c														\
	   lexer/matcher.c
SRCP = $(addprefix $(SRCD), $(SRCS))

# OBJECT FILES
OBJD = obj/
OBJS = $(SRCS:.c=.o)
OBJP = $(addprefix $(OBJD), $(OBJS))

# HEADER FILES
INCD = include/
INCS = minishell.h															\
	   token.h
INCP = $(addprefix $(INCD), $(INCS))

HEADERS += $(INCP)

# LIBRARIES

#		LIBFT
LIBFT_D = libft/
LIBFT_N = libft.a
LIBFT_H = libft.h															\
		  get_next_line.h													\
		  ft_printf.h														\
		  dynarr.h
LIBFT_I = $(addprefix $(LIBFT_D), $(INCD))
LIBFT_L = $(addprefix $(LIBFT_D), $(LIBFT_N))

CFLAGS += -I $(LIBFT_I)
LIBS += $(LIBFT_L)
HEADERS += $(addprefix $(LIBFT_I), $(LIBFT_H))

#		READLINE
LIBS += -lreadline

# 		MAC SPECIFIC LINKING
ifeq ($(shell uname), Darwin)
	LIBS += -L$(shell brew --prefix readline)
endif

#		LINUX SPECIFIC LINKIG
ifeq ($(shell uname), Linux)
	LIBS += -lhistory
endif

# RECIPES
all: $(NAME)

$(NAME): $(LIBFT_L) $(OBJP)
	@echo "Compiling main executable!"
	@$(CC) $(CFLAGS) $(OBJP) $(LIBS) -o $(NAME)

$(OBJD)%.o: $(SRCD)%.c $(HEADERS)
	@mkdir -p $(@D)
	@echo "Compiling: $<"
	@$(CC) $(CFLAGS) -c -o $@ $<

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
