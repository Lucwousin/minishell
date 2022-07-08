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
CFLAGS += -g
CFLAGS += -fsanitize=address

# SOURCE FILES
SRCD = src/
SRCS = main.c																\
	   minishell.c															\
	   lexer/tokenize.c														\
	   lexer/matcher.c														\
	   lexer/expander.c
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

# TEST FILES
TEST_DIR = test/

TEST_EXE = $(addprefix $(TEST_DIR), .test)
TEST_RESD = $(addprefix $(TEST_DIR), res/)

TEST_SRCD = $(addprefix $(TEST_DIR), $(SRCD))
TEST_SRCS = lexer.c
TEST_SRCP = $(addprefix $(TEST_SRCD), $(TEST_SRCS))

TEST_LIB = $(addprefix $(TEST_DIR), $(addsuffix _test.a, $(NAME)))
TEST_LIB_OBJS = $(filter-out $(OBJD)main.o, $(OBJP))

TEST_LIBS += $(TEST_LIB)
TEST_LIBS += $(LIBFT_L)

# LIBRARIES

#		LIBFT
LIBFT_D = libft/
LIBFT_N = libft.a
LIBFT_H = libft.h															\
		  get_next_line.h													\
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
all: $(NAME) test

$(NAME): $(LIBFT_L) $(OBJP)
	@echo "Compiling main executable!"
	@$(CC) $(CFLAGS) $(OBJP) $(LIBS) -o $(NAME)

$(OBJD)%.o: $(SRCD)%.c $(HEADERS)
	@mkdir -p $(@D)
	@echo "Compiling: $<"
	@$(CC) $(CFLAGS) -c -o $@ $<

$(LIBFT_L):
	@$(MAKE) -C $(LIBFT_D)

clean: cleantest
	@rm -rf $(OBJD)
	@echo "Done cleaning $(CURDIR)/$(OBJD)"
	@$(MAKE) -C $(LIBFT_D) clean

fclean:
	@rm -f $(NAME)
	@$(MAKE) clean
	@$(MAKE) -C $(LIBFT_D) fclean

re: fclean
	@$(MAKE)

test: $(TEST_LIB)
	@$(foreach test, $(TEST_SRCP), \
		echo "Running test $(test)"; \
		$(eval TESTNAME := $(TEST_RESD)$(basename $(notdir $(test)))) \
		$(CC) $(CFLAGS) $(TEST_LIBS) $(test) -o $(TEST_EXE); \
		$(TEST_EXE) > $(TESTNAME)-output; \
		rm -f $(TESTNAME)-diff; \
		diff $(TESTNAME)-expected $(TESTNAME)-output > $(TESTNAME)-diff; \
		if [ -s $(TESTNAME)-diff ]; then echo "[KO] $(test) - files differ"; fi;\
	)

$(TEST_LIB): $(NAME) $(TEST_LIB_OBJS)
	@ar -cr $(TEST_LIB) $(TEST_LIB_OBJS)
	@echo "Done creating archive $<"

generate_test_files: $(TEST_LIB)
	$(foreach test, $(TEST_SRCP), \
		@$(CC) $(CFLAGS) $(TEST_LIBS) $(test) -o $(TEST_EXE); \
		$(TEST_EXE) > $(TEST_RESD)/$(basename $(notdir $(test)))-expected; \
	)

cleantest:
	@rm -f $(TEST_RESD)*-diff $(TEST_RESD)*-output
	@rm -f $(TEST_LIB)
	@rm -f $(TEST_EXE)

.PHONY: all clean fclean re test
