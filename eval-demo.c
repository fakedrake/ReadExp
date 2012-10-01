#include <string.h>
#include <stdio.h>

#include "stack-eval.h"

int main(int argc, char *argv[])
{
    struct expression exp;
    memset(&exp, 0, sizeof(struct expression));

    int i, status;
    for (i=0; i<strlen(argv[1]); i++)
	if ((status = eval(argv[1][i], &exp)) < 0) {
	    printf ("Error on char %d: %s\n", i+1, exp.error);
	    return 2;
	}

    if (!status) {
	printf ("No expression provided apparently\n");
	return 1;
    }

    printf ("The result is: %d\n", pop(&exp));
    return 0;
}
