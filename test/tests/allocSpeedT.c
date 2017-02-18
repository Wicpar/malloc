/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   allocSpeedT.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnieto <fnieto@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/23 13:49:13 by fnieto            #+#    #+#             */
/*   Updated: 2017/02/18 15:25:51 by fnieto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "stdio.h"

#define iter 100
void	test(void)
{
	volatile long	*elems[iter];
	long	i;

	//show_alloc_mem();
	i = -1;
	while(++i < iter)
	{
		elems[i] = malloc(512);
		elems[i][0] = i;
	}
	//show_alloc_mem();
	i = -1;
	while(++i < iter)
	{
		free((void*)elems[i]);
	}
	//show_alloc_mem();
}
