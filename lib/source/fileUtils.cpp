#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "../include/debug.h"

#include "../include/fileUtils.h"

// ---------------------------------------

static ssize_t FileSize(FILE* file);

static void FileRead(char* buf, size_t fileSize, FILE* fileToRead);

static size_t CountLines(const char* str, const size_t len);

static void FillText(FileData* data);

static void ReplaceChar(char* str, size_t len);

// --------------------------------------

void DataDump(FileData* data) {
  ASSERT(data != nullptr);

  printf("\n#  FileData {\n");
  printf(  "#    buf [ %p ]\n", data->buf);
  printf(  "#    bufSize { %lu }\n", data->bufSize);
  printf(  "#    text [ %p ]\n", data->text);
  printf(  "#    nLines { %lu }\n", data->nLines);
  printf(  "#  }\n\n");
}

void GetData(FileData* data, FILE* file) {
  ASSERT(data != nullptr);
  ASSERT(file != nullptr);

  data->bufSize = (size_t)FileSize(file) + 1;

  data->buf = (char*)calloc(data->bufSize, sizeof(char));

  FileRead(data->buf, data->bufSize, file);

  data->nLines = CountLines(data->buf, data->bufSize); // i dont wanna adr last line

  ReplaceChar(data->buf, data->bufSize);

  data->text = (String*)calloc(data->nLines+1, sizeof(String));
  FillText(data);
}

void PutBuf(FileData* data, FILE* file) {
  ASSERT(data != nullptr);
  ASSERT(file != nullptr);

  fputs(data->buf, file);
}

void FreeData(FileData* data) {
  ASSERT(data != nullptr);

  free(data->buf);
  data->buf = nullptr;

  free(data->text);
  data->text = nullptr;
}

// -------------------------------------

static ssize_t FileSize(FILE* file) {
  ASSERT(file != nullptr);

  fseek(file, 0, SEEK_END);

  ssize_t size = ftell(file);

  fseek(file, 0, SEEK_SET);

  return size;
}

static void FileRead(char* buf, size_t fileSize, FILE* fileToRead) {
  ASSERT(buf        != nullptr);
  ASSERT(fileToRead != nullptr);

  fread(buf, sizeof(char), fileSize, fileToRead);
  *(buf + fileSize - 2) = '\n';
  *(buf + fileSize - 1) = '\0';
}

static size_t CountLines(const char* str, const size_t len) {
  ASSERT(str != nullptr);

  const char* iter = str;
  size_t lines = 0;

  while (iter < str + len) {
    if (*iter == '\n') {
      lines++;
    }
    iter++;
  }

  return lines;
}

static void ReplaceChar(char* str, size_t len) {
  char* moveStr = str;
  while (moveStr < str + len) {
    if (*moveStr == '\n')
      *moveStr = '\0';
    moveStr++;
  }
}

static void FillText(FileData* data) {
  ASSERT(data != nullptr);

  String* fillText = data->text;
  char* iter = data->buf;
  char* hold = iter;

  while (iter < data->buf + data->bufSize) {
    if (*iter == '\0') {
      fillText->str = hold;
      fillText->len = (size_t)(iter - hold);

      hold = iter + 1;
      fillText++;
    }

      iter++;
  }
}
