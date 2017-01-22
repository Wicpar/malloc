/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rlimit.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnieto <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/22 14:36:30 by fnieto            #+#    #+#             */
/*   Updated: 2017/01/22 14:53:43 by fnieto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/time.h> 
#include <sys/resource.h>
#include <stdio.h>

void	test(void)
{
	struct rlimit lim;

	getrlimit(RLIMIT_AS, &lim);
	printf("soft: %llu\n", lim.rlim_cur);
	printf("hard: %llu\n", lim.rlim_max);
}
