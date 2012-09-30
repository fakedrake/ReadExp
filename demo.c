#include <stdio.h>
#include <string.h>
#include "readexp.h"

int main(int argc, char** argv)
{
    int i;
    struct symbol s;

    if (argc < 2) {
	printf ("Expected at least one argument...\n");
	return 0;
    }

    for (i=0; i <= strlen(argv[1]); i++) {
	s = getOp(argv[1][i]);
	if (s.type == NUMBER)
	    printf ("Number: %d\n", s.val);
	if (s.type == OPERATOR)
	    printf ("Operator: '%c'\n", s.cval);
    }
    return 0;
}
