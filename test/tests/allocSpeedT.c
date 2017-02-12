/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   allocSpeedT.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnieto <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/23 13:49:13 by fnieto            #+#    #+#             */
/*   Updated: 2017/02/12 19:58:04 by fnieto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "stdlib.h"
#include "stdio.h"

#define iter 1
void	test(void)
{
	volatile long	*elems[iter];
	long	i;

	i = -1;
	while(++i < iter)
	{
		elems[i] = malloc(4);
		elems[i][0] = i;
		printf("%lx: %lx\n", (unsigned long)(elems[i]), elems[i][0]);
	}
}
