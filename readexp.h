#ifndef _READEXP_H_
#define _READEXP_H_

struct symbol {
    unsigned type;
    char cval;
    int ival;
    float fval;
};


/* Symbol types */
#define INVALID 0x0
#define NUMBER 0x1
#define OPERATOR 0x2
#define FLOAT 0x4

/* Does the same as getOp only a symbol struct may contain both a
 * nuber and an operator. */
struct symbol burstOp(char);

/* Use a ring buffer to store the items in order to emit them one by
 * one. There is a stable one character delay after the first number. */
struct symbol getOp(char);



#endif /* _READEXP_H_ */
