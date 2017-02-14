/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   all_once.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnieto <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/12 18:37:41 by fnieto            #+#    #+#             */
/*   Updated: 2017/02/14 13:58:37 by fnieto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "util.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>

void	test(void)
{
	char	*mem;
	char	*mem2;

	mem = malloc(10);
	mem2 = malloc(100);
	//printf("%s\n", strerror(errno));
	printf("%p\n", mem);
	printf("%p\n", mem2);
	show_alloc_mem();
	//mem = realloc(mem, 1000);
	//printf("%s\n", strerror(errno));
	//printf("%p\n", mem);
	//mem[0] = 'a';
	//mem[1] = '\n';
	//mem[2] = 0;
	//printf("%s\n", mem);
	free(mem);
	show_alloc_mem();
	free(mem2);
	show_alloc_mem();
	//printf("%s\n", strerror(errno));

}
