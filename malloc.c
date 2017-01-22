/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnieto <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/21 17:26:28 by fnieto            #+#    #+#             */
/*   Updated: 2017/01/22 22:59:45 by fnieto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "util.h"
#include <sys/resource.h>
#include <sys/mman.h>
#include <unistd.h>

static t_data g_data;

void			*memalloc(size_t size)
{
	void	*mem;
	long	l;
	long	o;
	void	*tmp;

	mem = malloc(size);
	if (!mem)
		return (0);
	tmp = mem - 8;
	l = size / 8;
	o = size % 8;
	while (--l >= 0)
		*((long*)(tmp += 8)) = 0;
	while (--o >= 0)
		*((char*)++tmp) = 0;
	return (mem);
}

t_ubyte			least_unset_bit(t_ulong *map, t_ubyte flip)
{
	const t_ulong	masks[] = {1, 3, 0xF, 0xFF, 0xFFFF, 0xFFFFFFFF};
	t_ulong			mask;
	t_ulong			range;
	t_ubyte			offset;
	int				i;

	mask = 0xFFFFFFFF;
	range = ~0;
	offset = 0;
	i = 5;
	while (--i >= 0)
	{
		if (!(*map & mask))
			break ;
		else if ((*map & mask) != ~0)
			range = mask;
		else
		{
			offset += (1 << (i + 1));
			range ^= mask;
		}
		mask = ((masks[i]) << offset);
	}
	*map ^= (!!flip) << offset;
	return (offset);
}

void			*handle_alloc(t_block *block, size_t max)
{
	if (!block->mem)
	{
		block->mem = mmap(0, max * 128, RW, MAP_ANON | MAP_PRIVATE, -1, 0);
		if (block->mem == (void*)-1)
		{
			block->mem = 0;
			return (0);
		}
		block->map[0] = 0;
		block->map[1] = 0;
		block->next = memalloc(sizeof(t_block));
	}
	if (block->map[0] == ~0 && block->map[1] == ~0)
		return (handle_alloc(block->next, max));
	return (block->mem + (size_t)least_unset_bit(block->map[0] != ~0 ?
				block->map : block->map + sizeof(t_ulong), 1) * max);
}

void			*handle_large_alloc(size_t size)
{
	return (0);
}

void			*malloc(size_t size)
{
	if (!size)
		return (0);
	if (!g_data.block_size)
		g_data = init();
	if (size <= g_data.partitions[0].max_size)
		return (handle_alloc(&g_data.partitions[0].origin, g_data.partitions[0].max_size));
	else if (size <= g_data.partitions[1].max_size)
		return (handle_alloc(&g_data.partitions[1].origin, g_data.partitions[1].max_size));
	else
	{
	}
	return (0);
}

void			free(void *ptr)
{
	if (!ptr)
		return ;
}

void			*realloc(void *ptr, size_t size)
{
	if (!g_data.block_size)
		return (0);
	return (ptr + size);
}

t_data			init(void)
{
	t_data			data;
	struct rlimit	lim;
	size_t			size;

	data.block_size = getpagesize();
	getrlimit(RLIMIT_AS, &lim);
	data.max_blocks = lim.rlim_cur / data.block_size;
	data.allocated_blocks = 0;
	size = data.block_size / 64;
	data.partitions[0].name = "TINY";
	data.partitions[0].max_size = size;
	data.partitions[1].name = "SMALL";
	data.partitions[1].max_size = size * 16;
	data.partitions[2].name = "LARGE";
	data.partitions[2].max_size = -1;
	return (data);
}
