NAME = minishell

CC = gcc
CFLAGS += $(INCLUDES) -Wall -Werror -Wextra

OBJ_DIR = obj/
SRC_DIR = src/
INC_DIR = include/
INCLUDES += -I $(INC_DIR)

SRCS = minishell.c															\
	   lexer/tokenize.c														\
	   lexer/matcher.c
HEADERS = minishell.h														\
		  token.h
HEADER_FILES += $(addprefix $(INC_DIR), $(HEADERS))
OBJS = $(SRCS:.c=.o)
OBJS_PREFIXED = $(addprefix $(OBJ_DIR), $(OBJS))

LIBFT_DIR = libft/
LIBFT_HEADERS = libft.h\
				get_next_line.h\
				ft_printf.h
HEADER_FILES += $(addprefix $(LIBFT_INC), $(LIBFT_HEADERS))
LIBFT_LIB = $(addprefix $(LIBFT_DIR), libft.a)
LIBFT_INC = $(addprefix $(LIBFT_DIR), $(INC_DIR))
INCLUDES += -I $(LIBFT_INC)

LIBS += $(LIBFT_LIB)
LIBS += -lreadline
LIBS += -lhistory

all: $(NAME)

$(NAME): $(LIBFT_LIB) $(OBJS_PREFIXED)
	@echo "Compiling main executable!"
	@$(CC) $(CFLAGS) $(OBJS_PREFIXED) $(LIBS) -o $(NAME)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c $(HEADER_FILES)
	@mkdir -p $(@D)
	@echo "Compiling: $<"
	@$(CC) $(CFLAGS) -c -o $@ $<

$(LIBFT_LIB):
	@$(MAKE) -C $(LIBFT_DIR)

clean:
	@rm -rf $(OBJ_DIR)
	@echo "Done cleaning $(CURDIR)/$(OBJ_DIR)"
	@$(MAKE) -C $(LIBFT_DIR) clean

fclean:
	@rm -f $(NAME)
	@$(MAKE) clean
	@$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean
	@$(MAKE)

.PHONY: all clean fclean re