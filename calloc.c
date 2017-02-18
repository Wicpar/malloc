/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnieto <fnieto@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/18 14:37:45 by fnieto            #+#    #+#             */
/*   Updated: 2017/02/18 19:03:23 by fnieto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "util.h"
#include "malloc.h"

void				*calloc(size_t nelem, size_t elsize)
{
	size_t	size;
	void	*ptr;

	size = nelem * elsize;
	ptr = malloc(size);
	ft_bzero(ptr, size);
	return (ptr);
}

void				*valloc(size_t size)
{
	return (malloc(MAX(size, 4096)));
}
