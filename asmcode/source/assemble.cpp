#include "../include/assemble.h"

//-----------------------------------------

static size_t Walkthrough = 0;

static size_t MinBinBufAlloc = 64;

static void ParseAndStore(String* line, BinBuf* binBuf, DArray* labelArr);

static void PutJmp(cmdKey_t cmdKey, jmpAdr_t jmpAdr, BinBuf* binBuf);
static void PutNoArgs(cmdKey_t cmdKey, BinBuf* binbuf);
static void PutImmed(cmdKey_t cmdKey, immed_t immed, BinBuf* binBuf);
static void PutReg(cmdKey_t cmdKey, regId_t reg, BinBuf* binBuf);
static void PutBoth(cmdKey_t cmdKey, regId_t reg, immed_t immed, BinBuf* binBuf);

static Args GetArgs(char* str, char* holdReg, immed_t* holdImmed);
static void GetJmpAdr(char* str, char* holdJmpLbl, jmpAdr_t* jmpAdr, DArray* labelArr);

//-----------------------------------------

void Assemble(FileData* input, FILE* output) {
  ASSERT(input != nullptr);
  ASSERT(output != nullptr);

  BinBuf binBuf = {nullptr, 0, 0};
  binBuf.buf = (byte_t*)calloc(MinBinBufAlloc, sizeof(byte_t)); //TODO - add check
  binBuf.cap = MinBinBufAlloc;
  binBuf.size = 0;

  Assembler assembler = {binBuf, {nullptr, 0, 0, 0}, *input, output};
  DArrayCtor(&assembler.labelArr, sizeof(Label));

  String* text = nullptr;

  //++++++++++[проход проход]++++++++++
  text = input->text;
  Walkthrough++;

  while (text < input->text + input->nLines) {
    ParseAndStore(text, &binBuf, &assembler.labelArr);

    text++;
  }
  //++++++++++[конец прохода]++++++++++
  binBuf.size = 0;
  //++++++++++[второй проход]++++++++++
  text = input->text;
  Walkthrough++;

  while (text < input->text + input->nLines) {
    ParseAndStore(text, &binBuf, &assembler.labelArr);

    text++;
  }
  //++++++++++[конец прохода]++++++++++

  fwrite(binBuf.buf, sizeof(byte_t), binBuf.size, output);

  DArrayDtor(&assembler.labelArr);
  free(binBuf.buf);
  binBuf.buf = nullptr;
}

void DumpLabel(void* label) {
  Label lbl = *(Label*)label;

  fprintf(stderr, "{ ");

  fprintf(stderr, "{ ");
  for (size_t i = 0; i < lbl.name.len; i++) {
      fputc(lbl.name.str[i], stderr);
  }
  fprintf(stderr, " } ");

  fprintf(stderr, "{ ");
  fprintf(stderr, "%lu", lbl.name.len);
  fprintf(stderr, " } ");

  fprintf(stderr, "{ ");
  fprintf(stderr, "%lf", lbl.pos);
  fprintf(stderr, " }");

  fprintf(stderr, " }");
}

//-----------------------------------------

// push [rax + 5]
// push [rax]
// push [5]
// push rax + 5
// push rax
// push 5

#define DEF_CMD(name, num, isJump, aldArgs, ...) \
  if (strcmp(holdCommand, #name) == 0) { \
    if (isJump) { \
      if (Walkthrough == 1) { \
        cmdKey_t cmdKey = 0 | num | BitFlags::ARG_IMMED; \
        jmpAdr_t jmpAdr = 0; \
\
        PutJmp(cmdKey, jmpAdr, binBuf); \
      } else { \
        cmdKey_t cmdKey = 0 | num | BitFlags::ARG_IMMED; \
        jmpAdr_t jmpAdr = 0; \
\
        GetJmpAdr(line->str, holdJmpLbl, &jmpAdr, labelArr); \
\
        PutJmp(cmdKey, jmpAdr, binBuf); \
      } \
    } else if (haveArgs) { \
      Args nArgs = GetArgs(line->str, holdRegArg, &holdImmedArg); \
      switch (nArgs) { \
        case Args::REG_AND_IMMED: { \
          cmdKey_t cmdKey = 0 | num | BitFlags::ARG_IMMED | BitFlags::ARG_REG; \
          regId_t reg = (regId_t)RegIds[holdRegArg[1] - 'a']; \
          immed_t immed = holdImmedArg; \
\
          PutBoth(cmdKey, reg, immed, binBuf); \
          break; \
        } \
        case Args::ONLY_REG: { \
          cmdKey_t cmdKey = 0 | num | BitFlags::ARG_REG; \
          regId_t reg = (regId_t)RegIds[holdRegArg[1] - 'a']; \
\
          PutReg(cmdKey, reg, binBuf); \
          break; \
        } \
        case Args::ONLY_IMMED: { \
          cmdKey_t cmdKey = 0 | num | BitFlags::ARG_IMMED; \
          immed_t immed = holdImmedArg; \
\
          PutImmed(cmdKey, immed, binBuf); \
          break; \
        } \
        case Args::NO_ARGS: { \
          break; \
        } \
        case Args::INIT_ARGS: {} \
        default: { \
          ASSERT(0 && "!  unknown number of args"); \
          break; \
        } \
      } \
    } else { \
      PutNoArgs(num, binBuf); \
    } \
  } else

static void ParseAndStore(String* line, BinBuf* binBuf, DArray* labelArr) {
    ASSERT(line            != nullptr);
    ASSERT(binBuf          != nullptr);
    ASSERT(binBuf->buf     != nullptr);
    ASSERT(labelArr        != nullptr);
    ASSERT(labelArr->array != nullptr);

    char holdCommand[100] = "";
    char holdRegArg[100] = "";
    char holdJmpLbl[100] = "";
    immed_t holdImmedArg = 0;

    if (Strchr(line, ':') != nullptr) { //label:
        if (Walkthrough == 1) {
            Label lbl = {{line->str, (size_t)(Strchr(line, ':') - line->str)}, (jmpAdr_t)binBuf->size}; // size = смещение от начала
            DArrayPushBack(labelArr, &lbl);
        } else {
            ;
        }
    } else { // обычная команда
        sscanf(line->str, "%s", holdCommand); //FIXME -  ограничить размер

        #include "../../shared/include/commandSet.h"

        /* else */ {};
    }
}

#undef DEF_CMD

static void PutJmp(cmdKey_t cmdKey, jmpAdr_t jmpAdr, BinBuf* binBuf) {
    if (binBuf->cap - binBuf->size < sizeof(cmdKey) + sizeof(jmpAdr)) {
        binBuf->cap *= 2;
        binBuf->buf = (byte_t*)realloc(binBuf->buf, binBuf->cap);
    }

    memcpy(binBuf->buf + binBuf->size, &cmdKey, sizeof(cmdKey));
    binBuf->size += sizeof(cmdKey);

    memcpy(binBuf->buf + binBuf->size, &jmpAdr, sizeof(jmpAdr));
    binBuf->size += sizeof(jmpAdr);
}

static void PutNoArgs(cmdKey_t cmdKey, BinBuf* binBuf) {
    if (binBuf->cap - binBuf->size < sizeof(cmdKey)) {
        binBuf->cap *= 2;
        binBuf->buf = (byte_t*)realloc(binBuf->buf, binBuf->cap);
    }

    memcpy(binBuf->buf + binBuf->size, &cmdKey, sizeof(cmdKey));
    binBuf->size += sizeof(cmdKey);
}

static void PutImmed(cmdKey_t cmdKey, immed_t immed, BinBuf* binBuf) {
    if (binBuf->cap - binBuf->size < sizeof(cmdKey) + sizeof(immed)) {
        binBuf->cap *= 2;
        binBuf->buf = (byte_t*)realloc(binBuf->buf, binBuf->cap);
    }

    memcpy(binBuf->buf + binBuf->size, &cmdKey, sizeof(cmdKey));
    binBuf->size += sizeof(cmdKey);

    memcpy(binBuf->buf + binBuf->size, &immed, sizeof(immed));
    binBuf->size += sizeof(immed);
}

static void PutReg(cmdKey_t cmdKey, regId_t reg, BinBuf* binBuf) {
    if (binBuf->cap - binBuf->size < sizeof(cmdKey) + sizeof(reg)) {
        binBuf->cap *= 2;
        binBuf->buf = (byte_t*)realloc(binBuf->buf, binBuf->cap);
    }

    memcpy(binBuf->buf + binBuf->size, &cmdKey, sizeof(cmdKey));
    binBuf->size += sizeof(cmdKey);

    memcpy(binBuf->buf + binBuf->size, &reg, sizeof(reg));
    binBuf->size += sizeof(reg);
}

static void PutBoth(cmdKey_t cmdKey, regId_t reg, immed_t immed, BinBuf* binBuf) {
    if (binBuf->cap - binBuf->size < sizeof(cmdKey) + sizeof(reg) + sizeof(immed)) {
        binBuf->cap *= 2;
        binBuf->buf = (byte_t*)realloc(binBuf->buf, binBuf->cap);
    }

    memcpy(binBuf->buf + binBuf->size, &cmdKey, sizeof(cmdKey));
    binBuf->size += sizeof(cmdKey);

    memcpy(binBuf->buf + binBuf->size, &reg, sizeof(reg));
    binBuf->size += sizeof(reg);

    memcpy(binBuf->buf + binBuf->size, &immed, sizeof(immed));
    binBuf->size += sizeof(immed);
}

static Args GetArgs(char* str, char* holdReg, immed_t* holdImmed) {
    char holdCommand[100] = "";

    if (sscanf(str, "%s %lf", holdCommand, holdImmed) == 2) {
        return Args::ONLY_IMMED;
    } else if (sscanf(str, "%s %s %lf", holdCommand, holdReg, holdImmed) == 3) {
        return Args::REG_AND_IMMED;
    } else if (sscanf(str, "%s %s", holdCommand, holdReg) == 2) {
        return Args::ONLY_REG;
    } else {
        return Args::NO_ARGS;
    }
}

static void GetJmpAdr(char* str, char* holdJmpLbl, jmpAdr_t* jmpAdr, DArray* labelArr) {
    char holdCommand[100] = "";

    sscanf(str, "%s %s", holdCommand, holdJmpLbl);
    String label = {holdJmpLbl, (size_t)(strchr(holdJmpLbl, '\0') - holdJmpLbl)};

    Label* ptr = nullptr;

    for (size_t i = 0; i < labelArr->size; i++) {
        DArrayAt(labelArr, i, (void**)&ptr);
        if (!Strcmp((String*)ptr, &label)) {
            *jmpAdr = ptr->pos;
        }
    }
}
