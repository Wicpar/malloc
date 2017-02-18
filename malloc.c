/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnieto <fnieto@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/21 17:26:28 by fnieto            #+#    #+#             */
/*   Updated: 2017/02/18 20:16:51 by fnieto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "util.h"

t_data			g_ftmd;

void					*ft_bzero(void *mem, size_t size)
{
	void *tmp;

	tmp = mem;
	while (size--)
		*((char*)tmp++) = 0;
	return (mem);
}

static t_ubyte			least_unset_bit(t_ulong *map, t_ubyte flip)
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

static void				*handle_alloc(t_block *blc, size_t max, int pid)
{
	if (!blc)
		return (0);
	if (!blc->mem)
	{
		blc->mem = mmap(0, max * F, RW, MAP_ANON | MAP_PRIVATE, -1, 0);
		if (blc->mem == (void*)-1)
		{
			blc->mem = 0;
			return (0);
		}
		blc->map[0] = 0;
		blc->map[1] = 0;
		blc->next = ft_bzero(malloc(sizeof(t_block)), sizeof(t_block));
		g_ftmd.parts[pid].first_free = blc;
	}
	if (blc->map[0] != ~0)
		return (blc->mem + max * least_unset_bit(&(blc->map[0]), 1));
	else if (blc->map[1] != ~0)
		return (blc->mem + max * (least_unset_bit(&(blc->map[1]), 1) + H));
	return (handle_alloc(blc->next, max, pid));
}

static void				*handle_large_alloc(t_block *block, size_t size)
{
	size_t	alloc;

	if (!block)
		return (0);
	if (block->mem)
		return (handle_large_alloc(block->next, size));
	else
	{
		alloc = (((size - 1) / g_ftmd.block_size) + 1) * g_ftmd.block_size;
		block->mem = mmap(0, alloc, RW, MAP_ANON | MAP_PRIVATE, -1, 0);
		if (block->mem == (void*)-1)
		{
			block->mem = 0;
			return (0);
		}
		block->map[0] = size;
		block->map[1] = alloc;
		block->next = ft_bzero(malloc(sizeof(t_block)), sizeof(t_block));
		g_ftmd.parts[2].first_free = block->next;
		return (block->mem);
	}
}

void					*malloc(size_t size)
{
	void	*i;

	ft_malloc_init();
	pthread_mutex_lock(&g_ftmd.lock);
	if (size <= g_ftmd.parts[0].max_size)
		i = (handle_alloc(g_ftmd.parts[0].first_free,
					g_ftmd.parts[0].max_size, 0));
	else if (size <= g_ftmd.parts[1].max_size)
		i = (handle_alloc(g_ftmd.parts[1].first_free,
					g_ftmd.parts[1].max_size, 1));
	else
		i = (handle_large_alloc(g_ftmd.parts[2].first_free, size));
	pthread_mutex_unlock(&g_ftmd.lock);
	return (i);
}
