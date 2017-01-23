/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   allocTiny.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnieto <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/22 22:22:34 by fnieto            #+#    #+#             */
/*   Updated: 2017/01/23 10:40:10 by fnieto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

void	test(void)
{
	char	*mem;

	mem = malloc(0);
	printf("%p\n", mem);
	mem = malloc(3);
	printf("%p\n", mem);
//	mem[0] = 'a';
//	mem[1] = '\n';
//	mem[2] = 0;
	printf("%c\n", *mem);
}
