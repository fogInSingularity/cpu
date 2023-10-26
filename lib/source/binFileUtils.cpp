#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "../include/debug.h"
#include "../include/binFileUtils.h"

// ---------------------------------------

static size_t FileSize(FILE* file);

static void FileRead(BinData* data, FILE* file);

// --------------------------------------

void GetData(BinData* data, FILE* file) {
  ASSERT(data != nullptr);
  ASSERT(file != nullptr);

  data->bufSz = FileSize(file);

  data->buf = (byte_t*)calloc(data->bufSz, sizeof(char));

  FileRead(data, file);
}

void PutBuf(BinData* data, FILE* file) {
  ASSERT(data != nullptr);
  ASSERT(data != nullptr);
  ASSERT(file != nullptr);

  fwrite(data->buf, sizeof(byte_t), data->bufSz, file);
}

void FreeData(BinData* data) {
  ASSERT(data != nullptr);

  free(data->buf);
  data->buf = nullptr;
}

// -------------------------------------

static size_t FileSize(FILE* file) {
  ASSERT(file != nullptr);

  fseek(file, 0, SEEK_END);

  size_t size = (size_t)ftell(file);

  fseek(file, 0, SEEK_SET);

  return size;
}

static void FileRead(BinData* data, FILE* file) {
  ASSERT(data      != nullptr);
  ASSERT(data->buf != nullptr);

  fread(data->buf, sizeof(byte_t), data->bufSz, file);
}
