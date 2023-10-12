#include "../include/assemble.h"

//-----------------------------------------

static size_t MinBinBufAlloc = 64;

static AsmState ParseAndStore(String* line, BinBuf* binBuf);

// static void PutHex(FILE* file, int n);

// static void PutHex(FILE* file, uint8_t n);

//-----------------------------------------

AsmState Assemble(FileData* input, FILE* output) {
    ASSERT(input != nullptr);
    ASSERT(output != nullptr);

    AsmState state = {SUCCESS, 0};

    BinBuf binBuf = {nullptr, 0, 0};

    binBuf.buf = (Byte*)calloc(MinBinBufAlloc, sizeof(Byte)); //TODO - add check
    binBuf.cap = MinBinBufAlloc;
    binBuf.size = 0;

    String* text = input->text;

    while (text < input->text + input->nLines) {
        state = ParseAndStore(text, &binBuf);

        text++;
    }

    binBuf.buf = (Byte*)realloc(binBuf.buf, binBuf.size);

    fwrite(binBuf.buf, sizeof(Byte), binBuf.size, output);

    free(binBuf.buf);
    binBuf.buf = nullptr;

    return state;
}

//-----------------------------------------

static AsmState ParseAndStore(String* line, BinBuf* binBuf) {
    ASSERT(line        != nullptr);
    ASSERT(binBuf      != nullptr);
    ASSERT(binBuf->buf != nullptr);

    AsmState state = {SUCCESS, 0};

    char holdCommand[100] = "";
    char holdStrArg[100] = "";
    int holdIntArg = 0;

    size_t cmdPos = 0;

    while (cmdPos <  CommandCount) {

        if (strncmp(CommandNames[cmdPos], line->str, strlen(CommandNames[cmdPos])) == 0) {
            if (sscanf(line->str, "%s %d", holdCommand, &holdIntArg) == 2) { // команда число
                uint8_t code = 0b0000'0000;

                code |= CommandIds[cmdPos] & CODE_ID_MASK;
                code |= ARG_FORMAT_IMMED;

                int cnst = holdIntArg;

                if (binBuf->cap - binBuf->size < sizeof(cnst) + sizeof(code)) {
                    binBuf->cap *= 2;
                    binBuf->buf = (Byte*)realloc(binBuf->buf, binBuf->cap);
                }

                memcpy(binBuf->buf + binBuf->size, &code, sizeof(code));
                binBuf->size += sizeof(code);
                memcpy(binBuf->buf + binBuf->size, &cnst, sizeof(cnst));
                binBuf->size += sizeof(cnst);
            } else if (sscanf(line->str, "%s %s %d", holdCommand, holdStrArg, &holdIntArg) == 3) { // команда регистр число
                uint8_t code = 0b0000'0000;

                code |= CommandIds[cmdPos] & CODE_ID_MASK;
                code |= ARG_FORMAT_IMMED | ARG_FORMAT_REG;

                int reg = RegIds[holdStrArg[1] - 'a'];
                int cnst = holdIntArg;

                if (binBuf->cap - binBuf->size < sizeof(cnst) + sizeof(code) + sizeof(reg)) {
                    binBuf->cap *= 2;
                    binBuf->buf = (Byte*)realloc(binBuf->buf, binBuf->cap);
                }

                memcpy(binBuf->buf + binBuf->size, &code, sizeof(code));
                binBuf->size += sizeof(code);
                memcpy(binBuf->buf + binBuf->size, &reg, sizeof(reg));
                binBuf->size += sizeof(reg);
                memcpy(binBuf->buf + binBuf->size, &cnst, sizeof(cnst));
                binBuf->size += sizeof(cnst);
            } else if (sscanf(line->str, "%s %s", holdCommand, holdStrArg) == 2) { // команда регистр
                uint8_t code = 0b0000'0000;

                code |= CommandIds[cmdPos] & CODE_ID_MASK;
                code |= ARG_FORMAT_REG;

                int reg = RegIds[holdStrArg[1] - 'a'];

                if (binBuf->cap - binBuf->size <sizeof(code) + sizeof(reg)) {
                    binBuf->cap *= 2;
                    binBuf->buf = (Byte*)realloc(binBuf->buf, binBuf->cap);
                }

                memcpy(binBuf->buf + binBuf->size, &code, sizeof(code));
                binBuf->size += sizeof(code);
                memcpy(binBuf->buf + binBuf->size, &reg, sizeof(reg));
                binBuf->size += sizeof(reg);
            } else {
                uint8_t code = 0b0000'0000;

                code |= CommandIds[cmdPos] & CODE_ID_MASK;

                memcpy(binBuf->buf + binBuf->size, &code, sizeof(code));
                binBuf->size += sizeof(code);
            }
            break;
        }

        cmdPos++;
    }

    return state;
}


