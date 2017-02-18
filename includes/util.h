/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnieto <fnieto@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/21 18:07:20 by fnieto            #+#    #+#             */
/*   Updated: 2017/02/18 17:38:25 by fnieto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTIL_H
# define UTIL_H
# include <string.h>
# include <sys/mman.h>
# include <pthread.h>

# define RW (PROT_READ | PROT_WRITE)
# define F (128)
# define H (F / 2)
# define TNBLC (2)
# define SMBLC (16)
# define TNFULL(BLCK) (TNBLC * BLCK)
# define SMFULL(BLCK) (SMBLC * BLCK)
# define TNMEM(BLCK) ((TNBLC * (BLCK)) / F)
# define SMMEM(BLCK) ((SMBLC * (BLCK)) / F)
# define MIN(A,B) ((A) < (B) ? (A) : (B))
# define MAX(A,B) ((A) > (B) ? (A) : (B))

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
	int		name_size;
	size_t	max_size;
	t_block	origin;
	t_block	*first_free;
}						t_partition;

typedef struct			s_data
{
	t_partition		parts[3];
	size_t			block_size;
	t_ulong			memsize[3];
	pthread_mutex_t	lock;
}						t_data;

extern t_data			g_ftmd;

void					ft_malloc_init(void);
void					free_block
						(void *ptr, t_block *blc, t_partition *part, int pid);
void					ft_putnbr(size_t nbr);
void					ft_putnbrhex(size_t nbr);
void					*ft_bzero(void *mem, size_t size);

#endif
