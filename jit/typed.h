class TVC {
public:
    TVC() { }
    TVC(Type type, LPCInt val) :
	type(type), val(val), code(NULL), merge(STACK_EMPTY) { }

    Type type;		/* type */
    LPCInt val;		/* value, only relevant for integer on the stack */
    Code *code;		/* instruction that consumes this TVC */
    StackSize merge;	/* merge reference */
};

class BlockContext {
public:
    BlockContext(CodeFunction *func, StackSize size);
    virtual ~BlockContext();

    void prologue(Type *mergeParams, Type *mergeLocals, StackSize mergeSp,
		  Block *b);
    void push(TVC val);
    void push(Type type, LPCInt val = 0) {
	push(TVC(type, val));
    }
    TVC pop(Code *code);
    TVC indexed();
    bool stores(int count, Code *popCode);
    bool storeN();
    Code *storePop() {
	return storeCode;
    }
    void spread() {
	spreadArgs = true;
    }
    Type kfun(LPCKFunCall *kf, Code *code);
    void args(int nargs, Code *code);
    StackSize merge(StackSize codeSp);
    bool changed();
    TVC get(StackSize stackPointer);
    Code *consumer(StackSize stackPointer, Type type);
    StackSize nextSp();

    Type *params;		/* function parameter types */
    Type *locals;		/* function local variable types */
    LPCParam nParams;		/* # parameters */
    LPCLocal nLocals;		/* # local variables */
    StackSize sp;		/* stack pointer */
    Type castType;		/* CASTX argument */
    CodeLine line;		/* current line */
    StackSize level;		/* catch level */

private:
    static Type mergeType(Type type1, Type type2);

    Type *origParams;		/* original parameter types */
    Type *origLocals;		/* original local variable types */
    Stack<TVC> *stack;		/* type/val/code stack */
    TVC altStack[3];		/* alternative stack */
    StackSize altSp;		/* alternative stack pointer */
    int storeCount;		/* number of STOREX instructions left */
    Code *storeCode;		/* pop at end of STORES? */
    bool spreadArgs;		/* SPREAD before call? */
    bool merging;		/* merging stack values? */
};

class TypedCode : public Code {
public:
    TypedCode(CodeFunction *function);
    virtual ~TypedCode();

    virtual void evaluateTypes(BlockContext *context);
    StackSize stackPointer() { return sp; }

    static Code *create(CodeFunction *function);

protected:
    static Type simplifiedType(Type type);

private:
    StackSize sp;		/* stack pointer */
};

class TypedBlock : public Block {
public:
    TypedBlock(Code *first, Code *last, CodeSize size);
    virtual ~TypedBlock();

    virtual Type paramType(LPCParam param);
    virtual Type localType(LPCLocal local);
    virtual void setContext(BlockContext *context, Block *b);
    virtual void evaluateTypes(BlockContext *context, Block **list);
    void evaluate(BlockContext *context);

    static Block *create(Code *first, Code *last, CodeSize size);

private:
    Type *params;		/* parameter types at end of block */
    Type *locals;		/* local variable types at end of block */
    StackSize endSp;		/* final stack pointer */
};
