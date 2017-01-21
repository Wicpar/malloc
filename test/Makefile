# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fnieto <fnieto@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2013/11/19 14:09:58 by apachkof          #+#    #+#              #
#    Updated: 2017/01/21 19:40:50 by fnieto           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME=test

CC=gcc

CFLAGS=-Wall -Wextra -Werror -O3

RM=rm -f

SRC=main.c

LIB= -L ../ -lft_malloc

INCL=-I ../includes/

OBJ=$(SRC:.c=.o)

$(NAME): $(OBJ)
		make -C ../
		$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(INCL) $(LIB)

%.o: %.c
		$(CC) $(CFLAGS) $(INCL) -c $< -o $@

all: $(NAME)

clean:
		make -C ../ fclean
		$(RM) $(OBJ)

fclean: clean
		make -C ../ fclean
		$(RM) $(NAME)

re: fclean all

.PHONY: clean fclean
