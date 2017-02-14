/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnieto <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/21 18:07:20 by fnieto            #+#    #+#             */
/*   Updated: 2017/02/14 20:26:44 by fnieto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTIL_H
# define UTIL_H
# include <string.h>
# include <sys/mman.h>

# define RW (PROT_READ | PROT_WRITE)
# define FVN (14695981039346656037UL)
# define F (128)
# define H (F / 2)
# define TNBLC (2)
# define SMBLC (16)
# define TNMASK(BLCK) (~((TNBLC * BLCK) - 1))
# define SMMASK(BLCK) (~((SMBLC * BLCK) - 1))
# define TNMEM(BLCK) ((TNBLC * (BLCK)) / F)
# define SMMEM(BLCK) ((SMBLC * (BLCK)) / F)


typedef unsigned long	t_ulong;
typedef unsigned int	t_uint;
typedef unsigned short	t_ushort;
typedef unsigned char	t_ubyte;

typedef struct			s_block
{
	void			*mem;
	t_ulong			map[2];
	struct s_block	*next;
	struct s_block	*prev;
}						t_block;

typedef struct			s_partition
{
	char	*name;
	int		name_size;
	size_t	max_size;
	t_block	origin;
}						t_partition;

typedef struct			s_data
{
	t_partition	parts[3];
	size_t		block_size;
	t_ulong		masks[3];
	size_t		max_blocks;
}						t_data;

extern t_data			g_data;

t_data					init(void);

#endif
