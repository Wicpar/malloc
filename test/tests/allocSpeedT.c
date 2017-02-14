/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   allocSpeedT.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnieto <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/23 13:49:13 by fnieto            #+#    #+#             */
/*   Updated: 2017/02/14 21:15:28 by fnieto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "stdio.h"

#define iter 1500
void	test(void)
{
	volatile long	*elems[iter];
	long	i;

	show_alloc_mem();
	i = -1;
	while(++i < iter)
	{
		elems[i] = malloc(200);
		elems[i][0] = i;
	}
	show_alloc_mem();
	i = -1;
	while(++i < iter)
	{
		show_alloc_mem();
		free((void*)elems[i]);
	}
	show_alloc_mem();
}
