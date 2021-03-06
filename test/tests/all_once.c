/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   all_once.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnieto <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/12 18:37:41 by fnieto            #+#    #+#             */
/*   Updated: 2017/02/16 15:31:03 by fnieto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include "../../includes/malloc.h"
//#include "util.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

void	test(void)
{
	char	*mem;
	char	*mem2;

//	show_alloc_mem();
	write(1, "tes0\n", 5);
	mem = malloc(10);
	write(1, "test\n", 5);
//	printf("%p\n", mem);
//	show_alloc_mem();
	mem2 = malloc(100);
//	printf("%p\n", mem2);
//	show_alloc_mem();
	mem2 = realloc(mem2, 1000);
	*mem = 100;
//	show_alloc_mem();
	free(mem);
//	show_alloc_mem();
	free(mem2);
//	show_alloc_mem();

}
