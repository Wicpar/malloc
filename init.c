/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnieto <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/15 17:35:30 by fnieto            #+#    #+#             */
/*   Updated: 2017/02/16 16:01:41 by fnieto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "util.h"
#include <sys/resource.h>
#include <unistd.h>

static pthread_once_t	g_ft_malloc_once_lock = PTHREAD_ONCE_INIT;

static void			once(void)
{
	pthread_mutexattr_t attr;

	ft_bzero(&g_ftmd, sizeof(g_ftmd));
	g_ftmd.block_size = getpagesize();
	g_ftmd.parts[0].name = "TINY";
	g_ftmd.parts[0].name_size = sizeof("TINY");
	g_ftmd.parts[0].max_size = TNMEM(g_ftmd.block_size);
	g_ftmd.parts[0].first_free = &(g_ftmd.parts[0].origin);
	g_ftmd.parts[1].name = "SMALL";
	g_ftmd.parts[1].name_size = sizeof("SMALL");
	g_ftmd.parts[1].max_size = SMMEM(g_ftmd.block_size);
	g_ftmd.parts[1].first_free = &(g_ftmd.parts[1].origin);
	g_ftmd.parts[2].name = "LARGE";
	g_ftmd.parts[2].name_size = sizeof("LARGE");
	g_ftmd.parts[2].max_size = -1;
	g_ftmd.parts[2].first_free = &(g_ftmd.parts[2].origin);
	g_ftmd.memsize[0] = TNFULL(g_ftmd.block_size);
	g_ftmd.memsize[1] = SMFULL(g_ftmd.block_size);
	g_ftmd.memsize[2] = g_ftmd.block_size;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&g_ftmd.lock, &attr);
}

void				ft_malloc_init(void)
{
	pthread_once(&g_ft_malloc_once_lock, &once);
}
