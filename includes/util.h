/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnieto <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/21 18:07:20 by fnieto            #+#    #+#             */
/*   Updated: 2017/01/22 22:53:57 by fnieto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTIL_H
# define UTIL_H
# include <string.h>
# include <sys/mman.h>

# define RW (PROT_READ | PROT_WRITE)

typedef unsigned long	t_ulong;
typedef unsigned int	t_uint;
typedef unsigned short	t_ushort;
typedef unsigned char	t_ubyte;

typedef struct			s_block
{
	void			*mem;
	t_ulong			map[2];
	struct s_block	*next;
}						t_block;

typedef struct			s_partition
{
	char	*name;
	size_t	max_size;
	t_block	origin;
}						t_partition;

typedef struct			s_data
{
	t_partition	partitions[3];
	size_t		block_size;
	size_t		max_blocks;
	size_t		allocated_blocks;
}						t_data;

t_data					init(void);

#endif
