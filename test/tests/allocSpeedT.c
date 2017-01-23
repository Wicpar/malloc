/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   allocSpeedT.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnieto <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/23 13:49:13 by fnieto            #+#    #+#             */
/*   Updated: 2017/01/23 16:17:14 by fnieto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "stdlib.h"
#include "stdio.h"

void	test(void)
{
	volatile long	*elems[100000];
	long	i;

	i = -1;
	while(++i < 100000)
	{
		elems[i] = malloc(4);
		elems[i][0] = i;
		printf("%lx: %lx\n", (unsigned long)(elems[i]), elems[i][0]);
	}
}
