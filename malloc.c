/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnieto <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/21 17:26:28 by fnieto            #+#    #+#             */
/*   Updated: 2017/02/14 18:50:39 by fnieto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "util.h"
#include <sys/resource.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <errno.h>

t_data g_data;

void			ft_putnbrhex(size_t nbr)
{
	const char	*map = "0123456789abcdef";
	char		hex[19];
	char		bit;
	int			i;

	i = 16;
	hex[4] = ' ';
	hex[9] = ' ';
	hex[14] = ' ';
	while (--i > -1)
	{
		hex[i + i / 4] = map[nbr % 16];
		nbr /= 16;
	}
	write(1, hex, 19);
}

void			ft_putnbr(size_t nbr)
{
	char bit;

	if (!nbr)
		return ;
	bit = nbr % 10 + '0';
	ft_putnbr(nbr / 10);
	write(1, &(bit), 1);
}

void			ft_putocc(t_ulong digit)
{
	char bit;

	bit = digit ? '*' : '.';
	write(1, &bit, 1);
}

void			ft_putmap(t_ulong map[2])
{
	int		i;

	i = -1;
	while (++i < 64)
		ft_putocc(map[0] & (1L << i));
	write(1, "\n                     ", 22);
	i = -1;
	while (++i < 64)
		ft_putocc(map[1] & (1L << i));
}

void			show_alloc_mem(void)
{
	int		i;
	t_block	*tmp;


	if (!g_data.block_size)
		g_data = init();
	i = -1;
	while (++i < 3)
	{
		write(1, g_data.parts[i].name, g_data.parts[i].name_size);
		write(1, ": ", 2);
		ft_putnbr(g_data.parts[i].max_size);
		write(1, "\n", 1);
		tmp = &(g_data.parts[i].origin);
		while (tmp && tmp->mem)
		{
			ft_putnbrhex((size_t)tmp->mem);
			if (i != 2)
			{
				write(1, ": ", 2);
				ft_putmap(tmp->map);
			}
			write(1, "\n", 1);
			tmp = tmp->next;
		}
		write(1, "\n", 1);
	}
}

void			*memalloc(size_t size)
{
	void	*mem;
	long	l;
	long	o;
	void	*tmp;

	mem = malloc(size);
	if (!mem)
		return (0);
	tmp = mem - sizeof(t_ulong);
	l = size / sizeof(t_ulong);
	o = size % sizeof(t_ulong);
	while (--l >= 0)
		*((long*)(tmp += sizeof(t_ulong))) = 0;
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

void			*handle_alloc(t_block *blc, size_t max, int pid)
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
		blc->next = memalloc(sizeof(t_block));
		blc->next->prev = blc;
	}
	if (blc->map[0] != ~0)
		return (blc->mem + max * least_unset_bit(blc->map, 1));
	else if (blc->map[1] != ~0)
		return (blc->mem + max * (least_unset_bit(&(blc->map[1]), 1) + H));
	return (handle_alloc(blc->next, max, pid));
}

void			*handle_large_alloc(t_block *block, size_t size)
{
	size_t	alloc;

	if (!block)
		return (0);
	if (block->mem)
	{
		return (handle_large_alloc(block, size));
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
		block->map[0] = size;
		block->map[1] = alloc;
		block->next = memalloc(sizeof(t_block));
		block->next->prev = block;
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
		return (handle_alloc(&g_data.parts[0].origin,
					g_data.parts[0].max_size, 0));
	else if (size <= g_data.parts[1].max_size)
		return (handle_alloc(&g_data.parts[1].origin,
					g_data.parts[1].max_size, 1));
	else
		return (handle_large_alloc(&g_data.parts[2].origin,
					g_data.parts[2].max_size));
}

void			clean_origin(t_block *blc, int pid)
{
	free(blc->next);
	blc->next = 0;
	blc->mem = 0;
}

void			clean_allocated(t_block *blc, int pid)
{
	blc->prev->next = blc->next;
	free(blc);
}

void			free_block(void *ptr, t_block *blc, t_partition *part, int pid)
{
	t_ulong		pos;
	void		*mem;

	pos = (ptr - blc->mem) / part->max_size;
	if (pid < 2)
		blc->map[pos / H] &= ~(1L << (pos % H));
	if (pid == 0 && blc->map[0] < 2 && !blc->map[1])
	{
		mem = blc->mem;
		blc->mem = 0;
		if (blc->prev)
			*(blc->prev->next) = *(blc->next);
		else
			*blc = *(blc->next);
		munmap(mem, part->max_size * 128);
	}
	else if (pid == 2 || (!blc->map[0] && !blc->map[1]))
	{
		mem = blc->mem;
		if (blc->prev)
			clean_allocated(blc, pid);
		else
			clean_origin(blc, pid);
		munmap(mem, pid != 2 ? part->max_size * 128 : blc->map[1]);
	}
}

void			free(void *ptr)
{
	t_ulong		hs;
	t_block		*bc[3];
	int			i;

	if (!g_data.block_size)
		return ;
	bc[0] = &g_data.parts[0].origin;
	bc[1] = &g_data.parts[1].origin;
	bc[2] = &g_data.parts[2].origin;
	while (bc[0] || bc[1] || bc[2])
	{
		i = -1;
		while (++i < 3)
			if (bc[i] && (ptr - bc[i]->mem) < ((~g_data.masks[i]) + 1))
				return free_block(ptr, bc[i], &g_data.parts[i], i);
		bc[0] = bc[0] ? bc[0]->next : 0;
		bc[1] = bc[1] ? bc[1]->next : 0;
		bc[2] = bc[2] ? bc[2]->next : 0;
	}
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

	if (size < g_data.parts[part].max_size)
		return (ptr);
	ret = malloc(size);
	if (!ret)
		return (0);
	ft_memcpy(ret, ptr, g_data.parts[part].max_size);
	free_block(ptr, blc, &(g_data.parts[part]), part);
	return (ret);
}

void			*realloc(void *ptr, size_t size)
{
	t_ulong		hs;
	t_block		*bc[3];
	int			i;

	if (!g_data.block_size)
		return (0);
	if(!ptr)
		return malloc(size);
	bc[0] = &g_data.parts[0].origin;
	bc[1] = &g_data.parts[1].origin;
	bc[2] = &g_data.parts[2].origin;
	while (bc[0] || bc[1] || bc[2])
	{
		i = -1;
		while (++i < 3)
			if (bc[i] && (ptr - bc[i]->mem) < ((~g_data.masks[i]) + 1))
				return realloc_block(bc[i], ptr, size, i);
		bc[0] = bc[0] ? bc[0]->next : 0;
		bc[1] = bc[1] ? bc[1]->next : 0;
		bc[2] = bc[2] ? bc[2]->next : 0;
	}
	return (0);
}

t_data			init(void)
{
	t_data			data;
	struct rlimit	lim;

	data.block_size = getpagesize();
	getrlimit(RLIMIT_AS, &lim);
	data.max_blocks = lim.rlim_cur / data.block_size;
	data.parts[0].name = "TINY";
	data.parts[0].name_size = sizeof("TINY");
	data.parts[0].max_size = TNMEM(data.block_size);
	data.parts[1].name = "SMALL";
	data.parts[1].name_size = sizeof("SMALL");
	data.parts[1].max_size = SMMEM(data.block_size);
	data.parts[2].name = "LARGE";
	data.parts[2].name_size = sizeof("LARGE");
	data.parts[2].max_size = -1;
	data.masks[0] = TNMASK(data.block_size);
	data.masks[1] = SMMASK(data.block_size);
	data.masks[2] = -1;
	return (data);
}
