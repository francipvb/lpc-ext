typedef struct {
    uint8_t major;	/* 2 */
    uint8_t minor;	/* 1 */
    size_t intSize;	/* size of Int */
    size_t inheritSize;	/* size of inherit offset */
    int mapSize;	/* size of kfun map */
    int nProtos;	/* # prototypes */
    size_t protoSize;	/* size of all prototypes together */
} JitInfo;

typedef struct {
    int nInherits;	/* # inherited objects */
    int nFunctions;	/* # functions */
    size_t progSize;	/* program size */
    size_t fTypeSize;	/* function type size */
    size_t vTypeSize;	/* variable type size */
} JitCompile;
