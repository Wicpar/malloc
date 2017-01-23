/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnieto <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/21 17:26:28 by fnieto            #+#    #+#             */
/*   Updated: 2017/01/23 16:38:18 by fnieto           ###   ########.fr       */
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

	range = ~0;
	offset = 0;
	i = 6;
	while (--i >= 0)
	{
		mask = ((masks[i]) << offset);
		if (!(*map & mask))
			break ;
		else if ((*map & mask) != mask)
			range &= mask;
		else
		{
			offset += (1L << i);
			range ^= mask;
		}
	}
	*map ^= (flip & 1L) << offset;
	return (offset);
}

void			*handle_alloc(t_block *block, t_ulong *filter, size_t max)
{
	if (!block)
		return (0);
	if (!block->mem)
	{
		block->mem = mmap(0, max * F, RW, MAP_ANON | MAP_PRIVATE, -1, 0);
		if (block->mem == (void*)-1)
		{
			block->mem = 0;
			return (0);
		}
		*filter |= (t_ulong)block->mem * FVN;
		block->map[0] = 0;
		block->map[1] = 0;
		block->next = memalloc(sizeof(t_block));
	}
	if (block->map[0] != ~0 || block->map[1] != ~0)
	{
		if (block->map[0] != ~0)
			return (block->mem + max * least_unset_bit(block->map, 1));
		else
			return (block->mem + max * (least_unset_bit(&(block->map[1]), 1) + H));
	}
	return (handle_alloc(block->next, filter, max));
}

void			*handle_large_alloc(t_block *block, t_ulong *fil, size_t size)
{
	size_t	alloc;

	if (!block)
		return (0);
	if (block->mem)
	{
		return (handle_large_alloc(block, fil, size));
	}
	else
	{
		alloc = (size + g_data.block_size - 1) / g_data.block_size;
		block->mem = mmap(0, alloc, RW, MAP_ANON | MAP_PRIVATE, -1, 0);
		if (block->mem == (void*)-1)
		{
			block->mem = 0;
			return (0);
		}
		*fil |= (t_ulong)block->mem * FVN;
		block->map[0] = size;
		block->map[1] = alloc;
		block->next = memalloc(sizeof(t_block));
		return (block->mem);
	}
}

void			*malloc(size_t size)
{
	if (!size)
		return (0);
	if (!g_data.block_size)
		g_data = init();
	if (size <= g_data.parts[0].max_size)
		return (handle_alloc(&g_data.parts[0].origin, &g_data.parts[0].filter,
					g_data.parts[0].max_size));
	else if (size <= g_data.parts[1].max_size)
		return (handle_alloc(&g_data.parts[1].origin, &g_data.parts[1].filter,
					g_data.parts[1].max_size));
	else
		return (handle_large_alloc(&g_data.parts[2].origin,
					&g_data.parts[2].filter, g_data.parts[2].max_size));
}

void			free(void *ptr)
{
	if (!ptr || !g_data.block_size)
		return ;

}

void			*realloc(void *ptr, size_t size)
{
	t_ulong		hash;
	t_ulong		align;
	t_block		*t;
	t_block		*s;
	t_block		*l;

	if (!g_data.block_size)
		return (0);
	align = (t_ulong)ptr / g_data.block_size / 2;
	hash = align * g_data.block_size * 2 * FVN;
	t = ((hash & g_data.parts[0].filter) == hash) ? &g_data.parts[0].origin : 0;
	hash = align / 16 * g_data.block_size * 32 * FVN;
	s = ((hash & g_data.parts[1].filter) == hash) ? &g_data.parts[0].origin : 0;
	hash = (t_ulong)ptr * FVN;
	l = ((hash & g_data.parts[2].filter) == hash) ? &g_data.parts[0].origin : 0;
	while (1)
		if (t || s || l)
		{
			if (t)
				if(ptr > t->mem && ptr < t->mem + g_data.parts[0].max_size * F)
				{
					if (size < g_data.parts[0].max_size)
						return (ptr);
				}
		}
		else
			break;
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
	size = data.block_size / F;
	data.parts[0].name = "TINY";
	data.parts[0].max_size = size;
	data.parts[1].name = "SMALL";
	data.parts[1].max_size = size * Q;
	data.parts[2].name = "LARGE";
	data.parts[2].max_size = -1;
	return (data);
}
