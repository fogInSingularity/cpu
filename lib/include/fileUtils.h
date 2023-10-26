#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <stdio.h>

#include "mystring.h"

/// @brief struct that holds file contents
struct FileData {
  char* buf;      ///< buffer that holds file string
  size_t bufSize; ///< size of buffer in bytes

  String* text;   ///< string array
  size_t nLines;  ///< number of strings
};


void DataDump(FileData* data);


/**
 * @brief get data from file
 *
 * @param data FileData struct
 * @param file file to get data from
*/
void GetData(FileData* data, FILE* file);

/**
 * @brief put buffer to file
 *
 * @param data FileData struct
 * @param file file to put buffer to
*/
void PutBuf(FileData* data, FILE* file);


/**
 * @brief free all data members
 *
 * @param data FileData struct
*/
void FreeData(FileData* data);

#endif // FILE_UTILS_H
