/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnieto <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/21 18:07:20 by fnieto            #+#    #+#             */
/*   Updated: 2017/01/21 18:45:16 by fnieto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTIL_H
# define UTIL_H
# include <string.h>

typedef unsigned long	ulong;

typedef struct			block_s
{
	void			*mem;
	ulong			map[2];
	struct block_s	*next;
}						block_t;

typedef struct			malloc_partition_s
{
	char	*name;
	size_t	block_size_byte;
	size_t	loc_size_byte;
}						malloc_partition_t;

#endif
