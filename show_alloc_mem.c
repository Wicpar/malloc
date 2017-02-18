/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnieto <fnieto@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/15 17:35:54 by fnieto            #+#    #+#             */
/*   Updated: 2017/02/18 20:03:22 by fnieto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "util.h"
#include "malloc.h"
#include <unistd.h>

void				ft_putnbrhex(size_t nbr)
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

void				ft_putnbr(size_t nbr)
{
	char bit;

	if (!nbr)
		return ;
	bit = nbr % 10 + '0';
	ft_putnbr(nbr / 10);
	write(1, &(bit), 1);
}

static void			ft_putocc(t_ulong digit)
{
	char bit;

	bit = digit ? '*' : '.';
	write(1, &bit, 1);
}

static void			ft_putmap(t_ulong map[2])
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

static void			ft_putmap_large(t_ulong map[2])
{
	write(1, "asked: ", 7);
	ft_putnbr(map[0]);
	write(1, "\n                     ", 22);
	write(1, "given: ", 17);
	ft_putnbr(map[1]);
	write(1, "\n", 1);
}

void				show_alloc_mem(void)
{
	int		i;
	t_block	*tmp;

	ft_malloc_init();
	pthread_mutex_lock(&g_ftmd.lock + (i = -1) + 1);
	while (++i < 3)
	{
		write(1, g_ftmd.parts[i].name, g_ftmd.parts[i].name_size);
		write(1, ": ", 2);
		ft_putnbr(g_ftmd.parts[i].max_size);
		write(1, "\n", 1);
		tmp = &(g_ftmd.parts[i].origin);
		while (tmp && tmp->mem)
		{
			ft_putnbrhex((size_t)tmp->mem);
			write(1, ": ", 2);
			if (i != 2)
				ft_putmap(tmp->map);
			else
				ft_putmap_large(tmp->map);
			write(1, "\n\n", 1 + (!(tmp->next && tmp->next->mem)));
			tmp = tmp->next;
		}
	}
	pthread_mutex_unlock(&g_ftmd.lock);
}
