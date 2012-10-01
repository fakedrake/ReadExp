#ifndef _STACK_EVAL_H_
#define _STACK_EVAL_H_

#define STACK_SIZE 50

/* Errors */
extern char error_unsupported_operator[];
extern char error_float_umsupported[];
extern char error_stack_overflow[];
extern char error_stack_underflow[];

struct expression {
    int stack[STACK_SIZE];
    int head;
    char* error;
};

/* Providing all the functions. */
int push (int, struct expression*);
int pop (struct expression*);
int eval(char, struct expression *);
int push_operator (char, struct expression*);

#endif /* _STACK-EVAL_H_ */
