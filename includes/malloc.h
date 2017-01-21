/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnieto <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/21 17:34:44 by fnieto            #+#    #+#             */
/*   Updated: 2017/01/21 17:38:52 by fnieto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_H
# define MALLOC_H
# include <stdlib.h>

void	free(void *ptr);
void	*malloc(size_t size);
void	*realloc(void *ptr, size_t size);

#endif
