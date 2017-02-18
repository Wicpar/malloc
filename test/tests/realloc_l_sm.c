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

	mem = malloc(10000);
	mem[0] = 100;
	mem[9999] = 100;
	mem = realloc(mem, 100);
	mem[0] = 100;
	mem[99] = 100;
	free(mem);
}
