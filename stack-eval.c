/* This file actually evaluates the expression it is fed. This is the
 * stack based. All functions that accept a pointer to en expression are able to produce
 * errors. They will return 0 if the exp is not ready, -1 if there is an
 * error or the size of the stack in exp. */
#include "readexp.h"
#include "stack-eval.h"

/* Errors */
char error_unsupported_operator[] = "Operator unsupported";
char error_float_umsupported[] = "Floating point operations not supported.";
char error_stack_overflow[] = "Stack overflow (too long expression).";
char error_stack_underflow[] = "Stack underflow (too many operators).";


int push (int v, struct expression* e)
{
    if (e->error)  return -1;
    if (e->head >= STACK_SIZE) {
	e->error = error_stack_overflow;
	return -1;
    }
    e->stack[e->head++] = v;
    return e->head;
}


/* This is an exception. Returns the poped number. */
int pop (struct expression* e)
{
    if (e->error)  return -1;
    if (e->head < 0){
	e->error = error_stack_underflow;
	return -1;
    }

    return e->stack[--e->head];
}

/* This tries to evaluate the expression so far.  We may wnat to add
 * floating point support since it is implemented by the parser. */
int eval(char c, struct expression *exp)
{
    struct symbol s = getOp(c);
    if (!VALID(s)) {
	return 0;
    }

    if ( s.type & FLOAT ) {
	exp->error = error_float_umsupported;
	return -1;
    }

    if (s.type & NUMBER) {

	push(s.ival, exp);
	if (exp->error)  return -1;

	return exp->head;
    }

    /* We have an operator apperently */
    return push_operator(s.cval, exp);
}

int push_operator (char op, struct expression* exp)
{
    int i1, i2;
    if (exp->error) return -1;
    switch (op) {
    case '+':
	push(pop(exp)+pop(exp), exp);
	if (exp->error) return -1;
	break;
    case '-':
	push(-pop(exp)+pop(exp), exp);
	if (exp->error) return -1;
	break;
    case '*':
	push(pop(exp)*pop(exp), exp);
	if (exp->error) return -1;
	break;
    case '/':
	i1 = pop(exp);
	i2 = pop(exp);
	push(i2/i1, exp);
	if (exp->error) return -1;
	break;
    default:
	exp->error = error_unsupported_operator;
	return -1;
    }
    return exp->head;
}
