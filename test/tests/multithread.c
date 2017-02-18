/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multithread.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnieto <fnieto@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/18 20:21:52 by fnieto            #+#    #+#             */
/*   Updated: 2017/02/18 20:33:42 by fnieto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include "../includes/malloc.h"
#define THREAD_NUM 10
#define M (1024 * 1024)

pthread_mutex_t	out_lock;
bool			multithreaded;

void	print(char *s)
{
	write(1, s, strlen(s));
}

void	*the_test()
{
	char	*addr1;
	char	*addr2;
	char	*addr3;

	addr1 = malloc(16 * M);
	strcpy(addr1, "Bonjours\n");
	print(addr1);
	addr2 = malloc(16 * M);
	sleep(1);
	addr3 = realloc(addr1, 128 * M);
	addr3[127 * M] = 42;
	print(addr3);
	free(addr3);
	free(addr2);
	return (0);

}

void	test()
{
	int			i;
	pthread_t	thread[THREAD_NUM];

	pthread_mutex_init(&out_lock, NULL);
	printf("Malloc test: \n");
	printf("Single threaded:\n");
	multithreaded = false;
	the_test();

	printf("Multi threaded:\n");
	multithreaded = true;
	i = -1;
	while (++i < THREAD_NUM) {
		pthread_create(&thread[i], NULL, the_test, NULL);
	}
	i = -1;
	while (++i < THREAD_NUM) {
		pthread_join(thread[i], NULL);
	}
	printf("All tests passed!\n");
}