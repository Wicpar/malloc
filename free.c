/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnieto <fnieto@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/15 17:34:57 by fnieto            #+#    #+#             */
/*   Updated: 2017/02/18 19:29:33 by fnieto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "util.h"
#include "malloc.h"

static t_block	*is_first(t_block *a, t_block *b)
{
	t_block	*tmpa;
	t_block	*tmpb;

	tmpa = a;
	tmpb = b;
	while (a || b)
	{
		if (tmpa == b)
			return (a);
		if (tmpb == a)
			return (b);
		tmpa = tmpa ? tmpa->next : 0;
		tmpb = tmpb ? tmpb->next : 0;
	}
	return (0);
}

void			free_block(void *ptr, t_block *blc, t_partition *part, int pid)
{
	t_ulong		pos;
	void		*mem;
	t_block		*tmp;

	pos = (ptr - blc->mem) / part->max_size;
	if (pid < 2 && (part->first_free = is_first(part->first_free, blc)))
		blc->map[pos / H] &= ~(1L << (pos % H));
	if (pid == 0 && blc->map[0] < 2 && !blc->map[1])
	{
		mem = blc->mem;
		tmp = blc->next;
		*(blc) = *(blc->next);
		munmap(mem, part->max_size * 128);
	}
	else if (pid == 2 || (pid == 1 && (!blc->map[0] && !blc->map[1])))
	{
		munmap(blc->mem, pid != 2 ? part->max_size * 128 : blc->map[1]);
		tmp = blc->next;
		*(blc) = *(blc->next);
		free(tmp);
	}
	if (part->first_free == tmp)
		part->first_free = blc;
}

void			free(void *ptr)
{
	t_block		*bc[3];
	int			i;

	if (!ptr)
		return ;
	ft_malloc_init();
	bc[0] = &g_ftmd.parts[0].origin;
	bc[1] = &g_ftmd.parts[1].origin;
	bc[2] = &g_ftmd.parts[2].origin;
	pthread_mutex_lock(&g_ftmd.lock);
	while (bc[0] || bc[1] || bc[2])
	{
		i = -1;
		while (++i < 3)
			if (bc[i] && (ptr - bc[i]->mem) < g_ftmd.memsize[i])
			{
				(free_block(ptr, bc[i], &(g_ftmd.parts[i]), i));
				pthread_mutex_unlock(&g_ftmd.lock);
				return ;
			}
		bc[0] = bc[0] ? bc[0]->next : 0;
		bc[1] = bc[1] ? bc[1]->next : 0;
		bc[2] = bc[2] ? bc[2]->next : 0;
	}
	pthread_mutex_unlock(&g_ftmd.lock);
}

void			vfree(void *ptr)
{
	free(ptr);
}
