/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnieto <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/21 17:26:28 by fnieto            #+#    #+#             */
/*   Updated: 2017/02/11 21:19:34 by fnieto           ###   ########.fr       */
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

void			*ft_memcpy(void *dest, const void *src, size_t n)
{
	void	*tmp;
	int		n64;

	tmp = dest;
	n64 = n / sizeof(t_ulong);
	n -= n64;
	while (--n64 >= 0)
	{
		*((t_ulong*)tmp) = *((t_ulong*)src);
		tmp += sizeof(t_ulong);
		src += sizeof(t_ulong);
	}
	while (--n < -1)
		*(char*)tmp++ = *(char*)src++;
	return dest;
}



void			*realloc_block(t_block *blc, void *ptr, size_t size, int part)
{
	void	*ret;
	t_ulong align;

	if (size < g_data.parts[part].max_size)
		return (ptr);
	ret = malloc(size);
	if (!ret)
		return (0);
	align = (t_ulong)(ptr - ) / g_data.parts[part].max_size;
	blc->map[align / H] &= ~(1 << align);
	ft_memcpy(ret, ptr, g_data.parts[part].max_size);
	return (ret);
}

void			*realloc(void *ptr, size_t size)
{
	t_ulong		hs;
	t_ulong		align[4];
	t_block		*bc[3];
	int			i;
	void		*ret;

	if (!g_data.block_size)
		return (0);
	align[2] = (t_ulong)ptr;
	align[0] = (t_ulong)ptr & (~(g_data.block_size * 2 - 1));
	hs = align[0] * FVN;
	bc[0] = ((hs & g_data.parts[0].filter) == hs) ? &g_data.parts[0].origin : 0;
	align[1] = (t_ulong)ptr & (~(g_data.block_size * 2 * H  - 1));
	hs = align[1] * FVN;
	bc[1] = ((hs & g_data.parts[1].filter) == hs) ? &g_data.parts[1].origin : 0;
	hs = (t_ulong)ptr * FVN;
	bc[2] = ((hs & g_data.parts[2].filter) == hs) ? &g_data.parts[2].origin : 0;
	while (1)
	{
		i = -1;
		while (++i < 3)
			if (bc[i] && ptr > bc[i]->mem && ptr < bc[i]->mem +
					g_data.parts[i].max_size * F)
				{
					if (size < g_data.parts[i].max_size)
						return (ptr);
					ret = malloc(size);
					if (!ret)
						return (0);
					align[3] = (t_ulong)(ptr - align[i]) /
						g_data.parts[i].max_size;
					bc[i]->map[align[3] / H] &= ~(1 << align[3]);
					ft_memcpy(ret, ptr, g_data.parts[i].max_size);
					return (ret);
				}
		if (!(bc[0] || bc[1] || bc[2]))
			break;
	}
	return (0);
}

t_data			init(void)
{
	t_data			data;
	struct rlimit	lim;
	size_t			size;

	data.block_size = getpagesize();
	getrlimit(RLIMIT_AS, &lim);
	data.max_blocks = lim.rlim_cur / data.block_size;
	size = data.block_size / H;
	data.parts[0].name = "TINY";
	data.parts[0].max_size = size;
	data.parts[1].name = "SMALL";
	data.parts[1].max_size = size * Q;
	data.parts[2].name = "LARGE";
	data.parts[2].max_size = -1;
	return (data);
}
