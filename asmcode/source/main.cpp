#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <ctype.h>

#include "../include/assemble.h"
#include "../../lib/include/fileUtils.h"
#include "../../lib/include/debug.h"
#include "../include/assemble.h"

int main(int argc, char** argv) {
    if (argc < 3) {
        fprintf(stderr, "!  not enough files\n");
        return 0;
    }

    FILE* scriptFile   = fopen(argv[1], "r");
    if (scriptFile == nullptr) {
        fprintf(stderr, "!  cant open file to read: %s\n", argv[1]);
        return 0;
    }

    FILE* byteCodeFile = fopen(argv[2], "wb");
    if (byteCodeFile == nullptr) {
        fprintf(stderr, "!  cant open file write: %s\n", argv[2]);
        return 0;
    }

    FileData scriptData = {};
    GetData(&scriptData, scriptFile);
    fclose(scriptFile);

    // DataDump(&scriptData);

    AsmState state = Assemble(&scriptData, byteCodeFile);

    fclose(byteCodeFile);
    FreeData(&scriptData);

    switch (state.aec) {
        case SUCCESS:
            break;
        case FAILURE:
            fprintf(stderr, "!  error occured on line( %lu )\n", state.line);
            break;
        default:
            ASSERT(0 && "!  UNKNOWN ASM STATE");
            break;
    }

    return 0;
}
