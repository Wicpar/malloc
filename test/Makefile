# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fnieto <fnieto@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2013/11/19 14:09:58 by fnieto            #+#    #+#              #
#    Updated: 2017/02/18 20:23:03 by fnieto           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME=test

CC=gcc

CFLAGS=-g

RM=rm -f

SRC=main.c

TNAMES=$(shell ls -1 tests)

TSRC=$(addprefix tests/,$(TNAMES))

LIB= -L ./ -lft_malloc

INCL=-I ../includes/

OBJ=$(SRC:.c=.o)
TOBJ=$(TSRC:.c=.o)

$(NAME): lib $(OBJ) $(TOBJ) FORCE

tests/%.o: tests/%.c lib $(OBJ) FORCE
	@$(CC) $(CFLAGS) $(INCL) -c $< -o $@
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $@ $(INCL) $(LIB)
	@echo $@
	@time ./run.sh ./test

%.o: %.c
	@$(CC) $(CFLAGS) $(INCL) -c $< -o $@

lib:
	@make -C ../
	@cp ../libft_malloc.so .
	@cp ../libft_malloc_x86_64_Darwin.so .

all: $(NAME)

clean:
	make -C ../ fclean
	$(RM) $(OBJ)
	$(RM) $(TOBJ)
	$(RM) *.so

fclean: clean
	make -C ../ fclean
	$(RM) $(NAME)

re: fclean all

FORCE:

.PHONY: clean fclean FORCE lib
