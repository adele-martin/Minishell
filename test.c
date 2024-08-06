#include <stdio.h>

int main()
{
	char *test = "PA=THasdfsdtherfgwnr2lkn48384uf=dev/intdsfasdf/";

	while (*test != '=')
		test++;
	test++;
	printf("%s\n", test);
	return 0;
}
