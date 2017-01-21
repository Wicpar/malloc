# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fnieto <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/04/30 19:43:25 by fnieto            #+#    #+#              #
#    Updated: 2017/01/21 19:34:49 by fnieto           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME=libft_malloc_$(HOSTTYPE).so

LINKNAME=libft_malloc.so

CC=gcc

CCFLAGS= -fPIC -Wall -Wextra -Werror -O3

RM=rm -f

SRC=malloc.c\

OBJ=$(SRC:.c=.o)

INCL=includes/

$(NAME): $(OBJ) $(INCL)/malloc.h
	@$(CC) -shared -o $(NAME) $(OBJ)
	@echo "[.o] >> $(NAME)"
	@ln -s $(NAME) $(LINKNAME)

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
