# include <stdlib.h>
# include <unistd.h>
# include <stdint.h>
# include <stdarg.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <string.h>
# include <stdio.h>
extern "C" {
# include "lpc_ext.h"
# include "jit.h"
}
# include "data.h"
# include "code.h"
# include "stack.h"
# include "block.h"
# include "typed.h"
# include "flow.h"
# include "genclang.h"
# include "jitcomp.h"

/*
 * NAME:	fatal()
 * DESCRIPTION:	fatal error
 */
void fatal(const char *format, ...)
{
    va_list argp;
    char buf[1024];

    va_start(argp, format);
    vsprintf(buf, format, argp);
    va_end(argp);
    fprintf(stderr, "Fatal error: %s\n", buf);

    abort();
}

static CodeContext *cc;

static void jit_compile(int nInherits, uint8_t *prog, int nFunctions,
			uint8_t *funcTypes, uint8_t *varTypes)
{
    if (nFunctions != 0) {
	Code::producer(&ClangCode::create);
	Block::producer(&ClangBlock::create);
	CodeObject object(cc, nInherits, funcTypes, varTypes);

	do {
	    CodeFunction func(&object, prog);
	    CodeSize size;
	    Block *b = Block::function(&func);

	    if (b != NULL) {
		size = b->fragment();
		if (size != 0) {
		    b->emit(&func, size);
		}
		b->clear();
	    }
	    prog = func.endProg();
	    fprintf(stderr, "\n");
	} while (--nFunctions != 0);
    }
}

static void filename(char *buffer, uint8_t *hash)
{
    static const char hex[] = "0123456789abcdef";
    int i;

    sprintf(buffer, "cache/%c%c/", hex[hash[0] >> 4], hex[hash[0] & 0xf]);
    buffer += 9;
    for (i = 0; i < 16; i++) {
	*buffer++ = hex[hash[i]>> 4];
	*buffer++ = hex[hash[i] & 0xf];
    }
    *buffer = '\0';
}

int main(int argc, char *argv[])
{
    JitInfo info;
    uint8_t hash[16];
    char reply;

    if (argc != 2 || chdir(argv[1]) < 0) {
	return 1;
    }

    /* read init params from 0 */
    if (read(0, &info, sizeof(JitInfo)) != sizeof(JitInfo)) {
	return 2;
    }
    uint8_t protos[info.protoSize];
    if (read(0, protos, info.protoSize) != info.protoSize) {
	return 2;
    }

    if (!CodeContext::validVM(info.major, info.minor)) {
	reply = false;
	write(1, &reply, 1);
	return 3;
    }

    cc = new CodeContext(info.intSize, info.inheritSize, protos, info.nBuiltins,
			 info.nKfuns);
    reply = true;
    write(1, &reply, 1);

    while (read(0, hash, 16) == 16) {
	char path[1000];
	JitCompile comp;
	int fd;

	filename(path, hash);
	fd = open(path, O_RDONLY);
	read(fd, &comp, sizeof(JitCompile));
	uint8_t prog[comp.progSize];
	read(fd, prog, comp.progSize);
	uint8_t ftypes[comp.fTypeSize];
	read(fd, ftypes, comp.fTypeSize);
	uint8_t vtypes[comp.vTypeSize];
	read(fd, vtypes, comp.vTypeSize);
	close(fd);

	jit_compile(comp.nInherits, prog, comp.nFunctions, ftypes, vtypes);
    }

    return 0;
}
