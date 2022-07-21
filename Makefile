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
CFLAGS += -g
CFLAGS += -fsanitize=address

# SOURCE FILES
SRCD := src/
SRCS := main.c																\
		minishell.c															\
		error.c																\
		lexer/tokenize.c													\
		lexer/lex/lex_operator.c											\
		lexer/lex/lex_simple.c												\
		lexer/lex/lex_variable.c											\
		lexer/evaluate.c													\
		lexer/preparse.c													\
		lexer/preparse/expand_var.c											\
		lexer/token_utils.c													\
		parser/parse.c														\
		parser/init_node.c													\
		parser/parse/parse_nodelist.c										\
		parser/parse/parse_command.c										\
		parser/parse/parse_pipeline.c										\
		parser/parse/parse_logic.c
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
		parse.h
INCP := $(addprefix $(INCD), $(INCS))

HEADERS += $(INCP)

# TEST FILES
TEST_DIR := test/

TEST_EXE := $(addprefix $(TEST_DIR), .test)
TEST_RESD := $(addprefix $(TEST_DIR), res/)

TEST_SRCD := $(addprefix $(TEST_DIR), $(SRCD))
TEST_SRCS := lexer.c														\
			 preparser.c													\
			 parser.c
TEST_SRCP := $(addprefix $(TEST_SRCD), $(TEST_SRCS))

TEST_LIB := $(addprefix $(TEST_DIR), $(addsuffix _test.a, $(NAME)))
TEST_LIB_OBJS := $(filter-out $(OBJD)main.o, $(OBJP))

TEST_LIBS += $(TEST_LIB)
TEST_LIBS += $(LIBFT_L)

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
LIBS += -lreadline

# 		MAC SPECIFIC LINKING
ifeq ($(shell uname), Darwin)
	LIBS += -L$(shell brew --prefix readline)
endif

#		LINUX SPECIFIC LINKIG
ifeq ($(shell uname), Linux)
	LIBS += -lhistory
endif

#		RANDOM THINGS
COMPILE := @$(CC) $(CFLAGS)

# RECIPES
all: $(NAME) test

$(NAME): $(LIBFT_L) $(OBJP)
	@echo "Compiling main executable!"
	$(COMPILE) $(OBJP) $(LIBS) -o $(NAME)

$(OBJD)%.o: $(SRCD)%.c $(HEADERS)
	@mkdir -p $(@D)
	@echo "Compiling: $<"
	$(COMPILE) -c -o $@ $<

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

test: $(TEST_LIBS)
	@$(foreach test, $(TEST_SRCP), \
		echo "Running test $(test)"; \
		$(eval TESTNAME := $(TEST_RESD)$(basename $(notdir $(test)))) \
		$(CC) $(test) $(TEST_LIBS) $(CFLAGS)/ -o $(TEST_EXE); \
		cat $(TESTNAME) | $(TEST_EXE) > $(TESTNAME)-output 2> /dev/null; \
		rm -f $(TESTNAME)-diff; \
		diff $(TESTNAME)-expected $(TESTNAME)-output > $(TESTNAME)-diff; \
		if [ -s $(TESTNAME)-diff ]; then echo "[KO] $(test) - files differ"; fi; \
		echo "Test $(test) done"; \
	)
	@rm -f $(TEST_EXE)

$(TEST_LIB): $(NAME) $(TEST_LIB_OBJS) $(LIBFT_L)
	@ar -cr $(TEST_LIB) $(TEST_LIB_OBJS)
	@echo "Done creating archive $(TEST_LIB)"

generate_test_files: $(TEST_LIBS)
	@$(foreach test, $(TEST_SRCP), \
		echo "Generating files for $(test)"; \
		$(eval TESTNAME := $(TEST_RESD)$(basename $(notdir $(test)))) \
		$(CC) $(test) $(TEST_LIBS) $(CFLAGS)/ -o $(TEST_EXE); \
		cat $(TESTNAME) | $(TEST_EXE) > $(TESTNAME)-expected 2> /dev/null; \
	)

cleantest:
	@rm -f $(TEST_RESD)*-diff $(TEST_RESD)*-output
	@rm -f $(TEST_LIB)

.PHONY: all clean fclean re test
