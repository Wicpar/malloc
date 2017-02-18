/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnieto <fnieto@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/15 17:35:07 by fnieto            #+#    #+#             */
/*   Updated: 2017/02/18 18:53:03 by fnieto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "util.h"
#include "malloc.h"

static void		*ft_memcpy(void *dest, const void *src, size_t n)
{
	void *tmp;

	tmp = dest;
	while (n--)
		*((char*)tmp++) = *((char*)src++);
	return (dest);
}

static void		*realloc_lblock(t_block *blc, void *ptr, size_t size)
{
	void	*ret;

	if (size < blc->map[1] && size > g_ftmd.parts[1].max_size)
		return (ptr);
	ret = malloc(size);
	if (!ret)
		return (0);
	ft_memcpy(ret, ptr, MIN(blc->map[1], size));
	free_block(ptr, blc, &(g_ftmd.parts[2]), 2);
	pthread_mutex_unlock(&g_ftmd.lock);
	return (ret);
}

static void		*realloc_block(t_block *blc, void *ptr, size_t size, int part)
{
	void	*ret;

	if (size < g_ftmd.parts[part].max_size &&
		(!part || (size > g_ftmd.parts[0].max_size)))
		return (ptr);
	ret = malloc(size);
	if (!ret)
		return (0);
	ft_memcpy(ret, ptr, MIN(g_ftmd.parts[part].max_size, size));
	free_block(ptr, blc, &(g_ftmd.parts[part]), part);
	pthread_mutex_unlock(&g_ftmd.lock);
	return (ret);
}

void			*realloc(void *ptr, size_t size)
{
	t_block		*bc[3];
	long		i;
	void		*ret;

	if (!ptr)
		return (malloc(size));
	ft_malloc_init();
	bc[0] = &g_ftmd.parts[0].origin;
	bc[1] = &g_ftmd.parts[1].origin;
	bc[2] = &g_ftmd.parts[2].origin;
	pthread_mutex_lock(&g_ftmd.lock);
	while (bc[0] || bc[1] || bc[2])
	{
		i = -1;
		while (++i < 3)
			if (bc[i] && i == 2 && (ptr - bc[i]->mem) < bc[i]->map[1])
				return (realloc_lblock(bc[i], ptr, size));
			else if (bc[i] && i < 2 && (ptr - bc[i]->mem) < g_ftmd.memsize[i])
				return (realloc_block(bc[i], ptr, size, i));
		bc[0] = bc[0] ? bc[0]->next : 0;
		bc[1] = bc[1] ? bc[1]->next : 0;
		bc[2] = bc[2] ? bc[2]->next : 0;
	}
	pthread_mutex_unlock(&g_ftmd.lock);
	return (0);
}

void			*reallocf(void *ptr, size_t size)
{
	if (size)
		return (realloc(ptr, size));
	free(ptr);
	return (0);
}
