/*
 * NEED TO REPLACE 2 TASKS BETWEEN THEM
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int edit(const char *fm, const char *new, size_t line);

#define BUFLEN	 1024

#define getb(l, r, b) {																	\
	r = fopen(filename, "r");															\
	for (size_t i = 1; i++ <= l && fgets(b, BUFLEN, r););	\
	fclose(r);																						\
}

__attribute__((cold))
int 
replace(const char *filename, size_t l1, size_t l2) 
{
	FILE *r;
	char *b1;
	char *b2;

	if (l1 == l2)
		return 1;

	if (!fopen(filename, "r"))
		{puts("Choose the usable task list"); return 0;}

	/* MALLOC */
	b1 = malloc(BUFLEN);
	b2 = malloc(BUFLEN);

	/* GETTING b1 AND b2 */
	getb(l1, r, b1);
	getb(l2, r, b2);

	/* MOVE */
	edit(filename, b2, l1);
	edit(filename, b1, l2);

	free(b1);
	free(b2);
	return 1;
}
