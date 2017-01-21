/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnieto <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/21 17:26:28 by fnieto            #+#    #+#             */
/*   Updated: 2017/01/21 18:47:37 by fnieto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "util.h"
#include <sys/resource.h>
#include <sys/mman.h>
#include <unistd.h>

const static malloc_partition_t sizes[3];

void	*malloc(size_t size)
{
	return ((void*)size + (ulong)sizes);
}

void	free(void *ptr)
{
	*(int*)ptr = 0;
}

void	*realloc(void *ptr, size_t size)
{
	return (ptr + size);
}
