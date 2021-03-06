/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnieto <fnieto@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/21 17:34:44 by fnieto            #+#    #+#             */
/*   Updated: 2017/02/18 19:31:59 by fnieto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_H
# define MALLOC_H
# include <string.h>

void	free(void *ptr);
void	vfree(void *ptr);
void	*malloc(size_t size);
void	*valloc(size_t size);
void	*calloc(size_t nelem, size_t elsize);
void	*realloc(void *ptr, size_t size);
void	*reallocf(void *ptr, size_t size);
void	show_alloc_mem(void);

#endif
