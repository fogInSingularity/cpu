#ifndef ASSEMBLE_H
#define ASSEMBLE_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "../../lib/include/fileUtils.h"
#include "../../shared/include/commands.h"

typedef uint8_t Byte;

struct BinBuf {
    Byte* buf;
    size_t size;
    size_t cap;
};

enum AsmExitCode {
    SUCCESS = 0,
    FAILURE = 1,
};

struct AsmState {
    AsmExitCode aec;
    size_t line;
};

AsmState Assemble(FileData* input, FILE* output);

#endif // ASSEMBLE_H
