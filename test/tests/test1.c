#include "../includes/malloc.h"

int test()
{
	int		i;
	char	*addr;

	i = 0;
	while (i < 1024)
	{
		addr = malloc(1024);
		addr[0] = 42;
		i++;
	}
	return (0);
}
