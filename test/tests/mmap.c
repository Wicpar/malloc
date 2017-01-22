/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mmap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnieto <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/22 19:01:09 by fnieto            #+#    #+#             */
/*   Updated: 2017/01/22 19:04:16 by fnieto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "util.h"
#include <unistd.h>
#include <stdio.h>

void	test(void)
{
	void	*mem;

	mem = mmap(0, getpagesize(), RW, 0, 0, 0);
	printf("%p\n", mem);
	munmap(mem, getpagesize());
}
