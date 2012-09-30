#include <string.h>
#include "readexp.h"

/* Abs for security reasons. */
#define AB(x) (((x)>0)?(x):0)
#define AMIN(x,y) (AB(x)>AB(y)?AB(y):AB(x))

/* type is non-nil only when last_read changes value. */
struct number_reader {
    int value, ready, active, base, fp;
};

struct op_reader {
    char value;
    int ready;
};


/* Return digit value or negative if not digit. Only decimal is
 * supported for now. This also fixes the base of the status iff it is
 * unset. There is no guarantee that this is called only once so the
 * code here has to be consistant. */
int digit(char c, struct number_reader* n)
{
    if ( n ) {
	if (n->base == 0) n->base = 10;

	if (c == '#')
	    if (n->value > 0) {
		n->base = n->value;
		n->value = 0;
		return 0;
	    } else {
		return -1;
	    }
    }

    if (c <= '0' + AMIN(n->base, 9) && c >= '0') return c - (int)'0';
    else if (c >= 'a' && c < 'a' + AMIN('z'-'a',n->base-9) + 1) return c - 'a' + 10;
    return -1;
}

/* Since we do not know the base we will assume the caracter is an
 * operator. */
void read_operator (char c, struct op_reader* op)
{
    if (c !=' ') {
	op->value = c;
	op->ready = 1;
    }
}

/* Return 0 on failure. */
int push_digit (char c, struct number_reader* num)
{
    int dig = digit(c, num);
    if (!num->active)
	num->value = 0;

    if (dig < 0) {
	if (c == '.') {
	    num->fp = 1;
	    return 1;
	}

	if (num->active)
	    num->ready = 1;
	num->active = 0;
	return 0;
    }

    if (num->fp) num->fp++;
    num->value = num->value*num->base + dig;
    num->active = 1;
    return 1;
}

int parse (char c, struct number_reader* num_rd, struct op_reader* op_rd )
{
    num_rd->ready = op_rd->ready = 0;
    if ( !push_digit(c, num_rd) )
	read_operator(c, op_rd);

    return num_rd->ready || op_rd->ready;
}

int ipow(b,p)
{
    int i, ret = 1;
    for (i=0;i<p;i++)
	ret *= b;
    return ret;
}

/* Use a ring buffer to store the items in order to emit them one by
 * one. There is a stable one character delay after the first number. */
struct symbol getOp(char c)
{
    static struct number_reader n = {0,0,0,0};
    static struct op_reader o = {0,0};
    static struct symbol ret[2];
    static int items = 0, start = 0;
    parse(c, &n, &o);
    if (n.ready) {
	if(n.fp) {
	    ret[ (start+items)%2 ].fval = (float)n.value / (float)ipow(n.base, n.fp-1);
	    ret[ (start+items)%2 ].type = FLOAT;
	} else {
	    ret[ (start+items)%2 ].ival = n.value;
	    ret[ (start+items)%2 ].type = NUMBER;
	}
	items++;
    }

    if (o.ready) {
	ret[ (start+items)%2 ].cval = o.value;
	ret[ (start+items)%2 ].type = OPERATOR;
	items++;
    }

    /* If there are items to emit */
    start %= 2;
    if (items) {
	items--;
	return ret[start++];
    }
    memset(ret, 0, sizeof(struct symbol));
    return *ret;
}

/* Does the same as getOp only a symbol struct may contayn both a
 * nuber and an operator. */
struct symbol burstOp(char c)
{
    static struct number_reader n = {0,0,0,0};
    static struct op_reader o = {0,0};
    struct symbol ret = {0,0,0};
    int items = 0, start = 0;
    parse(c, &n, &o);
    if (n.ready) {
	if(n.fp) {
	    ret.fval = (float)n.value / (float)ipow(n.base, n.fp-1);
	    ret.type = FLOAT;
	} else {
	    ret.ival = n.value;
	    ret.type |= NUMBER;
	    items++;
	}
    }

    if (o.ready) {
	ret.cval = o.value;
	ret.type |= OPERATOR;
	items++;
    }
    return ret;
}
