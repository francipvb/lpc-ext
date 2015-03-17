# define LPC_TYPE_CLASS		7
# define LPC_TYPE_LVALUE	10
# define LPC_TYPE_REF(t)	((t) >> 4)

typedef int64_t LPCInt;

typedef struct {
    uint32_t high;		/* 1 sign, 15 exponent, ... */
    uint64_t low;		/* ... 80 mantissa */
} LPCFloat;

typedef uint16_t LPCInherit;

# define THIS			0xffff

typedef struct {
    LPCInherit inherit;		/* program */
    uint16_t index;		/* index in string constant table */
} LPCStringConst;

typedef struct {
    uint8_t type;		/* compile-time type */
    LPCInherit inherit;		/* optional class string program */
    uint16_t index;		/* optional class string index */
} LPCType;

typedef uint8_t LPCLocal;

typedef struct {
    LPCInherit inherit;		/* program */
    uint8_t index;		/* index in variable table */
} LPCGlobal;

typedef struct {
    uint16_t func;		/* index in kfun table */
    uint8_t nargs;		/* # arguments */
} LPCKFunc;

typedef struct {
    LPCInherit inherit;		/* program */
    uint8_t func;		/* index in function table */
    uint8_t nargs;		/* # arguments */
} LPCDFunc;

typedef struct {
    uint16_t call;		/* index in call table */
    uint8_t nargs;		/* # arguments */
} LPCVFunc;
