# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fnieto <fnieto@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/04/30 19:43:25 by fnieto            #+#    #+#              #
#    Updated: 2017/02/18 18:04:59 by fnieto           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME=libft_malloc_$(HOSTTYPE).so

LINKNAME=libft_malloc.so

CC=gcc

#fpic for position Intependent code
CCFLAGS= -Wall -Wextra -Werror -fPIC
DLFLAGS= -shared $(CCFLAGS)

RM=rm -f

SRC=malloc.c\
	calloc.c\
	realloc.c\
	free.c\
	init.c\
	show_alloc_mem.c\

OBJ=$(SRC:.c=.o)

INCL=includes/

LIB=-lpthread

$(NAME): $(OBJ) $(INCL)/malloc.h $(INCL)/util.h
	@$(CC) $(DLFLAGS) -o $@ $(OBJ) $(LIB)
	@echo "[.o] >> $(NAME)"
	@ln -sf $(NAME) $(LINKNAME)

%.o: %.c
	$(CC) -I $(INCL) $(CFLAGS) -c $< -o $@
	@echo "$< >> $@"

all: $(NAME)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)
	$(RM) $(LINKNAME)

norminette:
	@norminette *.c includes/*.h

re: fclean all

.PHONY: clean fclean
